#include "user.h"
#include <boost/bind/bind.hpp>
namespace smdtest{
	typedef smnet::SMLockMgr lockm;
	void User::start(){
		this->_ioc.post(boost::bind(&User::doAction, this));
		this->_ioc.post(boost::bind(&User::doRecv, this));
	}

	void User::doAction(){
		while(this->_alive){
			{
				lockm _(this->_tsafe);
				_process->Do(*this);
			}
			auto cur = currentStrategy();
			auto ticker = cur->getTicker();
			ticker->tick();//wait some time.
		}
	}

	void User::doRecv(){
		while(this->_alive){
			{
				lockm _(this->_tsafe);
				if (!this->_recvChan.empty()){
					this->_recvChan.one_thread_get();
				}else{
					this->_pkg = this->recv_once();
				}

				this->Recive(this->_pkg);
			}
		}
	}

	void User::Recive(void* pkg){
		this->recivePkg(pkg);
		this->_process->Recive(*this, pkg);
	}
}
