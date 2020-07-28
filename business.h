#ifndef BUSINESS_H_P1BFXYDW
#define BUSINESS_H_P1BFXYDW
#include <vector>
#include <mutex>
#include "action.h"
#include "smncpp/lockm.h"
#include <iostream>

namespace smdtest{
	class User;
	class Business{
		public:
			void Do(User& usr){
				smnet::SMLockMgr _(this->_tsafe);
/*start: if current call is success, should try call next Action.
 * if use this->Do(usr); may cause dead-lock.*/
start:
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
						//set finish
						_ptr = _acts.size();
						break;
					case ActionStatus::Success:
						++_ptr;
						goto start;
						break;
				}
			}
			void Recive(User& usr, void* pkg){
				if(finish()){
					return;
				}

				auto& cur = current();
				cur.Recive(usr, pkg);
			}

			bool finish(){
				return _ptr == _acts.size();
			}
		private:
				Action& current(){
					return this->_acts[_ptr];
				}
		private:
			std::vector<Action> _acts;
			size_t _ptr;
			std::string _err;
			std::mutex _tsafe;
	};
}

#endif /* end of include guard: BUSINESS_H_P1BFXYDW */
