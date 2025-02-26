# PHP 语法变更列表


| 当前                                    | 变更                     | 说明                       |
| --------------------------------------- | ------------------------ | -------------------------- |
| `#[]`                                   | `@ `                     | 注解                       |
| `->`                                    | `.`                      | 对象操作符                 |
| `?->`                                   | `?.`                     | 对象操作符，Null 安全操作  |
| `::`                                    | `.`                      | 静态                       |
| `\`                                     | `.`                      | 命名空间分割符             |
| `extends`                               | `use CLASS`              | class 继承                 |
| `extends`                               | `use CLASS:METHOD as MN` | class 继承                 |
| `implements`                            | __DELETE__               | 接口实现                   |
| `include`                               | `use ?'__STRING__'`      | 包含文件，字符串           |
| `require`                               | `use '__STRING__'`       |                            |
| `trait`                                 | `use` `@NAME`            | 合并方法                   |
| `public function __get()`               | `get()`                  | get 魔术方法短写           |
| `public function __call()`              | `call()`                 | call 魔术方法短写          |
| `public static function __callStatic()` | `static()`               | callStatic 魔术方法短写    |
| `public function __set()`               | `set()`                  | set 魔术方法短写           |
| `public function __isset()`             | `isset()`                | isset 魔术方法短写         |
| `public function __unset()`             | `unset()`                | unset 魔术方法短写         |
| `public function __sleep()`             | `sleep()`                | sleep 魔术方法短写         |
| `public function __wakeup()`            | `wakeup()`               | wakeup 魔术方法短写        |
| `public function __serialize()`         | `serialize()`            | serialize 魔术方法短写     |
| `public function __unserialize()`       | `unserialize()`          | unserialize 魔术方法短写   |
| `public function __toString()`          | `string()`               | toString 魔术方法短写      |
| `public function __invoke()`            | `invoke()`               | invoke 魔术方法短写        |
| `public function __set_state()`         | `export()`               | setState 魔术方法短写      |
| `public function __debugInfo()`         | `dump()`                 | debugInfo 魔术方法短写     |
| `.`                                     | `#`                      | 字符连接符                 |
| `.=`                                    | `#=`                     | 字符连接赋值               |
| `foreach`                               | `for`                    | foreach 替代               |
| `while`                                 | `for`                    | while 替代                 |
| `instanceof`                            | `is`                     | 类型判断替代,支持所有类型  |
| `@`                                     | `@@`                     | 错误抑制符                 |
| `[ KEY => VAL ]`                        | `[ KEY : VAL ]`          | 数组声明中冒号代替箭头符号 |
| `array( KEY => VAL )`                   | `array( KEY : VAL )`     | 数组声明中冒号代替箭头符号 |
| `( ARRAY as KEY => VAL)`                | `( ARRAY as KEY : VAL)`  | foreach中冒号代替箭头符号  |
| `fn()=>`                                | `()->`                   | 箭头函数去掉 fn            |
| `self`                                  | `this`                   | 当前类                     |
| `abstract`                              | __DELETE__               | 改为无方法代码块           |
| `readonly`                              | `@readonly`              | 改为注解                   |
| `final`                                 | `@final`                 | 改为注解                   |
| HTML                                    | `` ```TAG      TAG``` `` | HTML代码开始               |
| `<?php ?>`                              |                          | PHP文件开始省略            |
| `switch case`                           | `case`                   |                            |
| `function`                              | `fn`                     |                            |

# 语法例子

```php
<?php

namespace TopNamespaceD.NamespaceD;

use TopNamespaceA.NamespaceA.ClassA;

global $a = 'superglobals'; //超全局变量
$b = 'local variable'; //局部变量
var $c = 'block variable c1'; //块变量$c

echo $c; //block variable c1
echo $a; //superglobals
echo $b; //local variable

case {
    1 =>
    expr;
    break
    2 =>
    expr
    break;
    3 ==>
    expr
    break;
    _ =>
    expr
    break;
}

for(true)
{
    echo $a; //superglobals
    echo $b; //local variable

    echo $c; //Notice:未定义
    var $c = 'block variable c2'; //新块变量$c
    echo $c; //block variable c2
}

echo $c; //block variable c1

fn func1()
{
    echo $a; //superglobals
    echo $b; //Notice:未定义
    echo $c; //Notice:未定义
}

fn func2()
{
    parent $b, $c;
    echo $a; //superglobals
    echo $b; //local variable
    echo $c; //block variable c1
}
if(true)
{
    var $d = 'block variable d';
    fn func3()
    {
        parent $b, $c, $d;
        echo $a; //superglobals
        echo $b; //local variable
        echo $c; //Notice:未定义
        echo $d; //block variable d
    }
}

class A {
    any funA()
    {
        echo $a; //superglobals
        echo $b; //Notice:未定义
    }
}

class C {
    any funA()
    {

    }
    any fun1()
    {

    }
}

@final
class ClassB use ClassA
{
    use ClassC : funA as cfunA, fun1 as cfun1;
    use NSTopB.NSD.ClassD;
    @readonly
    any static int $pubStatic;

    kin static string $proStatic;

    my static array $priStatic;

    pub int pubVar;//non-static-variable

    kin string proVar;

    my array priVar;

    any funA()
    {
        //method A
        $my.cfunA();
        $my:ClassA.funA();
        $my:NSTopB.NSD.ClassD.funD();
    }

    any fun1($args)
    {
        $my.cfun1();
        $my.pubVar = 1;
        my.$pubStatic = 'string1';
        $arr = ['k1' : 'v1', 'k2' : 'v2'];
        for($arr as $k : $v) {
        }

        for(true) {
        }

        do {
        } for (true);
    }

    kin fun2($args) int
    {
        $my.proVar = 'string2';
        my.$proStatic = 'string2'
        $fn = ()($a + $b);// Arrow function
        if($a is $my) {
        }
    }

    inner fun3($args)
    {
        $my.priVar = [];
        my.$priStatic = [];
        ClassA.func2();
        my.funcStatic();
        $my.func2($a);
    }

    any static funcStatic()
    {

    }
    any __init($arg1, $arg2) {

    }
    // public function __toString(): string
    string {  }

    // public function __toInt(): int
    int {  }

    // public function __toArray(): array
    array { }

    // public function __toBool(): bool
    bool {  }

    // public function __toFloat(): float
    float { }

    // public function __serialize(): array
    serialize { }

    // public function __wakeup(): void
    wakeup { }

    // public function __destruct(): void
    free { }

    // public function __sleep(): array
    sleep { }

    // public function __clone(): void
    clone { }

    // public function __debugInfo(): array
    dump { }

    // public function __get(string $name): mixed
    get { }

    // public function __set(string $name, mixed $value): void
    set { }

    // public function __call(string $name, array $value): mixed
    call  { }

    // public function __callStatic(string $name, array $value): mixed
    static  { }

    // public function __isset(string $name): bool
    isset { }

    // public function __unset(string $name): void
    unset { }

    // public function __unserialize(array $value): void
    unserialize { }

    // public static function __invoke(...$value): mixed
    invoke { }

    // public static function __set_state(array $value): object
    export  { }

}

```
