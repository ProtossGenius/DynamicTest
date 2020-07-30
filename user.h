#ifndef USER_H_UV8CBAVZ
#define USER_H_UV8CBAVZ

#include <mutex>
#include <memory>
#include <string>
#include <map>
#include "smncpp/channel.h"
#include "smncpp/base_asio_socket.h"
#include "process.h"

namespace smdtest{

	class User{
		public:
			void acceptConnect(std::string name, std::shared_ptr<smnet::SMConn> conn);
			void start();
		private:
			void recv();
		private:
			std::mutex _tsafe;
			std::map<std::string, std::shared_ptr<smnet::SMConn> > connMap;
			smnet::channel<void*> _recvChan;
			int _interval;
			Process _process;
	};
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
