#include "action.h"
#include "smncpp/assert.h"
namespace smdtest{

	void Action::setStatus(ActionStatus status){
		if(actionFinish(status)){
			_endTime = clock();
			finishDo();
		}
		if(status == ActionStatus::Ready){
			_startTime = clock();
		}
		this->_status = status;
	}

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
		_assert(false);
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

	const std::string NameNullAction(NullAction::Name());
	const std::string NameFinishAction(NullAction::Name());
}
