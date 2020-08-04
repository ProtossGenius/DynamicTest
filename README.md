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

后续大概会定义一个相对通用的统计模块。数据的统计有两种思路，像是排队时间、服务器响应时间这样的统计数据即可以通过统计模块来统计也可以通过输出日志文件的方式
留下数据。另一个方法是
