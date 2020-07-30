# DynamicTest的事件驱动流程

1. UserManager会根据需求创建User并调用user::start()，它的另一个功能是处理来自SMNRPC的命令（这部分是可以完全定制的）。
2. User是异步的，User会通过Strategy生成Process，随后开始按照预设的间隔（间接的）调用Action::Do()
3. Process结束之后User会重新回到初状态，继续通过Strategy生成Process。

# 需要注意的事项
1. Action::Do()和Action::Receive()是互斥的，所以不应该在Action::Do()中写下等待代码。


