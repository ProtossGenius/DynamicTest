#ifndef BUSINESS_H_P1BFXYDW
#define BUSINESS_H_P1BFXYDW
#include <vector>
#include <mutex>
#include "action.h"

namespace smdtest{
	class User;
	class Business{
		public:
			void Do(User& usr);
			void Recive(User& usr, void* pkg);
			std::string statusJson();
			std::string name(){return _name;}
			std::string error(){return _err;}
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
			std::string _name;
			std::mutex _tsafe;
	};
}

#endif /* end of include guard: BUSINESS_H_P1BFXYDW */
