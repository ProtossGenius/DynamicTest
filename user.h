#ifndef USER_H_UV8CBAVZ
#define USER_H_UV8CBAVZ

#include <mutex>
namespace smdtest{
	class User{


		private:
			std::mutex _tsafe;
	};
}

#endif /* end of include guard: USER_H_UV8CBAVZ */
