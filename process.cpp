#include "process.h"
#include "smncpp/lockm.h"

namespace smdtest{
	void Process::Do(User& usr){
		smnet::SMLockMgr _(this->_tsafe);
		bool lastFinish = false;

		do{
			lastFinish = false;
			if(finish()){
				return;
			}
			auto& cur = current();
			if(cur.error().size() != 0){
				_err = cur.error();
				_ptr = _busis.size();
			}else if(cur.finish()){
				++_ptr;
				lastFinish = true;
			}else{
				cur.Do(usr);
			}
		}while(lastFinish);
	}

	void Process::Recive(User& usr, void* pkg){
		smnet::SMLockMgr _(this->_tsafe);
		if(finish()){
			return;
		}

		auto& cur = current();
		cur.Recive(usr, pkg);
	}

	std::string Process::statusJson(){
		smnet::SMLockMgr _(this->_tsafe);
		if(finish()){
			return "{\"process\":\"" + name() + "\", \"status\":\"Finish\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":{}}, \"error\":\"" + _err + "\"}";
		}

		return "{\"process\":\"" + name() + "\", \"status\":\"Running\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":" + current().statusJson() + "}";
	}
}
