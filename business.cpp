#include "business.h"
#include "sstream"
#include "smncpp/lockm.h"
namespace smdtest{
	void Business::Do(User& usr){
		smnet::SMLockMgr _(this->_tsafe);
		bool lastFinish = false;

		do {
			lastFinish = false;
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
					cur.clean();
					//set finish
					_ptr = _acts.size();
					break;
				case ActionStatus::Success:
					cur.clean();
					++_ptr;
					lastFinish = true;
					break;
			}
		}while(lastFinish);
	}
	
	void Business::Recive(User& usr, void* pkg){
		smnet::SMLockMgr _(this->_tsafe);
		if(finish()){
			return;
		}

		auto& cur = current();
		cur.Recive(usr, pkg);
	}
	
	std::string Business::statusJson(){
		smnet::SMLockMgr _(this->_tsafe);
		if (finish()){
			return "{\"business\":\"" + name() + "\", \"status\":\"Finish\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":{}}, \"error\":\"" + _err + "\"}";
		}

		return "{\"business\":\"" + name() + "\", \"status\":\"Running\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":" + current().statusJson() + "}";
	}

}
