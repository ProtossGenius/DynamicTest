# DynamicTest

* 描述

C++动态测试。一种黑盒测试，作为经典单元测试的补充，特别针对网络服务器系统，关于下文中提到的类的含义请查看docs目录下的[术语表](./docs/Glossary.md)，这个表格很短，
所以建议在看下文之前先去喵一眼。(go.mod是为了写一些[工具](#about_tools))
DTest设计的思路是使用Action来模拟用户的基本操作（对于服务器系统来说就是发包）并且Action根据服务器回包来判断这项操作是否成功。由于User拥有唯
一ID（User::uid）接口，所以应该据此设计统计模块（而不应该在Action中修改User数据），User数据的只应该依据服务器的回包修改。


<h2 id="about_tools"> 工具</h2>
1. cmd/sdmtcact 
一个工具用于统计目标目录下的Action，并通过自动代码生成实现自动注册（注意，Action必须在smdtest命名空间，且必须要使用dtaction宏，dtaction的使用
可以参考 demo .

[demo](./test/demo/README.md)

# Catalog
* [\<file>datas_collect.md -> 数据的统计](./datas_collect.md)
* [\<file>dev_about.md -> 开发相关](./dev_about.md)
* [\<file>env_cfg.md -> 关于环境配置](./env_cfg.md)
* [\<file>what_can_do.md -> DTest提供的可能性](./what_can_do.md)

# SubCatalog

* [\<dir>docs -> 文档目录](./docs/README.md)
* [\<dir>smdtest -> 核心代码](./smdtest/README.md)
* [\<dir>test -> 测试目录](./test/README.md)