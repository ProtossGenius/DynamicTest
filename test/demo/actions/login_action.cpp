#include "login_action.h"
#include "smdtest/user.h"
#include "smncpp/ticker.h"
#include <random>
#include <iostream>
#include <ctime>
namespace smdtest{
	std::string getTime()
	{
	    time_t timep;
	    time (&timep);
	    char tmp[64];
	    strftime(tmp, sizeof(tmp), "[%Y-%m-%d %H:%M:%S]",localtime(&timep) );
	    return tmp;
	}

	void LoginAction::Do(User& usr){
		std::cout <<getTime() << "LoginAction::Do user: "  << usr.uid() <<std::endl;
		static std::default_random_engine random;
		static std::uniform_int_distribution<int> dis1(0, 100);
		auto& ioc = usr.getIoSvc();
		setStatus(ActionStatus::WaitResult);
		ioc.post([&]{
			std::string *result = nullptr;
			if (dis1(random) > 70){
				result = new std::string("Fail");
			}else{
				result = new std::string("Success");
			}
			usr.push(result);
			//should never free resource here, it's sync.
		});
	}

	void LoginAction::Recive(User& usr, void*pkg){
		std::cout <<getTime() << "LoginAction::Recive user: " << usr.uid() <<std::endl;
		auto& str = *(std::string*)pkg;
		//should not free resource here too.
		if (str == "Success"){
			this->setStatus(ActionStatus::Success);
			return;
		}
		this->setError(str);
	}
}
