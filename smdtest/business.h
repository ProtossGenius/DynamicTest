#ifndef BUSINESS_H_P1BFXYDW
#define BUSINESS_H_P1BFXYDW
#include <vector>
#include <mutex>
#include <memory>
#include "action.h"

namespace smdtest{
	class User;
	class Business{
		public:
			Business(std::vector<std::string> acts, std::string name, int maxCount): _acts(acts.size()), _ptr(0), _name(name),_MAX_COUNT(maxCount), _count(0){
				auto& am =  ActionManager::Instance();
				for(size_t i= 0; i < acts.size(); ++i){
					_acts[i] = am.create(acts[i]);
				}
			}
		public:
			void Do(User& usr);
			void Recive(User& usr, void* pkg);
			void Disconnect(User& usr, const std::string& cName);
			std::string statusJson();
			std::string name(){return _name;}
			std::string error(){return _err;}
			
			bool finish(){
				return loopEnd() && _count == _MAX_COUNT;
			}
			void clean(){
				_ptr = 0;
				_count = 0;
				_err = "";
			}
		private:
			bool loopEnd(){
				return _ptr == _acts.size();
			}
			Action& current(){
				if (loopEnd()){
					return getNullAction();
				}

				return *this->_acts[_ptr];
			}
		private:
			std::vector<std::shared_ptr<Action> > _acts;
			size_t _ptr;
			std::string _err;
			std::string _name;
			const int _MAX_COUNT;
			int _count;
	};
}

#endif /* end of include guard: BUSINESS_H_P1BFXYDW */
