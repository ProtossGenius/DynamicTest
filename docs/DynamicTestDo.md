# DynamicTest的事件驱动流程

1. CenterCtrl会根据需求创建User并调用user::start()。
2. User是异步的，User会通过Strategy生成Process，随后开始按照预设的间隔调用Action::Do()
3. Process结束之后User会继续通过Strategy生成Process。

# 需要注意的事项
1. Action::Do()和Action::Receive()是互斥的，所以不应该在Action::Do()中写下等待代码。
2. Action 无论何时都不应该修改User的数据（不论是Action::Do()还是Receive，数据的修改应该只由User进行）。
