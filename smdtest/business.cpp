#include "business.h"
#include "sstream"
#include "smncpp/lockm.h"
#include "action.h"

namespace smdtest{

	Business::Business(std::vector<std::string> acts, std::string name, int maxCount): _acts(acts.size()), _ptr(0), _name(name),_MAX_COUNT(maxCount), _count(0){
		auto& am =  ActionManager::Instance();
		for(size_t i= 0; i < acts.size(); ++i){
			_acts[i] = am.create(acts[i]);
		}
	}

	Action& Business::current(){
		if (onceLoopEnd()){
			return getNullAction();
		}

		return *this->_acts[_ptr];
	}
	
	void Business::clean(){
		_ptr = 0;
		_count = 0;
		_err = "";
		for (auto& it : this->_acts){
			it->clean();
		}
	}

	void Business::Do(User& usr){
		bool lastFinish = false;

		do {
			lastFinish = false;
			if(onceLoopEnd()){
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
					cur.Do(usr);
					if(cur.getStatus() == ActionStatus::Ready){
						cur.setStatus(ActionStatus::WaitResult);
					}
					break;
				case ActionStatus::Doing:
					cur.Do(usr);
					break;
				case ActionStatus::WaitResult:
					cur.Wait(usr);
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
					current().setStatus(ActionStatus::Ready);
					lastFinish = true;
					break;
			}
		}while(lastFinish);
	}
	
	void Business::Recive(User& usr, void* pkg){
		current().Recive(usr, pkg);
	}

	void Business::Disconnect(User& usr, const std::string& cName){
		current().Disconnect(usr, cName);
	}
	
	std::string Business::statusJson(){
		return "{\"business\":\"" + name() + "\",\"error\":\"" + error() + "\", \"size\":" + std::to_string(_acts.size()) +
			", \"index\":" + std::to_string(_ptr) + ", \"current\":" + current().statusJson() +
		   ", \"LOOP_COUNT\":" + std::to_string(_MAX_COUNT )+ ", \"cur_count\":" + std::to_string(_count)+ "}";
	}

}
