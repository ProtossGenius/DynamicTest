#ifndef STRATEGY_H_U4JARKYK
#define STRATEGY_H_U4JARKYK
#include <memory>
#include "process.h"

namespace smdtest{
	class Strategy{

		public:
			virtual std::shared_ptr<Process> getProcess() = 0;
			virtual int interval_ms() = 0;
	};
}

#endif /* end of include guard: STRATEGY_H_U4JARKYK */
