# PHP alternative syntax

- [x] 以下表示已完成, this completed
- [ ] 以下表示未完成, this todo
----
- [ ] 可以去掉PHP标签`<?php`,`?>`
- [x] 命名空间分割符可以使用句点号`.`,规则如故
    ```php
    namespace NS1.NS2.NS3;
    use NS4.NS5;
    ```
- [x] 属性改为以`@`开头，错误抑制号改用`@@`号，删除`#[]`。每个属性后必须跟随空白，属性名与`(`之间不能有空格
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
    ```
- [x] 可使用`+`代替`extends`，使用`:`代替`implements`
    ```php
    class ClassName + ParentClass : Interface1,Interface2 {}
    ```

- [x] 类成员修饰符`public`、`protected`、`private`,依次更改为为`any`、`kin`、`own`
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
- [x] 类方法申明可省略`function`关键字
    ```php
    class ClassName {
        public publicMethod1() {}
        any publicMethod2() {}
    }
    ```
- [x] 必须定义为`public`的魔术方法可使用简略申明，省略修饰符与`function`关键字
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
- [x] 对象操作符改用句点号`.`
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
- [x] 字符串连接符改用`#`号,`#=`代替`.=`
    ```lua
    $s1 = 'string1';
    $s2 = 'string2';
    $s3 = $s1 # $s2;
    $s4 = 'str1' # 'str2';
    ```
- [x] `for`结构支持`foreach`,`while`语法
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
- [ ]  箭头函数可以使用`->`号并省略`fn`关键词
    ```php
    $fn = ($x) -> $x + $y;
    //equivalent to
    $fn1= fn($x)=> $x + $y;
    ```
- [ ]  增加`int`列表类型
    ```php
    $v = int[3, 4, 56, 7];
    ```
- [ ]  增加`字符串`列表类型
    ```php
    $v = string['s1', 's2', 's3'];
    ```
- [ ]  增加`bool`列表类型
    ```php
    $v = bool[true, false];
    ```
- [ ]  增加`float`列表类型
    ```php
    $v = float[1.2, 1.5, 1.6];
    ```
- [ ]  增加`大整数`类型，16字节
    ```php
    $int = 2;
    $bint = (bint) $int;
    ```
- [ ]  增加`大整数`列表
    ```php
    $v = bint[1, 34, 45];
    ```
- [ ]  原php表签外的HTML或其他文本使用：`<?标识符` `标识符?>`包含，标识符规则尊守`heredoc`规则
    ```php
    <?HERE_STRING 
    any string
    HERE_STRING?>
    ```
- [ ]  增加块级局部变量，使用`var`声明
    ```php
    $v = 1;
    if(true) {
        var $v = 2;
        var_dump($v); // output: 2;
    }
    var_dump($v); //output: 1;

    $block = 2;
    foreach($arr as $k) {
        var $block = 1; 
        var_dump($block);//output:1
    }
    var_dump($block); //output:2
    ```



