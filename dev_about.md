# 开发相关
1. 不能在Action中使用任何版本的thread::sleep()在异步调用中使用thread::sleep必然会导致死锁，就算是不会导致死锁的Sleep版本（例如smncpp/ticker.h中提供的sleep）也不应该使用，
Action中的方法不应该因为任何因素而阻塞。在Action或者Strategy中使用user的XXXWithLock函数同样会引发死锁。
2. User::close()方法只应该通过Strategy::getProcess()调用Strategy::closeUser(usr)来关闭，只有这样才能够保证它是安全的关闭而且只被关闭一次。因为这个缘故，User::close()方法
被设置为私有的。
3. 可以通过多线程运行`io_service.run()`来提升性能。
4. 第一次Action::Do之后，如果你没有修改Action的状态（Action的状态仍然是ActionStatus::Ready)，那么business会自动将状态改变为ActionStatus::WaitResult。如果你需要你的
Action::Do在得到结果前一直调用（如游戏中的战斗），那么你需要在Action::Do中将状态置为ActionStatus::Going;
5. User没有提供无锁版本的setStrategy方法，原因是Action中不应该修改User的Strategy，而Strategy可以很轻易的改变自身的行为，无论是用impl手法还是使用策略模式，它没有必要
通过改变User的当前的Strategy来更改User的行为策略。所有的WithLock函数都是提供给开发者在其他线程中使用的。
6. User现在提供了模板成员方法以直接选择使用shared\_ptr或者引用来管理资源，当你的自定义User是使用shared\_ptr来管理资源的时候，建议是返回一个通过new创建的shared\_ptr的副本
（以尽可能减少拷贝的花费），通过getSharedData方法获得值之后通过强制类型转换转换成原有的shared\_ptr，下面会包含示例代码。smn-cpp或许会针对这种情况特别设计一种计数型的智能指针。
7. 现在Action增加了虚方法Wait，在进入了ActionStatus::WaitResult之后，每次检测帧都会调用Action::Wait，超时相关的检测代码应该放到这里。
8. 只应该在User::freePkg里面释放数据包
```
//在自定义的User中
void *_getData(type, key){
	shared_ptr<Type> it = getYourData(type, key);
	return new shared_ptr<Type>(it);//这样的拷贝和资源成本非常低。
}

//在使用的时候
void anything(User& usr){
	auto ptr = usr.getSharedData<shared_ptr<Type> >(...);
	auto val = *ptr;//直接auto val = *usr.getSharedData...是不行的，或许会导致野指针。
}
```
