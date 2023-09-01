# PHP new syntax changed list


|当前  |    变更     |       说明          |
|------|-------------|---------------------|
|`#[]` |  `@ `       |注解            |
|`->`  |  `.`        |对象操作符    |
|`?->` |`?.`|对象操作符，Null 安全操作|
|`\`   | `.`        |命名空间分割符|
|`extends`|`+`      |class 继承|
|`implements`|`:` |接口实现|
|`public function __get()`|`get()`|get 魔术方法短写|
|`public function __call()`|`call()`|call 魔术方法短写|
|`public function __callStatic()`|`callStatic()`|callStatic 魔术方法短写|
|`public function __set()`|`set()`|set 魔术方法短写|
|`public function __isset()`|`isset()`|isset 魔术方法短写|
|`public function __unset()`|`unset()`|unset 魔术方法短写|
|`public function __sleep()`|`sleep()`|sleep 魔术方法短写|
|`public function__wakeup()`|`wakeup()`|wakeup 魔术方法短写|
|`public function __serialize()`|`serialize()`|serialize 魔术方法短写|
|`public function __unserialize()`|`unserialize()`|unserialize 魔术方法短写|
|`public function __toString()`|`toString()`|toString 魔术方法短写|
|`public function __invoke()`|`invoke()`|invoke 魔术方法短写|
|`public function __set_state()`|`setState()`|setState 魔术方法短写| 
|`public function __debugInfo()`|`debugInfo()`|debugInfo 魔术方法短写|
|`.`|`#`|字符连接符|
|`.=`|`#=`|字符连接赋值|
|`foreach`|`for`|foreach 替代|
|`while`|`for`|while 替代|
|`instanceof`|`is`|类型判断替代,支持所有类型|
|`@`|`@@`|错误抑制符|
|`[ KEY => VAL ]`|`[ KEY : VAL ]`|数组声明中冒号代替箭头符号|
|`array( KEY => VAL )`|`array( KEY : VAL )`|数组声明中冒号代替箭头符号|
|`( ARRAY as KEY => VAL)`|`( ARRAY as KEY : VAL)` |foreach中冒号代替箭头符号|
|`fn()=>`|`()=>`|箭头函数去掉 fn|
|`self`|`this`|当前类|
|`abstract`|`@abstract`|改为注解|
|`readonly`|`@readonly`|改为注解|
|`final`|`@final`|改为注解|