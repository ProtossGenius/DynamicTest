#include "business.h"
#include "sstream"
#include "smncpp/lockm.h"
namespace smdtest{
	void Business::Do(User& usr){
		smnet::SMLockMgr _(this->_tsafe);
/*start: if current call is success, should try call next Action.
 * if use this->Do(usr); may cause dead-lock.*/
start:
		if(finish()){
			return;
		}
		auto& cur = current();
		switch(cur.getStatus()){
			case ActionStatus::Ready:
			case ActionStatus::Doing:
				cur.Do(usr);
				break;
			case ActionStatus::WaitResult:
				break;
			case ActionStatus::Fail:
				_err = cur.error();
				//set finish
				_ptr = _acts.size();
				break;
			case ActionStatus::Success:
				++_ptr;
				goto start;
				break;
		}
	}
	
	void Business::Recive(User&usr, void* pkg){
		smnet::SMLockMgr _(this->_tsafe);
		if(finish()){
			return;
		}

		auto& cur = current();
		cur.Recive(usr, pkg);
	}
	
	std::string Business::showStatus(){
		std::stringstream ss;
		//TODO: .
		return ss.str();
	}

}
