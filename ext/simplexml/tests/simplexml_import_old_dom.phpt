--TEST--
SimpleXML [interop]: simplexml_import_dom (old DOM)
--EXTENSIONS--
simplexml
dom
--FILE--
<?php
$dom = new domDocument;
$dom->load(__DIR__."/book.xml");
if(!$dom) {
  echo "Error while parsing the document\n";
  exit;
}
$s = simplexml_import_dom($dom);
$books = $s->book;
foreach ($books as $book) {
    echo "{$book->title} was written by {$book->author}\n";
}
?>
--EXPECT--
The Grapes of Wrath was written by John Steinbeck
The Pearl was written by John Steinbeck
