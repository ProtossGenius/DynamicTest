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
	
	void strReplaceAll(std::string& str, const std::string& src, const std::string& dest){
		using std::string;
		string::size_type pos = 0, srcLen = src.size(), destLen = dest.size();
		pos = str.find(src, pos);
		while(pos != string::npos){
			str.replace(pos, srcLen, dest);
			pos = str.find(src, pos + destLen);
		}
	}

	std::string Action::statusJson(){
		std::string err = this->_err;
		strReplaceAll(err, "\\", "\\\\");
		strReplaceAll(err, "\"", "\\\"");
		return "{\"name\":\"" + name() +"\",\"status\":\"" + to_string(getStatus()) + "\", \"error\":\"" + err + "\"}";
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
