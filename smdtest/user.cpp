#include "user.h"
#include <boost/bind/bind.hpp>
#include <iostream>
namespace smdtest{
	typedef smnet::SMLockMgr lockm;
	void User::start(){
		this->_ioc.post(boost::bind(&User::doAction, this));
	}
	
	void User::onDisconnect(const std::string& sId){
		lockm _(this->_tsafe);
		this->_process->Disconnect(*this, sId);
	}

	void User::doAction(){
		this->_deadLock = false;
		{
			lockm _(this->_tsafe);
			_process->Do(*this);
			if (this->_process->finish()){
				this->_process = this->_strategy->getProcess(*this, this->_process->error());
			}
		}
		auto ticker = this->_strategy->getTicker();
		ticker->tick();//wait some time.
		if (this->_alive){
			this->_ioc.post(boost::bind(&User::doAction, this));
		}else{
			this->logData();
		}
	}
	void User::push(void*pkg){
		this->_recvChan.push(pkg);
		this->_ioc.post(boost::bind(&User::dealPkg, this));
	}
	void User::dealPkg(){
		this->_deadLock = false;
		this->_recvChan.one_thread_get();//see User::User it's result save in _pkg;
		{
			lockm _(this->_tsafe);
			this->Recive(this->_pkg);
		}
	}

	void User::Recive(void* pkg){
		this->_process->Recive(*this, pkg);
		this->recivePkg(pkg);
	}
}
