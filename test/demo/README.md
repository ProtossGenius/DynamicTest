# DEMO

DynamicTest的Demo

* MyUser.h

自定义的User类，实现了父类的一些虚接口

* actions

用来存放自定义的Action，自定义的Action应该位于smdtest命名空间且使用dtaction宏，这样可以方便工具smdtcact手机Action信息

* actions.h actions.cpp

使用工具smdtcact生成的，不应该修改他们。作用是注册Action到ActionManager中，并且为他们生成一个字符串常量。

* my\_strategy h & cpp

一个简单的策略，包括了读取User数据、关闭User等常用操作的使用。

* sls.json smdtest smncpp

工具smake会用到，后两者是根据sls.json生成的软连接。

# Catalog
---
[<<< upper page](../README.md)
---

# SubCatalog

---
[<<< upper page](../README.md)
---