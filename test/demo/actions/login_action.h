#ifndef LOGIN_ACTION_H_01IKLOHN
#define LOGIN_ACTION_H_01IKLOHN
#include "smdtest/action.h"
#include <iostream>
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
		std::string desc() override{
			return "Demo Login Action";
		}
		private:
		void finishDo() override{
			using namespace std;
			cout << "Action<" << this->name() << "> time use = "  << useTime() << endl;
		}
	);
}

#endif /* end of include guard: LOGIN_ACTION_H_01IKLOHN */
