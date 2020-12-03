# 关于环境配置

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
