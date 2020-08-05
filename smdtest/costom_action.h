#ifndef COSTOM_ACTION_H_SPWETCAC
#define COSTOM_ACTION_H_SPWETCAC
#include "action.h"

namespace smdtest{

	typedef std::function<void(User&)> ActionFuncDo;	
	typedef std::function<bool(void*)> ActionFuncFilter;
	typedef std::function<void(User&, void*)> ActionFuncRecive;
	typedef std::function<void(User&, const std::string& name)> ActionFuncDisconnect;
	typedef std::function<ActionStatus()> ActionFuncGetStatus;
	typedef std::function<std::string()> GetString;
	typedef GetString ActionFuncStatusJson;
	typedef GetString ActionFuncName;
	typedef GetString ActionFuncDesc;
	typedef GetString ActionFuncError;
//only should use here.
#define SMCASetter(type, vName, fName)\
		public: \
			void fName(type val){smnet::SMLockMgr _(_tsafe);vName = val;} \
		private: \
			type vName;

	class CustomAction: public Action{
		public:
			CustomAction() {}
			virtual ~CustomAction() = 0;
		public:
			void Do(User& usr) override	{smnet::SMLockMgr _(_tsafe); _do(usr);}
			bool Filter(void* pkg) override {smnet::SMLockMgr _(_tsafe);return _filter(pkg);}
			void Recive(User& usr, void* pkg) override {smnet::SMLockMgr _(_tsafe);_recive(usr, pkg);}
			void Disconnect(User& usr, const std::string& name) override {smnet::SMLockMgr _(_tsafe);_disc(usr, name);}
			ActionStatus getStatus() override {smnet::SMLockMgr _(_tsafe);return _getStatus(); }
			std::string statusJson() override{smnet::SMLockMgr _(_tsafe);return _statusJson();}
			std::string name() override {smnet::SMLockMgr _(_tsafe);return _name();}
			std::string desc() override {smnet::SMLockMgr _(_tsafe);return _desc();}
			std::string error() override {smnet::SMLockMgr _(_tsafe);return _err();}
		private:
		SMCASetter(ActionFuncDo			,_do			,setFuncDo)
		SMCASetter(ActionFuncFilter 	,_filter		,setFuncFilter)
		SMCASetter(ActionFuncRecive		,_recive		,setFuncRecive)
		SMCASetter(ActionFuncDisconnect	,_disc			,setFuncDisc)
		SMCASetter(ActionFuncGetStatus 	,_getStatus		,setFuncGetStatus)
		SMCASetter(ActionFuncStatusJson	,_statusJson 	,setFuncStatusJson)
		SMCASetter(ActionFuncName 		,_name 			,setFuncName)
		SMCASetter(ActionFuncDesc 		,_desc 			,setFuncDesc)
		SMCASetter(ActionFuncError 		,_err 			,setFuncErr)
		private:
		std::mutex _tsafe;
	};
}


#endif /* end of include guard: COSTOM_ACTION_H_SPWETCAC */
