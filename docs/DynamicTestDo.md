# DynamicTest的事件驱动流程

（以下所有“自定义”的意味着需要用户自己实现）
1. 根据需求创建User并调用异步方法user::start()，User的产生速度是自定义的。
2. User是异步的，User会通过Strategy生成Process，随后开始按照Strategy提供的Ticker间隔调用Action::Do()
3. Process结束之后User会继续通过Strategy生成Process。

# 需要注意的事项
1. Action::Do()和Action::Receive()是互斥的，所以不应该在Action::Do()中写下等待代码。
2. Action 无论何时都不应该修改User的数据（不论是Action::Do()还是Receive，数据的修改应该只由User进行）。
3. Socket由Action 创建，但并不由Action或者User维护，Socket信息经由User提供的recvChan传递给User。Socket的管理是自定义的。
4. 接收到的数据（包）是以void* 的形式异步传递的，这意味着他们应当是通过new产生的以及总是需要安全的释放它们，合适的地方是User::recivePkg(void* pkg)
   它是包最后一次被调用的地方（在这里没有被释放掉的话资源便永远的泄露了）。同时千万不要尝试在Action中释放包。
