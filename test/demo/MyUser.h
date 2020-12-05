#ifndef MYUSER_H_431TWJH2
#define MYUSER_H_431TWJH2
#include "smdtest/user.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace demo{
	class MyUser:public smdtest::User{
		public:
			MyUser(boost::asio::io_service& ioc, std::shared_ptr<smdtest::Strategy> strategy, const std::string& uuid):smdtest::User(ioc, strategy),m_Uid(uuid){}
		private:
			//you should recive pkg & change user's data
			void recivePkg(void* pkg) override{
				auto pStr = (std::string*) pkg;
				std::cout << *pStr << std::endl;
				//change data.
				_aMap[*pStr]	 = *pStr;
				// 注意,不应该在此处释放资源
			}
			//you should let your use have an unique id.
			std::string uid()override{return m_Uid;}
			//when close you maybe need free the resource.
			void _close()override{std::cout << "user: " << this->uid() << " closed.";}
			//User should approve data-search interface.
			void * _getData(const std::string &type, const std::string &key) override{
				//another way is save the data as a pointer, then you can use getRefData to get value.
				return new std::string(_aMap[key]); 
			}
			//return user's status.
			std::string _statusJson() override{
				return "{\"uid\":\"" + m_Uid + "\", \"process_status\":\"" + currentProcess()->statusJson() +
					"\", \"is_in_dead_lock\":\"" + (isInDeadLock() ? "true":"false")+ "\"}";
			}
			//get all of the data for analysis.
			void _logData() override{
				for(auto it : _aMap){
					std::cout << it.first << " : " << it.second <<std::endl;
				}
			}
			void freePkg(void *pkg) override{
				delete (std::string*)pkg;
			}
		private:
			std::string m_Uid;
			std::map<std::string, std::string> _aMap;


	};
}


#endif /* end of include guard: MYUSER_H_431TWJH2 */
