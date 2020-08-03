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
			User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy):_ioc(ioc), _recvChan(200), _strategy(strategy), _process(strategy->getProcess()),
				_alive(true), _pkg(nullptr){ _recvChan.setExport(&_pkg);}
		public:
			void start();
			void setAlive(bool alive){this->_alive = alive;}
			//getRecvChan all message send to _recvChan, it's thread-safe.
			smnet::channel<void*>& getRecvChan(){return this->_recvChan;}
		public: 
			//getData return value's pointer. usually should not new create data or you must remember free it..
			virtual void* getData(const std::string& type, const std::string& key) = 0;
			//setSession manage socket. get session should from getData.
			virtual void setSession(const std::string& key, void* session) = 0;
			virtual std::string statusJson() = 0;
		protected:
			//deal with the package . and update user's data(action should not change user's data).
			//if pkg need free, should free in here. 
			virtual void recivePkg(void* pkg) = 0;
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
			void* _pkg;
	};
	
	template<typename DataType>
	DataType& getUserData(User& usr, const std::string& type, const std::string& key){
		return *(DataType*)usr.getData(type, key);
	}
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
