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
			virtual ~User(){if (this->_worker != nullptr){delete _worker;}}
			void start();
			bool isInDeadLock(){
				if(_deadLock)return true;
				_deadLock = true;
				return false;
			}
			bool isAlive(){return this->_alive;}
			void onDisconnectWithLock(const std::string& sId);
			void push(void* pkg);
			void setStrategyWithLock(std::shared_ptr<Strategy> ns){
				lockm _(this->_tsafe);
				this->_strategy->getTicker()->setTickDo(nullptr);
				this->_strategy = ns;
				start();
			}

		public: 
			boost::asio::io_service& getIoSvc(){return _ioc;}


			//statusJson return User's status.
			std::string statusJsonWithLock() {
				lockm _(this->_tsafe);
				return  this->_statusJson();
			}

			virtual std::string uid() = 0;

			// logDataWithLock 输出User数据.
			void logDataWithLock() {
				lockm _(this->_tsafe);
				_logData();
			}
			//statusJson return User's status.
			std::string statusJson() {
				return  this->_statusJson();
			}
			//logData log data. 
			void logData() {
				_logData();
			}
		protected:
			//recivePkg 处理数据包， 更新数据始终应该在这里进行.
			virtual void recivePkg(void* pkg) = 0;
			// freePkg if pkg need free, should free in here. 
			virtual void freePkg(void *pkg) = 0;
			// _close 关闭时需要做到事情.
			virtual void _close() {}
			virtual void _logData() = 0;
			virtual void* _getData(const std::string& type, const std::string& key) = 0;
			virtual std::string _statusJson() = 0;
			std::shared_ptr<Process> currentProcess(){
				return this->_process;
			}
		public: // 获得数据
			
			template<typename DataType>
			std::shared_ptr<DataType> getSharedData(const std::string& type, const std::string &key){
				return std::shared_ptr<DataType>((DataType*)this->_getData(type, key));
			}
			template<typename DataType>
			std::shared_ptr<DataType> getSharedDataWithLock(const std::string& type, const std::string &key){
				lockm _(this->_tsafe);
				return getSharedData<DataType>(type, key);
			}
			//getData return value's pointer. usually should not new create data or you must remember free it..
			template<typename DataType>
			const DataType& getDataRef(const std::string& type, const std::string& key){
				return *(DataType*)this->_getData(type, key);
			}
			
			template<typename DataType>
			const DataType& getDataRefWithLock(const std::string& type, const std::string& key) {
				lockm _(this->_tsafe);
				return getDataRef<DataType>(type, key);
			}
		private:
			void close();
			/* Recive 
			 * 1. 首先根据用户包更新User数据,
			 * 2. 之后传递给当前Action进行处理.
			 * 3. 释放数据包(回收)
			 * */
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
		private:
			template<typename DataType>
			friend const DataType& getRefData(User& usr, const std::string& type, const std::string& key);
			template<typename DataType>
			friend const DataType& getRefDataWithLock(User& usr, const std::string& type, const std::string& key);
			friend class Strategy;

	};
	
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
