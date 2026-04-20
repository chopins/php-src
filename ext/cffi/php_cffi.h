/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author:  |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_CFFI_H
# define PHP_CFFI_H

extern zend_module_entry cffi_module_entry;
# define phpext_cffi_ptr &cffi_module_entry

ZEND_BEGIN_MODULE_GLOBALS(cffi)
   bool is_cli;
   HashTable *dl_class_table;
ZEND_END_MODULE_GLOBALS(cffi)

ZEND_EXTERN_MODULE_GLOBALS(cffi);

# if defined(ZTS) && defined(COMPILE_DL_CFFI)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_CFFI_H */
