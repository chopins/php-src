--TEST--
test1() Basic test
--EXTENSIONS--
cffi
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension cffi is loaded and working!
NULL
