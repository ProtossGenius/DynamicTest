#ifndef USER_H_UV8CBAVZ
#define USER_H_UV8CBAVZ

#include <mutex>
#include <atomic>
#include <memory>
#include <string>
#include <map>
#include "smncpp/channel.h"
#include "strategy.h"
#include <boost/asio/io_service.hpp>
#include "smncpp/lockm.h"
#include <functional>

namespace smdtest{

	class User{
		public:
			User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy):_ioc(ioc), _recvChan(200), _strategy(strategy), _process(strategy->getProcess()),
				_alive(true){ _recvChan.setExport(&_pkg);}
		public:
			void start();
			void setAlive(bool alive){this->_alive = alive;}
			//getRecvChan unsafe. maybe action will get package(like url-test). but use chan may cause dead-lock. should call in sync.
			smnet::channel<void*>& getRecvChan(){return this->_recvChan;}
			//getData return value's pointer. usually should not new create data or you must remember free it..
			virtual void* getData(const std::string& type, const std::string& key) = 0;
			//setSession manage socket. get session should from getData.
			virtual void setSession(const std::string& key, void* session) = 0;
		protected:
			//get a package.
			virtual void* recv_once() = 0;
			//deal with the package . and update user's data(action should not change user's data).
			virtual void recivePkg(void*) = 0;
		private:
			void Recive(void* pkg);
			void doAction();
			void doRecv();
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
