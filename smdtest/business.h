#ifndef BUSINESS_H_P1BFXYDW
#define BUSINESS_H_P1BFXYDW
#include <vector>
#include <mutex>
#include <memory>

namespace smdtest{
	class User;
	class Action;
	class Business{
		public:
			Business(std::vector<std::string> acts, std::string name, int maxCount = 1);
		public:
			void Do(User& usr);
			void Recive(User& usr, void* pkg);
			void Disconnect(User& usr, const std::string& cName);
			std::string statusJson();
			std::string name(){return _name;}
			std::string error(){return _err;}
			
			bool finish(){
				return onceLoopEnd() && _count == _MAX_COUNT;
			}
			void clean();
		private:
			bool onceLoopEnd(){
				return _ptr == _acts.size();
			}
			Action& current();
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
