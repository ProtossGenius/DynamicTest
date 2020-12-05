#include "user.h"
#include <boost/bind/bind.hpp>
#include <iostream>
using namespace std;
namespace smdtest{
	typedef smnet::SMLockMgr lockm;
	User::User(boost::asio::io_service& ioc, std::shared_ptr<Strategy> strategy):_ioc(ioc), _worker(new boost::asio::io_service::work(ioc)), _recvChan(200), _strategy(strategy), 
	_alive(true), _deadLock(true), _pkg(nullptr){ 
		_recvChan.setExport(&_pkg);
		this->_process = this->_strategy->firstProcess();
	}

	void User::close(){
		//recv maybe in block if recvChan is empty. should cancel the block and free resourece.
		this->_alive = false;
		this->_strategy->getTicker()->setTickDo([]{});
		delete this->_worker;
		this->_worker = nullptr;
		if (this->_recvChan.empty()){
			this->_recvChan.push(nullptr);
		}
		this->_logData();
		_close();				
	}

	void User::start(){
		this->_strategy->getTicker()->setTickDo([&]{
			this->_ioc.post(boost::bind(&User::doAction, this));
		});
	}
	
	void User::onDisconnectWithLock(const std::string& sId){
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
		this->recivePkg(pkg);
		this->_process->Recive(*this, pkg);
		this->freePkg(pkg);
	}
}
