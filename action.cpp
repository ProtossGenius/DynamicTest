#include "action.h"

namespace smdtest{

	std::shared_ptr<Action> ActionManager::create(const std::string& name){
		smnet::SMLockMgr _(this->_tsafe);
		auto iter = this->_CrtrMap.find(name);
		if (iter != this->_CrtrMap.end()){
			return std::shared_ptr<Action>(iter->second());
		}

		return	this->create("NullAction");
	}
}
