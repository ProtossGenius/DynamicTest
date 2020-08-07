#include "strategy.h"
#include "user.h"
namespace smdtest{
	void Strategy::closeUser(User& usr){
		usr.close();
	}
}
