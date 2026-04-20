--TEST--
Check if cffi is loaded
--EXTENSIONS--
cffi
--FILE--
<?php
echo 'The extension "cffi" is available';
?>
--EXPECT--
The extension "cffi" is available
