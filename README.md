# DynamicTest

* 描述

动态测试。一种黑盒测试，作为经典单元测试的补充，特别针对网络服务器系统，关于类的含义请查看docs目录下的[术语表](./docs/Glossary.md)，这个表格很短，
所以建议在看下文之前先去喵一眼。
DTest设计的思路是使用Action来模拟用户的基本操作（对于服务器系统来说就是发包）并且Action根据服务器回包来判断这项操作是否成功。由于User拥有唯
一ID（User::uid）接口，所以应该据此设计统计模块（而不应该在Action中修改User数据），User数据的只应该依据服务器的回包修改。

* DTest提供的可能性

DTest设计为便于组合的，[术语表](./docs/Glossary.md)中包含对粒度的描述，[行为文档](./docs/DynamicTestDo.md)中描述了DTest如何运行。
首先是关于组合的介绍，Action不需要保存任何数据或者状态，通过ActionManager单件可以通过Action::name()获得一个Action指针（如果不存在该名称的Action会返回一个
注册名为NullAction的Creator生成的Action指针，NullAction有一个预定义版本，但使用者依然可以通过重新调用ActionManager::Reg来覆盖之前的定义）
，更高层次的Business和Process则只是对其下层结构的简单组合。这意味着即使在只使用C++作为开发语言的情况下，开发者也可以轻松的通过配置任意组合任何已完成的Action。


Strategy类的设计是希望在此之上更进一步，最简单的策略是重复执行单个Process，这也是压力测试的基本策略，只要令Strategy::getProcess(User& usr, const string& error)
方法总是返回某个相同的Process就可以实现这个策略（实际情况可能需要判断是否需要先运行一次登陆Process而后再不断返回压测Process，不过我想这并不难）

Strategy在为User分配Process的时候会得到User信息，这意味着你可以通过User身上的数据（User提供了查询接口）或者自定义（如前文所说的统计模块）的统计数据来更加智能
的设计策略——可以让它们更具随机性，更接近真实玩家，也更容易击中静态测试不易击中的bug（或许，大概，希望如此……这也是Dynamic的来源）。

* 数据的统计

数据的统计有两种思路，像是排队时间、服务器响应时间这样的统计数据即可以通过统计模块来统计也可以通过输出日志文件的方式留下数据。另一个方法是将其存储在自定义的
统计模块中，推荐是两种兼用——前者可以不必担心数据的丢失，后者可以在运行时实时查看数据。

* 关于开发

Dtest的核心代码在[./smdtest]中，第三方库包括boost（的异步）和smn-cpp（使用了其Ticker、LockManager等），boost的安装可以参考网络，smn-cpp的使用借助了工具smake（
我知道你是第一次听说），对应的手动操作参考./smdtest/sls.json，与其等价的命令如下：
sls.json

```
[{"type":"g","path":"github.com/ProtossGenius/smn-cpp/smncpp", "cmd_target":["make sm_build -j8"], "cmd_local":[" echo \"this is local cmd\""]}]
```
等价的命令
```
cd /tmp
git clone https://github.com/ProtossGenius/smn-cpp XXX/smn-cpp
ln -s XXX/smn-cpp/smncpp smncpp
cd smncpp && make sm_build -j8 
cd - && echo this is local cmd# 这个仅仅是测试用的

```
demo代码(将)位于./test/demo
在这里简短的介绍一下smake，因为demo代码中也会用到。它会分析目录树并且生成Makefile来确保所有的cpp都会被编译，并且在某些文件有修改的情况下能够（尽可能）最小程度的
重新编译。同时它也会读取目录下的sls.json文件，分析需要用到的第三方库（目前仅支持git{type:g}和本地目录{type:l}）并且创建软连接。而后依次分别在目标目录和sls.json所
在目录运行cmd\_target，cmd\_local，下面的json摘抄z自smdtest/sls.json

* 开发时的一些限制
1. 不能在Action中使用任何版本的thread::sleep()在异步调用中使用thread::sleep必然会导致死锁，作为代替smncpp中的ticker.h头文件中提供了sleep(int millsec)函数，可以安全的等待millsec毫秒。
2. 不应该再Action::Do中添加阻塞，因为Do和Recive是互斥的，正确的方法是将状态置为ActionStatus::WaitResult然后在Action::Recive中收到特定的包之后将Action的状态置为ActionStatus::Success
或者Fail。（看上去Action没有任何地方需要阻塞一会儿的，因为也没有必要在收包的时候阻塞一会儿）


