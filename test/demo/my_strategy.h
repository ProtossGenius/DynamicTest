#ifndef MY_STRATEGY_H_BP5EG74U
#define MY_STRATEGY_H_BP5EG74U
#include "smdtest/strategy.h"
#include "smdtest/business.h"
#include "actions.h"
namespace demo{
	class MyStrategy:public smdtest::Strategy{
	public:
		MyStrategy();
	public:
		std::shared_ptr<smdtest::Process> firstProcess() override{
			return _login;
		}
		std::shared_ptr<smdtest::Process> getProcess(smdtest::User &usr, const std::string &err) override;
		std::shared_ptr<smnet::Ticker> getTicker() override{
			return  _ticker;
		}

	private:
		std::shared_ptr<smnet::Ticker> _ticker;
		std::shared_ptr<smdtest::Process> _login;
	};
}


#endif /* end of include guard: MY_STRATEGY_H_BP5EG74U */
