#ifndef PROCESS_H_YXGUL8IP
#define PROCESS_H_YXGUL8IP
#include "business.h"
namespace smdtest{
	class User;
	class Process{
		public:
			Process(std::vector<Business>&& busis, const std::string& name):_busis(std::move(busis)), _ptr(0), _name(name) {}
		public:
			void Do(User& usr);
			void Recive(User& usr, void *pkg);
			void Disconnect(User& usr, const std::string& cName);
			std::string statusJson();
			std::string name(){return _name;}
			std::string error(){return _err;}
			bool finish(){
				return _ptr == _busis.size();
			}

		private:
			Business& current(){return _busis[_ptr];}
		private:
			std::vector<Business> _busis;
			size_t _ptr;
			std::string _name;
			std::string _err;
			std::mutex _tsafe;
	};
}

#endif /* end of include guard: PROCESS_H_YXGUL8IP */
