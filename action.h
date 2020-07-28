#ifndef ACTION_H_5KX1OQZA
#define ACTION_H_5KX1OQZA

namespace smdtest{
	class User;
	enum ActionStatus{
		Ready,
		Doing,
		Succsess,
		Fail 
	};

	class Action{
		public:
			virtual ~Action(){}
			virtual void Do(User& usr) = 0;
			virtual bool Filter(void* pkg) = 0;
			virtual void Recive(User& usr, void* pkg) = 0;
			ActionStatus getStatus(){return _status;}
		protected:
			void setStatus(ActionStatus status){this->_status = status;}
		private:
			ActionStatus _status;
	};
}

#endif /* end of include guard: ACTION_H_5KX1OQZA */
