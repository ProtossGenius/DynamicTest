#include "user.h"
#include <boost/bind/bind.hpp>
#include "smncpp/lockm.h"
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
				this->action_once();
			}
			//TODO: use smnet::Ticker to wait some time.
		}
	}

	void User::doRecv(){
		while(this->_alive){
			{
				lockm _(this->_tsafe);
				void* pkg = this->recv_once();
				this->_process->Recive(*this, pkg);
			}
		}
	}
}
