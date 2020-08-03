#include "user.h"
#include <boost/bind/bind.hpp>
namespace smdtest{
	typedef smnet::SMLockMgr lockm;
	void User::start(){
		this->_ioc.post(boost::bind(&User::doAction, this));
		this->_ioc.post(boost::bind(&User::dealPkg, this));
	}

	void User::doAction(){
		while(this->_alive){
			{
				lockm _(this->_tsafe);
				_process->Do(*this);
				if (this->_process->finish()){
					this->_process = this->_strategy->getProcess();
				}
			}
			auto ticker = this->_strategy->getTicker();
			ticker->tick();//wait some time.
		}
	}

	void User::dealPkg(){
		while(this->_alive){
			this->_recvChan.one_thread_get();//see User::User it's result save in _pkg;
			{
				lockm _(this->_tsafe);
				this->Recive(this->_pkg);
			}
		}
	}

	void User::Recive(void* pkg){
		this->_process->Recive(*this, pkg);
		this->recivePkg(pkg);
	}
}
