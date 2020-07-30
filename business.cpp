#include "business.h"
#include "sstream"
#include "smncpp/lockm.h"
namespace smdtest{
	void Business::Do(User& usr){
		smnet::SMLockMgr _(this->_tsafe);
		bool lastFinish = false;

		do {
			lastFinish = false;
			if(loopEnd()){

				if(_err.size() != 0){
					this->_count = _MAX_COUNT;
					return;
				}

				if (this->_count != -1){
					++this->_count;
				} 

			    if(this->_count == -1 || this->_count < this->_MAX_COUNT){
					this->_ptr = 0;
					continue;
				}

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
		current().Recive(usr, pkg);
	}

	void Business::Disconnect(User& usr, const std::string& cName){
		smnet::SMLockMgr _(this->_tsafe);
		current().Disconnect(usr, cName);
	}
	
	std::string Business::statusJson(){
		smnet::SMLockMgr _(this->_tsafe);
		return "{\"business\":\"" + name() + "\",\"error\":\"" + error() + "\", \"size\":" + std::to_string(_acts.size()) +
			", \"index\":" + std::to_string(_ptr) + ", \"current\":" + current().statusJson() +
		   ", \"LOOP_COUNT\":" + std::to_string(_MAX_COUNT )+ ", \"cur_count\":" + std::to_string(_count)+ "}";
	}

}
