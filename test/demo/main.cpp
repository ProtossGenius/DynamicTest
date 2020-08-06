#include "MyUser.h"
#include "my_strategy.h"
#include <map>

std::map<std::string, std::shared_ptr<demo::MyUser> > UserManager;

int main(){
	boost::asio::io_service ioc;
	for (int i = 0; i < 100; ++i){
		auto strategy = std::make_shared<demo::MyStrategy> ();
		std::string uid = "uesr" + std::to_string(i);
		auto user = std::make_shared<demo::MyUser>(ioc, strategy, uid);
		std::cout <<"User("<< uid << ").start();"<<std::endl;
		user->start();
		UserManager[uid] = user;
	}
	ioc.run();
}
