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
| `fn()=>`                                | `()=>`                   | 箭头函数去掉 fn            |
| `self`                                  | `this`                   | 当前类                     |
| `abstract`                              | __DELETE__               | 改为无方法代码块           |
| `readonly`                              | `@readonly`              | 改为注解                   |
| `final`                                 | `@final`                 | 改为注解                   |
| HTML                                    | `` ```TAG      TAG``` `` | HTML代码开始               |
| `<?php ?>`                              |                          | PHP文件开始省略            |

# 语法例子

```php
<?php

namespace TopNamespaceD.NamespaceD;

use TopNamespaceA.NamespaceA.ClassA;
use TopNamespaceB.NamespaceB.InterfaceA;
use TopNamespaceC.NamespaceC.InterfaceB;

@final
class ClassB
{
    use ClassA;
    use ClassC : MethodA as CMethodA, fun1 as CFun1;
    use InterfaceA;
    use InterfaceB;
    @readonly
    pub static int $pubStatic;

    ext static string $proStatic;

    inner static array $priStatic;

    pub int $pubVar;

    ext string $proVar;

    inner array $priVar;

    pub MethodA()
    {
        //method A
        $this.CMethodA();
    }

    pub fun1($args)
    {
        $this.CFun1();
        $this.pubVar = 1;
        this.$pubStatic = 'string1';
        $arr = ['k1' : 'v1', 'k2' : 'v2'];
        for($arr as $k : $v) {
        }

        for(true) {
        }

        do {
        } for (true);
    }

    ext fun2($args)
    {
        $this.proVar = 'string2';
        this.$proStatic = 'string2'
        $fn = ()=> $a + $b;// Arrow function
        if($a is $this) {
        }
    }

    inner fun3($args)
    {
        $this.priVar = [];
        this.$priStatic = [];
        ClassA.func2();
        this.funcStatic();
        $this.func2($a);
    }

    pub static funcStatic()
    {

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
    destruct { }

    // public function __sleep(): array
    sleep { }
    
    // public function __clone(): void
    clone { }

    // public function __debugInfo(): array
    dump { }

    // public function __get(string $name): mixed
    get($name) { }

    // public function __set(string $name, mixed $v): void
    set($name, $v) { }

    // public function __call(string $name, array $args): mixed
    call($name, $args) { }

    // public function __callStatic(string $name, array $args): mixed
    static($name, $args) { }

    // public function __isset(string $name): bool
    isset($name) { }

    // public function __unset(string $name): void
    unset($name) { }

    // public function __unserialize(array $data): void
    unserialize(array $data) { }
    
    // public static function __invoke(...$values): mixed
    invoke(...$values) { }

    // public static function __set_state(array $properties): object
    export(array $properties) { }
    
}

```