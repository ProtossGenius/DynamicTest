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

namespace smdtest{
	class User{
		public:
			User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy):_ioc(ioc), _strategy(strategy), _process(strategy->getProcess()),
				_alive(true){}
		public:
			void start();
			void setAlive(bool alive){this->_alive = alive;}
		protected:
			virtual void action_once() = 0;
			virtual void* recv_once() = 0;
		private:
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
	};
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
