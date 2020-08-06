#ifndef USER_H_UV8CBAVZ
#define USER_H_UV8CBAVZ

#include <atomic>
#include <boost/asio/io_service.hpp>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "smncpp/channel.h"
#include "smncpp/lockm.h"
#include "strategy.h"

namespace smdtest{

	class User{
		typedef smnet::SMLockMgr lockm;
		public:
			User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy);
			virtual ~User(){}
			void start();
			bool isDeadLock(){
				if(_deadLock)return true;
				_deadLock = true;
				return false;
			}
			bool isAlive(){return this->_alive;}
			void push(void* pkg);
			void onDisconnect(const std::string& sId);
			void setStrategy(std::shared_ptr<Strategy> ns){
				smnet::SMLockMgr _(this->_tsafe);
				this->_strategy->getTicker()->close();
				this->_strategy = ns;
				start();
			}

		public: 
			boost::asio::io_service& getIoSvc(){return _ioc;}

			//getData return value's pointer. usually should not new create data or you must remember free it..
			void* getData(const std::string& type, const std::string& key){
				return this->_getData(type, key);
			}
			//never call it in Action but should call it in another.
			void* getDataByLock(const std::string& type, const std::string& key) {
				lockm _(this->_tsafe);
				return this->_getData(type, key);
			}

			//statusJson return User's status.
			std::string statusJsonByLock() {
				lockm _(this->_tsafe);
				return  this->_statusJson();
			}

			virtual std::string uid() = 0;

			//logData log data. (it always be call when be closed == Do loop end.).
			void logDataByLock() {
				smnet::SMLockMgr _(this->_tsafe);
				_logData();
			}
			//statusJson return User's status.
			std::string statusJson() {
				lockm _(this->_tsafe);
				return  this->_statusJson();
			}
			//logData log data. (it always be call when be closed == Do loop end.).
			void logData() {
				smnet::SMLockMgr _(this->_tsafe);
				_logData();
			}
			//anything should do when close User.
			void close(){
				//recv maybe in block if recvChan is empty. should cancel the block and free resourece.
				this->_alive = false;
				this->_strategy->getTicker()->setTickDo([]{});
				delete this->_worker;
				this->_worker = nullptr;
				if (this->_recvChan.empty()){
					this->_recvChan.push(nullptr);
				}
				this->_logData();
				_close();				
			}
		protected:
			//deal with the package . and update user's data(action should not change user's data).
			//if pkg need free, should free in here. 
			virtual void recivePkg(void* pkg) = 0;
			//you should free resource here, such as sessions.
			virtual void _close() {}
			virtual void _logData() = 0;
			virtual void* _getData(const std::string& type, const std::string& key) = 0;
			virtual std::string _statusJson() = 0;
			std::shared_ptr<Process> currentProcess(){
				smnet::SMLockMgr _(this->_tsafe);
				return this->_process;
			}
			
		private:
			void Recive(void* pkg);
			void doAction();
			void dealPkg();
		private:
			boost::asio::io_service& _ioc;
			boost::asio::io_service::work* _worker;
			std::mutex _tsafe;
			smnet::channel<void*> _recvChan;
			int _interval;
			std::shared_ptr<Strategy> _strategy;
			std::shared_ptr<Process> _process;
		    std::atomic_bool _alive;
			std::atomic_bool _deadLock;
			void* _pkg;
	};
	
	template<typename DataType>
	std::shared_ptr<DataType> getSharedData(User& usr, const std::string& type, const std::string& key){
		return std::shared_ptr<DataType>((DataType*)usr.getData(type, key));
	}
	
	template<typename DataType>
	DataType& getRefData(User& usr, const std::string& type, const std::string& key){
		return *(DataType*)usr.getData(type, key);
	}

}

#endif /* end of include guard: USER_H_UV8CBAVZ */
