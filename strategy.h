#ifndef STRATEGY_H_U4JARKYK
#define STRATEGY_H_U4JARKYK
#include <memory>
#include "process.h"
#include "smncpp/ticker.h"
namespace smdtest{
	class Strategy{
		public:
			virtual std::shared_ptr<Process> getProcess(const std::string& err) = 0;
			virtual std::shared_ptr<smnet::Ticker> getTicker() = 0;
	};
}

#endif /* end of include guard: STRATEGY_H_U4JARKYK */
