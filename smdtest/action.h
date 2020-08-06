#ifndef ACTION_H_5KX1OQZA
#define ACTION_H_5KX1OQZA
#include <functional>
#include <map>
#include <mutex>
#include <memory>
#include <string>

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

	std::string to_string(ActionStatus as);
	
	class Action{
		public:
			Action():_status(ActionStatus::Ready){}
			virtual ~Action(){}
		public:
			virtual void Do(User& usr) = 0;
			virtual bool Filter(void* pkg) = 0;
			virtual void Recive(User& usr, void* pkg) = 0;
			virtual void Disconnect(User& usr, const std::string& name) = 0;
			virtual std::string statusJson() = 0;
			virtual std::string name() = 0;
			virtual std::string desc() = 0;
			std::string error(){
				return _err;
			}
		public:
			ActionStatus getStatus(){return _status;}
			void clean(){_status = ActionStatus::Ready;_err = "";}
		protected:
			void setError(const std::string& err){
				_status = ActionStatus::Fail;
				_err = err;
			}
			void setStatus(ActionStatus status){
				this->_status = status;
			}
		private:
			ActionStatus _status;
			std::string _err;
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
			std::string desc(){return "NullAction Desc";}
			bool Filter(void*)override{return false;}
			void Recive(User&, void*)override{}
			void Disconnect(User&, const std::string&)override{}
			std::string statusJson() override{return "{}";}
	);

	inline 
	NullAction& getNullAction(){
		static NullAction null;
		return null;
	}
}

#endif /* end of include guard: ACTION_H_5KX1OQZA */