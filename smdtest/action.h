#ifndef ACTION_H_5KX1OQZA
#define ACTION_H_5KX1OQZA
#include <functional>
#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <ctime>
#include "smncpp/lockm.h"
namespace smdtest{
	class User;
	enum ActionStatus{
		Ready,
		Doing,
		WaitResult,
		Success,
		Fail 
	};
	
	inline 
	bool actionFinish(ActionStatus as){
		return ActionStatus::Success == as || ActionStatus::Fail == as;
	}

	std::string to_string(ActionStatus as);
	class Action{
		friend class Business;
		public:
			Action():_status(ActionStatus::Ready){}
			virtual ~Action(){}
		public:
			virtual void Do(User& usr) = 0;
			virtual void Wait(User& usr) = 0;
			virtual bool Filter(void* pkg) = 0;
			virtual void Recive(User& usr, void* pkg) = 0;
			virtual void Disconnect(User& usr, const std::string& name) = 0;
			virtual std::string statusJson() ;	
			virtual std::string name() = 0;
			virtual std::string desc() = 0;
			std::string error(){
				return _err;
			}
		public:
			virtual ActionStatus getStatus(){return _status;}
			void clean(){_status = ActionStatus::Ready;_err = ""; _startTime = _endTime = 0;}
		protected:
			void setError(const std::string& err){
				_status = ActionStatus::Fail;
				_err = err;
			}
			void setStatus(ActionStatus status);

			clock_t useTime(){
				return double(_endTime - _startTime) / CLOCKS_PER_SEC;
			}
			virtual void finishDo(){}
		private:
			ActionStatus _status;
			std::string _err;
			clock_t _startTime;
			clock_t _endTime;
	};

	class ActionManager{
		typedef std::map<std::string, std::function<Action*()> > CreaterMap;
		private:
			ActionManager(){}
		public:
			static ActionManager& Instance(){
				static ActionManager mgr;
				return mgr;
			}
		public:
			void Reg(const std::string& name, std::function<Action*()> creater){
				smnet::SMLockMgr _(this->_tsafe);
				_CrtrMap[name] = creater;
			}
			std::shared_ptr<Action> create(const std::string& name);
		public://unsafe
			CreaterMap& getCreaterMap()	{return _CrtrMap;}
		private:
			CreaterMap _CrtrMap;
			std::mutex _tsafe;
	};
	
	class Register{
		public:
			Register(const std::string& name, std::function<Action*()> creater){
				ActionManager::Instance().Reg(name, creater);
			}
	};

#define _STR(str) #str
#define dtaction(action, code) class action :public Action {  \
	public: \
		static Action* New(){return new action();} \
		std::string name() override{return _STR(action);} \
	public:\
		static const std::string Name(){\
			static const Register _reg(_STR(action), action::New);\
			return _STR(action);}\
	code \
}; \

	dtaction(NullAction,  
		public:
			NullAction():Action(){setStatus(ActionStatus::Fail); setError("NullAction");}
		public:
			void Do(User&)override{}
			void Wait(User&) override{}
			std::string desc() override{return "NullAction Desc";}
			bool Filter(void*)override{return false;}
			void Recive(User&, void*)override{}
			void Disconnect(User&, const std::string&)override{}
			std::string statusJson() override{return "{}";}
	);
	extern const std::string NameNullAction;
	dtaction(FinishAction,  
		public:
			FinishAction():Action(){setStatus(ActionStatus::Success); }
		public:
			void Do(User&)override{}
			void Wait(User &usr) override{}
			std::string desc() override{return "NullAction Desc";}
			bool Filter(void*)override{return false;}
			void Recive(User&, void*)override{}
			void Disconnect(User&, const std::string&)override{}
			std::string statusJson() override{return "{}";}
	);
	extern const std::string NameFinishAction;
	inline 
	NullAction& getNullAction(){
		static NullAction null;
		return null;
	}

	inline 
	FinishAction& getFinishAction(){
		static FinishAction finish;
		return finish;
	}
}

#endif /* end of include guard: ACTION_H_5KX1OQZA */
