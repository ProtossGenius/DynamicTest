# DTest提供的可能性

DTest设计为便于组合的，[术语表](./docs/Glossary.md)中包含对粒度的描述，[行为文档](./docs/DynamicTestDo.md)中描述了DTest如何运行。
首先是关于组合的介绍，Action不保存任何数据，通过ActionManager单件可以通过Action::Name()获得一个Action指针（如果不存在该名称的Action会返回一个
注册名为NullAction的Creator生成的Action指针，NullAction有一个预定义版本，但你依然可以通过重新调用ActionManager::Reg来覆盖之前的定义）
，更高层次的Business和Process则只是对其下层结构的简单组合。这意味着即使在只使用C++作为开发语言的情况下，开发者也可以轻松的通过配置任意组合任何已完成的Action。

Strategy类的设计是希望在此之上更进一步，最简单的策略是重复执行单个Process，这也是压力测试的基本策略，只要令Strategy::getProcess(User& usr, const string& error)
方法总是返回某个相同的Process就可以实现这个策略（实际情况可能需要判断是否需要先运行一次登陆Process而后再不断返回压测Process，不过我想这并不难）

Strategy在为User分配Process的时候会得到User信息，这意味着你可以通过User身上的数据（User提供了查询接口）或者自定义（如前文所说的统计模块）的统计数据来更加智能
的设计策略——可以让它们更具随机性，更接近真实玩家，也更容易击中静态测试不易击中的bug（或许，大概，希望如此……这也是Dynamic的来源）。

User::getData提供了一个非常灵活的获得User数据的接口，这意味着你可以自己的实现版中轻易的复用服务器代码。User在退出的时候必然会执行的\_logData和\_close()方法可以让
你决定在测试结束后与服务器记录的数据进行比较，我想这样可以避免大部分由于服务器发包失误而导致的显示错误。始终建议\_logData能够持久的记录下User的状态，这会有助于
回归测试。

Process的设计目的在于流程的检测——用于回归测试，最基础的就是Action根据服务器的回包来决定当前的状态是正确还是错误，当然还有刚刚说过的通过\_logData与数据库中的数据进行对比。

如果你是在后端的开发过程中使用这个框架，那么它可以帮助你解除后端与前端的绑定，你可以随时测试服务器的响应是否符合预期而不必等待客户端完成他们的工作，顺便的——你很自然的就
完成了一个测试用例。

