# DynamicTest

* 描述

C++动态测试。一种黑盒测试，作为经典单元测试的补充，特别针对网络服务器系统，关于类的含义请查看docs目录下的[术语表](./docs/Glossary.md)，这个表格很短，
所以建议在看下文之前先去喵一眼。(go.mod是为了写一些[工具](#about_tools))
DTest设计的思路是使用Action来模拟用户的基本操作（对于服务器系统来说就是发包）并且Action根据服务器回包来判断这项操作是否成功。由于User拥有唯
一ID（User::uid）接口，所以应该据此设计统计模块（而不应该在Action中修改User数据），User数据的只应该依据服务器的回包修改。

* DTest提供的可能性

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

* 数据的统计

数据的统计有两种思路，像是排队时间、服务器响应时间这样的统计数据即可以通过自定义的统计模块来统计也可以通过输出日志文件的方式留下数据，推荐是两种兼用——前者可以不必担
心数据的丢失，后者可以在运行时实时查看数据。

* 关于环境配置

Dtest的核心代码在[./smdtest]中，第三方库包括boost（的异步）和smn-cpp（使用了其Ticker、LockManager等）.
demo代码位于./test/demo，它实现了一个简单的策略——直到在Action::Do返回Success前一直返回login\_action，返回Success后关闭User。DTest基础的用法基本上在里面都有展现。

   * Windows

这个项目是在linux下用vim开发的，没有用经典的cmake来管理项目，这意味着很难一键在windows下搭建，下面会提供手动配置的过程。
boost的配置请参考网络。smn-cpp的配置
1. 通过`git clone https://github.com/ProtossGenius/smn-cpp`下载代码
2. 将smn-cpp/smncpp添加到VisutalStudio的include配置中。
3. 编译运行

   * Linux

如果不希望安装go和smake的话可以参考上述步骤手动配置运行，通过软链接确保编译通过即可。
在已经安装了go（最好配置一下goproxy以防止某些库下载不下来）的情况下直接运行make init即可。

在这里简短的介绍一下smake，因为demo代码中也会用到。它会分析目录树并且生成Makefile来确保所有的cpp都会被编译，并且在某些文件有修改的情况下能够（尽可能）最小程度的
重新编译。同时它也会读取目录下的sls.json文件，分析需要用到的第三方库（目前仅支持git{type:g}和本地目录{type:l}）并且创建软连接。而后依次分别在目标目录和sls.json所
在目录运行cmd\_target，cmd\_local。smake的获得方式是：
```
go get -u github.com/ProtossGenius/smntools/cmd/smake
```
* 开发相关
1. 不能在Action中使用任何版本的thread::sleep()在异步调用中使用thread::sleep必然会导致死锁，就算是不会导致死锁的Sleep版本（例如smncpp/ticker.h中提供的sleep）也不应该使用，
Action中的方法不应该因为任何因素而阻塞。在Action或者Strategy中使用user的XXXWithLock函数同样会引发死锁。
2. User::close()方法只应该通过Strategy::getProcess()调用Strategy::closeUser(usr)来关闭，只有这样才能够保证它是安全的关闭而且只被关闭一次。因为这个缘故，User::close()方法
被设置为私有的。
3. 可以通过多线程运行`io_service.run()`来提升性能。
4. 第一次Action::Do之后，如果你没有修改Action的状态（Action的状态仍然是ActionStatus::Ready)，那么business会自动将状态改变为ActionStatus::WaitResult。如果你需要你的
Action::Do在得到结果前一直调用（如游戏中的战斗），那么你需要在Action::Do中将状态置为ActionStatus::Going;
5. User没有提供无锁版本的setStrategy方法，原因是Action中不应该修改User的Strategy，而Strategy可以很轻易的改变自身的行为，无论是用impl手法还是使用策略模式，它没有必要
通过改变User的当前的Strategy来更改User的行为策略。所有的WithLock函数都是提供给开发者在其他线程中使用的。
6. 总是应该通过User的friend模板方法来获得User的Data，由于User::getData返回的是一个void\*，它太容易导致内存泄漏了，不论是忘记删除还是重复删除。（因此getData和getDataWithLock
也被设置为私有的）

<h2 id="about_tools"> 工具</h2>
1. cmd/sdmtcact 
一个工具用于统计目标目录下的Action，并通过自动代码生成实现自动注册（注意，Action必须在smdtest命名空间，且必须要使用dtaction宏，dtaction的使用
可以参考[DEMO](test/demo/actions/login_action.h)

