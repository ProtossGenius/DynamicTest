#ifndef PROCESS_H_YXGUL8IP
#define PROCESS_H_YXGUL8IP
#include "business.h"
namespace smdtest{
	class User;
	class Process{
		public:
			Process(const std::string& name):_ptr(0), _name(name){}
			Process(std::vector<Business>&& busis, const std::string& name):_ptr(0), _name(name), _busis(std::move(busis)) {}
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
			void clean();
		public:	//unsafe
			std::vector<Business>& getBussnessVector(){return this->_busis;}
		private:
			Business& current(){return _busis[_ptr];}
		private:
			size_t _ptr;
			std::string _name;
			std::string _err;

			std::vector<Business> _busis;
	};
}

#endif /* end of include guard: PROCESS_H_YXGUL8IP */
