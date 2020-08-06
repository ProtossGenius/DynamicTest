#include "my_strategy.h"
#include "smdtest/user.h"
#include <iostream>
namespace demo{
	MyStrategy::MyStrategy():_ticker(smnet::GetTickManager().getTicker(200)), _login(std::make_shared<smdtest::Process>("LoginProcess")){
			smdtest::Business bus(std::vector<std::string>{smdtest::LoginAction::Name()}, "LoginBusiness", 1);
			_login->getBussnessVector().emplace_back(bus);
		}
std::shared_ptr<smdtest::Process> MyStrategy::getProcess(smdtest::User &usr, const std::string &err) {
			//std::cout << "create process for :" << usr.uid() <<std::endl;
			auto& str = *smdtest::getSharedData<std::string>(usr, "", "Success");
//			std::cout << "?Success = " << str << "|" <<(str != "") <<std::endl;
			if (str != ""){
				usr.close();
			}
			_login->clean();
			return _login;
		}
}