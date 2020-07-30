#ifndef ACTION_H_5KX1OQZA
#define ACTION_H_5KX1OQZA
#include <string>
namespace smdtest{
	class User;
	enum ActionStatus{
		Ready,
		Doing,
		WaitResult,
		Success,
		Fail 
	};
	
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
			ActionStatus getStatus(){return _status;}
			virtual std::string name() = 0;
			virtual std::string error() = 0;
			virtual void clean() = 0;
		protected:
			void setStatus(ActionStatus status){this->_status = status;}
		private:
			ActionStatus _status;
	};

#define dtaction(action, code) class action :public Action {  \
	public: \
		std::string name() override{return #action;}\
	private: \
	code \
};

	dtaction(NullAction, 
		public:
			NullAction(){setStatus(ActionStatus::Fail); }
		public:
		void Do(User&)override{}
		bool Filter(void*)override{return false;}
		void Recive(User&, void*)override{}
		void Disconnect(User&, const std::string&)override{}
		std::string statusJson() override{return "{}";}
		std::string error()override{
			return "NullAction";
		}
		void clean()override;		
	);

	NullAction& getNullAction(){
		static NullAction null;
		return null;
	}
}

#endif /* end of include guard: ACTION_H_5KX1OQZA */
