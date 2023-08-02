# SQL injection

## SQL injection vulnerability in WHERE clause allowing retrieval of hidden data

> 最基础的SQL注入，由于题目中已经告诉了语句格式，即告诉了`category`字段为单引号闭合，实际测试中可能需要花费一定时间去判断*脆弱字段*和*闭合形式*。

随便选择一个类别，然后修改类别名为不存在的名称，例如`abc`，然后进行单引号闭合，加入OR语句`or 1=1`，最后进行注释`--+`以使SQL语句成立，最后请求URL即为`https://xxxxxxx.web-security-academy.net/filter?category=abc'%20or%201%3D1--+`。
