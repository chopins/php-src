# PHP alternative syntax

simplified write and reuse keyword 简化书写，复用关键字。

- [x] completed 已完成
- [ ] todo  待做
----
- [ ] remove php tag `<?php`,`?>`，移除php标签`<?php`,`?>`
- [x] namespace separator use dot `.` instead of `\`, 命名空间分隔符使用`.`代替`\`
    ```php
    namespace NS1.NS2.NS3;
    use NS4.NS5;
    ```
- [x] A attribute use `@` instead of `#[]`，silence use `@@`，remove`#[]`。after every attribute must have whitespace, betwen in attribute label and `(` can be no whitespace
    ```php
    @attr1 @attr2()
    @attr3
    class ClassName {
        @attr2
        public function method() {}
    }

    //@att5(),att6 is syntax error, right use @attr5 @attr6
    //@att6 ()  is syntax error, no whitspace,right use @attr6()
    //@att@attr is syntax error,  right use @attr @attr
    //@attr()@attr2 is syntax error, right use @attr() @attr
    //@attr(1, 4, 5) is right
    ```
- [x] can be use plus`+` instead of `extends`, can be use colon`:` instead of `implements`.  可使用`+`代替`extends`，使用`:`代替`implements`
    ```php
    class ClassName + ParentClass : Interface1,Interface2 {}
    ```

- [x] class member modifier can be use `any`,`kin`,`own` instead of `public`、`protected`、`private`.类成员修饰符`public`、`protected`、`private`,依次更改为为`any`、`kin`、`own`
    ```php
    class ClassName {
        any $publicProperty;
        kin $protectedProperty;
        own $privateProperty;
        any function publicMethod() {}
        kin function protectMethod() {}
        own function privateMethod() {}
    }
    ```
- [x] class method declaration can be omit `function.`类方法申明可省略`function`关键字
    ```php
    class ClassName {
        public publicMethod1() {}
        any publicMethod2() {}
    }
    ```
- [x] must be declaree magic method can be omit method modifiers, `function` and prefix underline. 必须定义为`public`的魔术方法可使用简略申明，省略修饰符与`function`关键字
    ```php
    //now syntax
    class now {
        public function __get() {

        }
        public function __callStatic() {

        }
    }
    //alternative syntax
    class alternative {
        get() {

        }
        callStatic() {

        }
    }
    ```
- [x] object opreater use dot`.` instead of `->` 对象操作符改用句点号`.`
    ```php
    class ClassName {
        any int $property;
        any method() {
            $this.$property = 0;
            $this.protectedMethod();
        }
        kin protectedMethod() {

        }
    }
    ```
- [x] string concat use `#` instead of `.`, remove `#` hash comment, `#=` instead of `.=`. 字符串连接符改用`#`号,`#=`代替`.=`
    ```lua
    $s1 = 'string1';
    $s2 = 'string2';
    $s3 = $s1 # $s2;
    $s4 = 'str1' # 'str2';
    ```
- [x] `for` structure same `foreach` and `while`.  `for`结构支持`foreach`,`while`语法
    ```php
    //example 1:
    for($arr as $k => $v) {

    }
    //equivalent to
    foreach($arr as $k => $v) {

    }

    //example 2:
    for($v > 1) {
    }
    //equivalent to
    while($v > 1) {

    }

    //example 3:
    do {

    }for($v > 1);
    //equivalent to
    do {

    }while($v > 1);
    ```
- [ ]  Add `int` list. 增加`int`列表类型
    ```php
    $v = int[3, 4, 56, 7];
    ```
- [ ]  Add `string` list. 增加`字符串`列表类型
    ```php
    $v = string['s1', 's2', 's3'];
    ```
- [ ] Add `bool` list. 增加`bool`列表类型
    ```php
    $v = bool[true, false];
    ```
- [ ] Add `float` list. 增加`float`列表类型
    ```php
    $v = float[1.2, 1.5, 1.6];
    ```
- [ ] Add double integer, size is 16 byte 增加`大整数`类型，16字节
    ```php
    $int = 2;
    $bint = (bint) $int;
    ```
- [ ] Add double integer list 增加`大整数`列表
    ```php
    $v = bint[1, 34, 45];
    ```
- [ ] Add here is html doc tag. 原php表签外的HTML或其他文本使用：`<?标识符` `标识符?>`包含，标识符规则尊守`heredoc`规则
    ```php
    //becase remove php tag <?php, so add here is html tag
    $a =1;
    <?HERE_STRING 
    any html string
    HERE_STRING?>

    //equivalent to
    <?php
    $a=1;
    ?>
    any html string
    ```
- [ ] Add block local variable 增加块级局部变量，使用`var`声明
    ```php
    if(true) {
        var $v = 2;
        var_dump($v); // output: 2;
    }
    var_dump($v); //output: undefined;

    foreach($arr as $k) {
        var $block = 1; 
        var_dump($block);//output:1
    }
    var_dump($block); //output:undefined
    ```



