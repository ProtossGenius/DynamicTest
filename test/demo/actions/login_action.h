#ifndef LOGIN_ACTION_H_01IKLOHN
#define LOGIN_ACTION_H_01IKLOHN
#include "smdtest/action.h"

namespace smdtest{
	dtaction(LoginAction,
		public:

		//send a sync message to user's chan.
		void Do(User &usr) override;
		void Disconnect(User &usr, const std::string &name) override{}
		void Wait(User &usr) override{}
		bool Filter(void *pkg) override{
			return  true;
		}
		void Recive(User &usr, void *pkg) override;
		std::string statusJson() override{
			return "{\"name\":\"" + name() +"\",\"status\":\"" + to_string(getStatus()) + "\"}";
		}
		std::string desc() override{
			return "Demo Login Action";
		}
	);
}

#endif /* end of include guard: LOGIN_ACTION_H_01IKLOHN */
