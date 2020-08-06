#include "action.h"

namespace smdtest{

	std::string to_string(ActionStatus as){
		switch(as){
			case ActionStatus::Ready:
				return "ActionStatus::Ready";
			case ActionStatus::Doing:
				return "ActionStatus::Doing";
			case ActionStatus::Fail:
				return "ActionStatus::Fail";
			case ActionStatus::Success:
				return "ActionStatus::Success";
			case ActionStatus::WaitResult:
				return "ActionStatus::WaitResult";
		}
		return "Unknown";
	}
	
	std::shared_ptr<Action> ActionManager::create(const std::string& name){
		smnet::SMLockMgr _(this->_tsafe);
		auto iter = this->_CrtrMap.find(name);
		if (iter != this->_CrtrMap.end()){
			return std::shared_ptr<Action>(iter->second());
		}

		return	this->create("NullAction");
	}
}
