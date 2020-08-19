#include "process.h"
#include "smncpp/lockm.h"
namespace smdtest{
	void Process::Do(User& usr){
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
				cur.clean();
			}else if(cur.finish()){
				++_ptr;
				lastFinish = true;
				cur.clean();
			}else{
				cur.Do(usr);
			}
		}while(lastFinish);
	}

	void Process::clean(){
		_ptr = 0;
		_err = "";
		for(auto& it : this->_busis){
			it.clean();
		}
	}

	void Process::Recive(User& usr, void* pkg){
		if(finish()){
			return;
		}

		current().Recive(usr, pkg);
	}
	
	void Process::Disconnect(User& usr, const std::string& cName){
		if(finish()){
			return;
		}

		current().Disconnect(usr, cName);
	}

	std::string Process::statusJson(){
		if(finish()){
			return "{\"process\":\"" + name() + "\", \"status\":\"Finish\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":{}}, \"error\":\"" + _err + "\"}";
		}

		return "{\"process\":\"" + name() + "\", \"status\":\"Running\", \"index\":\"" + std::to_string(_ptr) + "\" \"current\":" + current().statusJson() + "}";
	}
}
