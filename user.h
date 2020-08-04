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
		public:
			User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy):_ioc(ioc), _recvChan(200), _strategy(strategy), 
				_alive(true), _deadLock(true), _pkg(nullptr){ _recvChan.setExport(&_pkg);this->_process = this->_strategy->getProcess(*this, "");}
			void start();
			bool isDeadLock(){
				if(_deadLock)return true;
				_deadLock = true;
				return false;
			}
			bool isAlive(){return this->_alive;}
			//getRecvChan all message send to _recvChan, it's thread-safe.
			smnet::channel<void*>& getRecvChan(){return this->_recvChan;}
			void onDisconnect(const std::string& sId);
		public: 
			//getData return value's pointer. usually should not new create data or you must remember free it..
			virtual void* getData(const std::string& type, const std::string& key) = 0;
			//uid get user's Unique id.
			virtual std::string uid();
			//statusJson return User's status.
			virtual std::string statusJson() = 0;
			//logData log data. (it always be call when be closed == Do loop end.).
			void logData() {
				smnet::SMLockMgr _(this->_tsafe);
				_logData();
			}
			//anything should do when close User.
			void close(){
				//recv maybe in block if recvChan is empty. should cancel the block and free resourece.
				this->_alive = false;
				if (this->_recvChan.empty()){
					this->_recvChan.push(nullptr);
				}
				_close();				
			}
		protected:
			//deal with the package . and update user's data(action should not change user's data).
			//if pkg need free, should free in here. 
			virtual void recivePkg(void* pkg) = 0;
			//you should free resource here, such as sessions.
			virtual void _close() {}
			virtual void _logData() = 0;
		private:
			void Recive(void* pkg);
			void doAction();
			void dealPkg();
		private:
			boost::asio::io_service& _ioc;
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
	DataType& getUserData(User& usr, const std::string& type, const std::string& key){
		return *(DataType*)usr.getData(type, key);
	}
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
