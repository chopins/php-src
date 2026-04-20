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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "php_cffi.h"
#include "zend_API.h"

#include <ffi.h>
#include <sys/types.h>

#include "cffi_arginfo.h"
#include "ext/standard/info.h"
#include "main/SAPI.h"

#include "zend_exceptions.h"
#include "zend_observer.h"

#ifdef HAVE_LIBDL
#ifdef PHP_WIN32
#include "win32/param.h"
#include "win32/winutil.h"
#define GET_DL_ERROR() php_win_err()
#else
#include <sys/param.h>
#define GET_DL_ERROR() DL_ERROR()
#endif
#endif

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(cffi)

static ZEND_NAMED_FUNCTION(cffi_call_api_trampoline);

typedef struct _cffi_type_object {
	zend_object std;
	ffi_type type;
	union
	{
		int8_t i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		float f32;
		double f64;
		char c[1];
		void* p;
		#ifdef HAVE_LONG_DOUBLE
		long double ld;
		#endif
	} v;
} cffi_type_object;

static zend_class_entry* cffi_ce_C_Exception;
static zend_class_entry* cffi_ce_C_DL;
static zend_class_entry* cffi_ce_C_Type;
static zend_class_entry* cffi_ce_C_CArray;
static zend_class_entry* cffi_ce_C_Struct;
static zend_class_entry* cffi_ce_C_Union;
//#ifdef HAVE_FFI_STDCALL
static zend_class_entry* cffi_ce_C_ABI_Stdcall;
//#endif
#ifdef HAVE_FFI_VECTORCALL_PARTIAL
static zend_class_entry* cffi_ce_C_ABI_Vectorcall;
#endif
#ifdef HAVE_FFI_FASTCALL
static zend_class_entry* cffi_ce_C_ABI_Fastcall;
#endif
#ifdef HAVE_FFI_THISCALL
static zend_class_entry* cffi_ce_C_ABI_Thiscall;
#endif
#ifdef HAVE_FFI_PASCAL
static zend_class_entry* cffi_ce_C_ABI_Pascal;
#endif
#ifdef HAVE_FFI_REGISTER
static zend_class_entry* cffi_ce_C_ABI_Register;
#endif
#ifdef HAVE_FFI_MS_CDECL
static zend_class_entry* cffi_ce_C_ABI_MSCdecl;
#endif
#ifdef HAVE_FFI_SYSV
static zend_class_entry* cffi_ce_C_ABI_Sysv;
#endif

static zend_class_entry* cffi_ce_C_Unsigned;
static zend_class_entry* cffi_ce_C_Pointer;
static zend_class_entry* cffi_ce_C_P1;
static zend_class_entry* cffi_ce_C_P2;
static zend_class_entry* cffi_ce_C_P3;
static zend_class_entry* cffi_ce_C_P4;
static zend_class_entry* cffi_ce_C_Void;
static zend_class_entry* cffi_ce_C_Char;
static zend_class_entry* cffi_ce_C_Float32;
static zend_class_entry* cffi_ce_C_Double;
static zend_class_entry* cffi_ce_C_Int8;
static zend_class_entry* cffi_ce_C_Int16;
static zend_class_entry* cffi_ce_C_Int32;
static zend_class_entry* cffi_ce_C_Int64;
#ifdef HAVE_LONG_DOUBLE
static zend_class_entry* cffi_ce_C_LongDouble;
#endif

static zend_object_handlers cffi_type_handlers;
static zend_object_handlers cffi_api_handlers;

#define CFFI_IMP_REQUIRE_ERROR(error, tname) zend_error_at_noreturn(E_COMPILE_ERROR, class_type->info.user.filename, class_type->info.user.line_start, error, ZSTR_VAL(class_type->name), tname)

static int cffi_implement_check_require(zend_class_entry* interface, zend_class_entry* class_type) /* {{{ */
{
	if ((interface == cffi_ce_C_CArray || interface == cffi_ce_C_Pointer) && !instanceof_function(class_type, cffi_ce_C_Type)) {
		CFFI_IMP_REQUIRE_ERROR("Class %s must first inherit from class %s", ZSTR_VAL(cffi_ce_C_Type->name));
		return FAILURE;
	}

	if(interface == cffi_ce_C_Unsigned) {
		if(!EXPECTED(class_type->backed_enum_table == NULL)) {
			return FAILURE;
		}
		if(class_type->backed_enum_table == (HashTable*)&ffi_type_sint8) {
			class_type->backed_enum_table = (HashTable*)&ffi_type_uint8;
		} else if(class_type->backed_enum_table == (HashTable*)&ffi_type_sint16) {
			class_type->backed_enum_table = (HashTable*)&ffi_type_uint16;
		} else if(class_type->backed_enum_table == (HashTable*)&ffi_type_sint32) {
			class_type->backed_enum_table = (HashTable*)&ffi_type_uint32;
		} else if(class_type->backed_enum_table == (HashTable*)&ffi_type_sint64) {
			class_type->backed_enum_table = (HashTable*)&ffi_type_uint64;
		}
		return SUCCESS;
	}

	char* const_name = interface == cffi_ce_C_CArray ? "SIZE" : "LEVEL";

	zend_string* const_name_str = ZSTR_INIT_LITERAL(const_name, 0);
	zend_class_constant* const_val = zend_hash_find_ptr(CE_CONSTANTS_TABLE(class_type), const_name_str);
	zend_string_free(const_name_str);
	if (const_val == NULL) {
		CFFI_IMP_REQUIRE_ERROR("Const %s::%s is not defined", const_name);
		return FAILURE;
	}
	if(interface == cffi_ce_C_Pointer) {
		if(Z_TYPE(const_val->value) != IS_LONG) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be an integer", const_name);
			return FAILURE;
		}
		if(Z_LVAL(const_val->value) <= 0) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be greater 0", const_name);
			return FAILURE;
		}
		class_type->backed_enum_table = (HashTable*)&ffi_type_pointer;
		return SUCCESS;
	}

	if (Z_TYPE(const_val->value) != IS_ARRAY || !HT_IS_PACKED(Z_ARRVAL(const_val->value))) {
		CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be an integer list", const_name);
		return FAILURE;
	}

	zval* val;
	ZEND_HASH_PACKED_FOREACH_VAL(Z_ARRVAL(const_val->value), val) {
		if (Z_TYPE_P(val) != IS_LONG) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be an integer list", const_name);
			return FAILURE;
		}
		if (Z_LVAL_P(val) <= 0) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be an integer list and each element must > 0", const_name);
			return FAILURE;
		}
	}
	ZEND_HASH_FOREACH_END();

	return SUCCESS;
}

static ffi_type* cffi_ctype_ce_set_ffi_type(zend_type* type) {
	zend_class_entry *ce;
	if(ZEND_TYPE_IS_COMPLEX(*type)) {
		if(UNEXPECTED(ZEND_TYPE_HAS_LIST(*type))) {
			zend_type *list_type;
			if(ZEND_TYPE_LIST(*type)->num_types > 2) {
				return NULL;
			}
			ZEND_TYPE_LIST_FOREACH(ZEND_TYPE_LIST(*type), list_type) {
				 ce = zend_lookup_class(ZEND_TYPE_NAME(*type));
				if (UNEXPECTED(!ce)) {
					return NULL;
				}
				if(instanceof_function(ce, cffi_ce_C_Pointer)) {
					return &ffi_type_pointer;
				}
			}ZEND_TYPE_LIST_FOREACH_END();
			return (ffi_type*)ce->backed_enum_table;
		} else if(ZEND_TYPE_HAS_NAME(*type)) {
			ce = zend_lookup_class(ZEND_TYPE_NAME(*type));
			if (UNEXPECTED(!ce)) {
				return NULL;
			}
			if(!instanceof_function(ce, cffi_ce_C_Type)) {
				return NULL;
			}
			return (ffi_type*)ce->backed_enum_table;
		}
	} else {
		uint32_t type_mask = ZEND_TYPE_FULL_MASK(*type);
		if(type_mask & MAY_BE_STRING) {
			return &ffi_type_pointer;
		} else if(type_mask & MAY_BE_LONG) {
			return &ffi_type_sint64;
		} else if(type_mask & MAY_BE_DOUBLE) {
			return &ffi_type_double;
		} else if(type_mask & MAY_BE_REF) {
			return &ffi_type_pointer;
		}
	}
	return NULL;
}

static int cffi_implement_c_func_abi_type(zend_class_entry* interface, zend_class_entry* class_type)
{
	class_type->enum_backing_type = interface->enum_backing_type;
	return SUCCESS;
}

static int cffi_implement_c_dl(zend_class_entry* interface, zend_class_entry* class_type) {
	DL_HANDLE handle = NULL;
	char* err;
	zend_function *method;
	zend_string* dl_file = zend_string_init("DL_FILE", sizeof("DL_FILE") - 1, 0);
	zend_class_constant* dl_file_const = zend_hash_find_ptr(CE_CONSTANTS_TABLE(class_type), dl_file);
	zend_string_release(dl_file);
	if(!UNEXPECTED(class_type->enum_backing_type)) {
		class_type->enum_backing_type = interface->enum_backing_type;
	}
	if (Z_STRLEN(dl_file_const->value) <= 0) {
		CFFI_IMP_REQUIRE_ERROR("Const %s::DL_FILE value must be defined", "DL_FILE");
		return FAILURE;
	}

	handle = DL_LOAD(Z_STRVAL(dl_file_const->value));
	if (!handle) {
		err = GET_DL_ERROR();
		#ifdef PHP_WIN32
		if (err && err[0]) {
			zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (%s)", ZSTR_VAL(lib), err);
			php_win32_error_msg_free(err);
		} else {
			zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (Unknown reason)", ZSTR_VAL(lib));
		}
		#else
		zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (%s)", Z_STRVAL(dl_file_const->value), err);
		GET_DL_ERROR(); /* free the buffer storing the error */
		#endif
		return FAILURE;
		#ifdef RTLD_DEFAULT
	} else if (1) {
		// TODO: this might need to be disabled or protected ???
		handle = RTLD_DEFAULT;
		#endif
	}

	ZEND_HASH_FOREACH_PTR(&class_type->function_table, method) {
		if (method->common.fn_flags & (ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)) {
			void* addr = DL_FETCH_SYMBOL(handle, ZSTR_VAL(method->common.function_name));
			if (!addr) {
				CFFI_IMP_REQUIRE_ERROR("Class %s can not resolving C function '%s'", ZSTR_VAL(method->common.function_name));
				return FAILURE;
			}

			method->common.type = ZEND_INTERNAL_FUNCTION;
			method->common.fn_flags = method->common.fn_flags;
			method->internal_function.handler = cffi_call_api_trampoline;
			zend_arg_info* arg_ptr = method->common.arg_info - 1;
			zend_internal_arg_info* args;
			args = emalloc(sizeof(zend_internal_arg_info));

			uint32_t num_args = method->common.num_args + 1;
			if(method->common.fn_flags & ZEND_ACC_VARIADIC) {
				num_args++;
			}

			zend_internal_arg_info* internal_arg_info = ecalloc(num_args + 1, sizeof(zend_internal_arg_info));
			ffi_type** args_ffi_type = emalloc(sizeof(ffi_type*) * num_args);
			uint32_t i;
			printf("num args %d\n", num_args);
			for (i = 0; i < num_args; i++) {
				args->name = ZSTR_VAL(arg_ptr[i].name);
				args->type = arg_ptr[i].type;
				args->default_value = ZSTR_VAL(arg_ptr[i].default_value);
				internal_arg_info[i] = *args;
				args_ffi_type[i] = cffi_ctype_ce_set_ffi_type(&arg_ptr[i].type);
			}
			efree(args);
			method->internal_function.arg_info = internal_arg_info + 1;
			method->internal_function.reserved[0] = arg_ptr + 1;
			method->internal_function.reserved[1] = addr;
			method->internal_function.reserved[2] = args_ffi_type;
		}
	}
	class_type->backed_enum_table = handle;
	zend_hash_add_ptr(cffi_globals.dl_class_table, class_type->name, class_type);
	ZEND_HASH_FOREACH_END();
	return SUCCESS;
}
/*
static zend_object* cffi_api_new(zend_class_entry* class_ce) {
	cffi_api_object* capi;
	capi = zend_object_alloc(sizeof(cffi_api_object), class_ce);
	zend_object_std_init(&capi->std, class_ce);
	object_properties_init(&capi->std, class_ce);
	capi->lib = NULL;
	zend_hash_init(capi->func_list, 0, NULL, NULL, 1);
	zend_hash_init(capi->var_list, 0, NULL, NULL, 1);
	return &capi->std;
}*/
/*
static zend_function* cffi_get_call_trampoline_func(zend_object* object, zend_function* method) {
	zend_function* trampoline;

	trampoline = ecalloc(1, sizeof(zend_internal_function));

	trampoline->common.type = ZEND_INTERNAL_FUNCTION;
	trampoline->common.fn_flags = method->common.fn_flags | ZEND_ACC_CALL_VIA_TRAMPOLINE;
	trampoline->common.arg_flags[0] = method->common.arg_flags[0];
	trampoline->common.arg_flags[1] = method->common.arg_flags[1];
	trampoline->common.arg_flags[2] = method->common.arg_flags[2];
	trampoline->internal_function.handler = cffi_call_api_trampoline;
	trampoline->common.function_name = method->common.function_name;
	trampoline->common.scope = method->common.scope;
	trampoline->common.num_args = method->common.num_args;
	trampoline->common.required_num_args = method->common.required_num_args;
	trampoline->common.doc_comment = method->common.doc_comment;
	trampoline->common.prototype = method->common.prototype;
	// trampoline->common.arg_info = method->common.arg_info;

	zend_arg_info* arg_ptr = method->common.arg_info - 1;
	zend_internal_arg_info* args;
	args = emalloc(sizeof(zend_internal_arg_info));

	uint32_t num_args = method->common.num_args + 1;

	zend_internal_arg_info* internal_arg_info = ecalloc(num_args + 1, sizeof(zend_internal_arg_info));

	for (uint32_t i = 0; i < num_args; i++) {
		args->name = ZSTR_VAL(arg_ptr[i].name);
		args->type = arg_ptr[i].type;
		args->default_value = ZSTR_VAL(arg_ptr[i].default_value);
		internal_arg_info[i] = *args;
	}
	efree(args);
	trampoline->internal_function.arg_info = internal_arg_info + 1;
	trampoline->internal_function.reserved[0] = object;
	return trampoline;
}*/
/*
static zend_function* cffi_api_get_method(zend_object** object, zend_string* method, const zval* key) {
	zend_object* zobj = *object;
	zval* func;
	zend_function* fbc;
	printf("CALL: cffi_api_get_method\n");
	if (UNEXPECTED((func = zend_hash_find(&zobj->ce->function_table, method)) != NULL)) {
		fbc = Z_FUNC_P(func);
		if (fbc->common.fn_flags & (ZEND_ACC_PUBLIC)) {
			return cffi_get_call_trampoline_func(zobj, fbc);
		}
	}
	return zend_std_get_method(object, method, key);
}
*/
zval* cffi_api_read_property(zend_object* object, zend_string* member, int type, void** cache_slot, zval* rv) {}
zval* cffi_api_write_property(zend_object* object, zend_string* member, zval* value, void** cache_slot) {}

static zend_object* cffi_type_new(zend_class_entry* class_type) /* {{{ */
{
	cffi_type_object* ctype;

	ctype = zend_object_alloc(sizeof(cffi_type_object), class_type);

	zend_object_std_init(&ctype->std, class_type);
	object_properties_init(&ctype->std, class_type);
	// if (ctype_ce->ffi_type_unsigned != NULL &&
	// zend_class_implements_interface(class_type, cffi_ce_C_Unsigned))
	// {
	// 	ctype->type = *ctype_ce->ffi_type_unsigned;
	// }
	if (zend_class_implements_interface(class_type, cffi_ce_C_Pointer)) {
		ctype->type = ffi_type_pointer;
	}

	return &ctype->std;
}
/* }}} */

static ZEND_NAMED_FUNCTION(cffi_call_api_trampoline) /* {{{ */
{
	uint32_t num_args = EX(func)->common.num_args;
	uint32_t pass_num_args = EX_NUM_ARGS();
	zval *args;
	int argc;
	uint32_t max_args = EX(func)->common.fn_flags & ZEND_ACC_VARIADIC ? pass_num_args : num_args;
	uint32_t func_abi = EX(func)->common.scope->enum_backing_type;
	//zend_long --> int64 int32 int16 int8
	//zend_string ---> char* pointer
	//zend_float ---> double
	//C\Type object --> pointer

	ZEND_PARSE_PARAMETERS_START(num_args, max_args)
		Z_PARAM_VARIADIC('+', args, argc);
	ZEND_PARSE_PARAMETERS_END();


	void* addr = EX(func)->internal_function.reserved[1];
	ffi_type** arg_types = EX(func)->internal_function.reserved[2];
	ffi_status status;
	ffi_cif cif;
	void* ret;
	void** arg_values = do_alloca(pass_num_args + (8 * sizeof(void*)), arg_values_use_heap);

	zend_string* fname = EX(func)->common.function_name;
	zend_class_entry* cls = EX(func)->common.scope;
	printf("CALL: cffi_call_api_trampoline: class:%s, fname:%s\n", ZSTR_VAL(cls->name), ZSTR_VAL(fname));

	if((status = ffi_prep_cif(&cif, func_abi, 2, arg_types[0], arg_types + 1)) != FFI_OK) {
		zend_throw_error(cffi_ce_C_Exception, "Failed call '%s' (%d)", ZSTR_VAL(fname), status);
		RETURN_THROWS();
	}

	//ffi_call(&cif, addr, ret, arg_values);

	//RETURN_STR(fname);
}

// ZEND_METHOD(C_DL, __construct) {
// 	zend_string* lib;
// 	zend_function* method;
// 	cffi_api_object* cffi;
// 	DL_HANDLE handle = NULL;
// 	char* err;

// 	ZEND_PARSE_PARAMETERS_START(1, 1)
// 	Z_PARAM_STR(lib)
// 	ZEND_PARSE_PARAMETERS_END();

// 	if (lib) {
// 		handle = DL_LOAD(ZSTR_VAL(lib));
// 		if (!handle) {
// 			err = GET_DL_ERROR();
// #ifdef PHP_WIN32
// 			if (err && err[0]) {
// 				zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (%s)", ZSTR_VAL(lib), err);
// 				php_win32_error_msg_free(err);
// 			} else {
// 				zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (Unknown reason)", ZSTR_VAL(lib));
// 			}
// #else
// 			zend_throw_error(cffi_ce_C_Exception, "Failed loading '%s' (%s)", ZSTR_VAL(lib), err);
// 			GET_DL_ERROR(); /* free the buffer storing the error */
// #endif
// 			RETURN_THROWS();
// 		}

// #ifdef RTLD_DEFAULT
// 	} else if (1) {
// 		// TODO: this might need to be disabled or protected ???
// 		handle = RTLD_DEFAULT;
// #endif
// 	}
// 	cffi = (cffi_api_object*)Z_OBJ_P(getThis());
// 	zend_class_entry* cffi_api_ce = Z_OBJCE_P(getThis());
// 	zend_hash_add_ptr(cffi_globals.dl_class_table, cffi_api_ce->name, cffi_api_ce);
// 	cffi->lib = handle;

// 	ZEND_HASH_FOREACH_PTR(&cffi_api_ce->function_table, method) {
// 		if (method->common.fn_flags & ZEND_ACC_PUBLIC) {
// 			void* addr = DL_FETCH_SYMBOL(cffi->lib, ZSTR_VAL(method->common.function_name));
// 			if (!addr) {
// 				zend_throw_error(cffi_ce_C_Exception, "Failed resolving C function '%s'", ZSTR_VAL(method->common.function_name));
// 				RETURN_THROWS();
// 			}
// 			zend_hash_add_ptr(cffi->func_list, method->common.function_name, addr);
// 		}
// 	}
// 	ZEND_HASH_FOREACH_END();
// 	zend_property_info* property;
// 	int properts_num = cffi_api_ce->default_properties_count;
// 	ZEND_HASH_FOREACH_PTR(&cffi_api_ce->properties_info, property) {
// 		if (property->flags & ZEND_ACC_PUBLIC) {
// 			void* addr = DL_FETCH_SYMBOL(cffi->lib, ZSTR_VAL(property->name));
// 			if (!addr) {
// 				zend_throw_error(cffi_ce_C_Exception, "Failed resolving C variable '%s'", ZSTR_VAL(property->name));
// 				RETURN_THROWS();
// 			}
// 			zend_hash_add_ptr(cffi->var_list, property->name, addr);
// 		}
// 	}
// 	ZEND_HASH_FOREACH_END();
// }

ZEND_METHOD(C_DL, getMethods) {
	zend_string* path;
	zend_function* func;

	ZEND_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	zend_class_entry* ce = Z_OBJ_P(getThis())->ce;

	array_init(return_value);
	ZEND_HASH_FOREACH_PTR(&ce->function_table, func) {
		if (func->common.fn_flags & ZEND_ACC_PUBLIC) {
			add_next_index_str(return_value, zend_string_copy(func->common.function_name));
		}
	}
	ZEND_HASH_FOREACH_END();
}

ZEND_METHOD(C_Type, __construct) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, isNull) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, free) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, getValue) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, memcmp) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, memcpy) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, memset) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, addr) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, cast) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, sizeof) { RETURN_BOOL(1); }

ZEND_METHOD(C_Type, getAlignment) { RETURN_BOOL(1); }

ZEND_API zend_observer_fcall_handlers cffi_execute_get_c_defined(zend_execute_data* execute_data) {
	char* fn;
	printf("get c defined for observer\n");
	if (EX_CALL_KIND()) {
		printf("execute top code\n");
	}
	return (zend_observer_fcall_handlers){NULL, NULL};
}


#define CFFI_REG_ABI_INTERFACE(class, abi)  cffi_ce_C_ABI_ ## class = register_class_C___ ## class(); \
	cffi_ce_C_ABI_ ## class->interface_gets_implemented = cffi_implement_c_func_abi_type;\
	cffi_ce_C_ABI_ ## class ->enum_backing_type= FFI_ ## abi;


ZEND_MINIT_FUNCTION(cffi) {
	// zend_observer_fcall_register(cffi_execute_get_c_defined);
	cffi_globals.is_cli = strcmp(sapi_module.name, "cli") == 0;
	zend_register_auto_global(zend_string_init_interned("_C", sizeof("_C") - 1, 1), 0, NULL);
	cffi_ce_C_Exception = register_class_C_Exception(zend_ce_error);
	cffi_ce_C_DL = register_class_C_DL();
	cffi_ce_C_DL->enum_backing_type = FFI_DEFAULT_ABI;
	cffi_ce_C_DL->interface_gets_implemented = cffi_implement_c_dl;
	// cffi_ce_C_DL->create_object = cffi_api_new;
	// cffi_ce_C_DL->default_object_handlers = &cffi_api_handlers;
	// memcpy(&cffi_api_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	// cffi_api_handlers.get_constructor = zend_std_get_constructor;
	// cffi_api_handlers.free_obj = zend_object_std_dtor;
	// cffi_api_handlers.clone_obj = NULL;
	// cffi_api_handlers.read_property = cffi_api_read_property;
	// cffi_api_handlers.write_property = cffi_api_write_property;
	// cffi_api_handlers.read_dimension = zend_std_read_dimension;
	// cffi_api_handlers.write_dimension = zend_std_write_dimension;
	// cffi_api_handlers.get_property_ptr_ptr = zend_std_get_property_ptr_ptr;
	// cffi_api_handlers.has_property = zend_std_has_property;
	// cffi_api_handlers.unset_property = zend_std_unset_property;
	// cffi_api_handlers.has_dimension = zend_std_has_dimension;
	// cffi_api_handlers.unset_dimension = zend_std_unset_dimension;
	// cffi_api_handlers.get_method = cffi_api_get_method;
	// cffi_api_handlers.compare = zend_std_compare_objects;
	// cffi_api_handlers.cast_object = zend_std_cast_object_tostring;
	// cffi_api_handlers.get_debug_info = NULL;
	// cffi_api_handlers.get_closure = NULL;
	// cffi_api_handlers.get_properties = zend_std_get_properties;
	// cffi_api_handlers.get_gc = zend_std_get_gc;
	#ifdef HAVE_FFI_STDCALL
	CFFI_REG_ABI_INTERFACE(Stdcall, STDCALL);
	#endif
	#ifdef HAVE_FFI_VECTORCALL_PARTIAL
	CFFI_REG_ABI_INTERFACE(Vectorcall,VECTORCALL_PARTIAL);
	#endif
	#ifdef HAVE_FFI_FASTCALL
	CFFI_REG_ABI_INTERFACE(Fastcall, FASTCALL);
	#endif
	#ifdef HAVE_FFI_THISCALL
	CFFI_REG_ABI_INTERFACE(Thiscall,THISCALL);
	#endif
	#ifdef HAVE_FFI_PASCAL
	CFFI_REG_ABI_INTERFACE(Pascal,PASCAL);
	#endif
	#ifdef HAVE_FFI_REGISTER
	CFFI_REG_ABI_INTERFACE(Register, REGISTER);
	#endif
	#ifdef HAVE_FFI_MS_CDECL
	CFFI_REG_ABI_INTERFACE(MSCdecl,MS_CDECL);
	#endif
	#ifdef HAVE_FFI_SYSV
	CFFI_REG_ABI_INTERFACE(Sysv,SYSV);
	#endif
	cffi_ce_C_CArray = register_class_C_CArray();
	cffi_ce_C_CArray->interface_gets_implemented = cffi_implement_check_require;

	cffi_ce_C_Unsigned = register_class_C_Unsigned();
	cffi_ce_C_Unsigned->interface_gets_implemented = cffi_implement_check_require;
	cffi_ce_C_Pointer = register_class_C_Pointer();
	cffi_ce_C_Pointer->interface_gets_implemented = cffi_implement_check_require;

	cffi_ce_C_P1 = register_class_C_P1(cffi_ce_C_Pointer);

	cffi_ce_C_P1->backed_enum_table = (HashTable*)&ffi_type_pointer;

	cffi_ce_C_P2 = register_class_C_P2(cffi_ce_C_Pointer);
	cffi_ce_C_P2->backed_enum_table = (HashTable*)&ffi_type_pointer;

	cffi_ce_C_P3 = register_class_C_P3(cffi_ce_C_Pointer);

	cffi_ce_C_P3->backed_enum_table = (HashTable*)&ffi_type_pointer;

	cffi_ce_C_P4 = register_class_C_P4(cffi_ce_C_Pointer);

	cffi_ce_C_P4->backed_enum_table = (HashTable*)&ffi_type_pointer;

	cffi_ce_C_Type = register_class_C_Type();
	cffi_ce_C_Type->create_object = cffi_type_new;
	cffi_ce_C_Type->default_object_handlers = &cffi_type_handlers;
	cffi_ce_C_Type->backed_enum_table = NULL;
	memcpy(&cffi_type_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	// cffi_type_handlers.write_property = cffi_type_write_property;
	// cffi_type_handlers.unset_property = cffi_type_unset_property;
	// cffi_type_handlers.free_obj = cffi_type_free_obj;
	// cffi_type_handlers.clone_obj = cffi_type_clone_obj;
	// cffi_type_handlers.cast_object = cffi_type_cast_obj;
	// cffi_type_handlers.do_operation = cffi_type_do_operation;
	// cffi_type_handlers.compare = cffi_type_compare;

	cffi_ce_C_Struct = register_class_C_Struct(cffi_ce_C_Type);
	cffi_ce_C_Union = register_class_C_Union(cffi_ce_C_Type);

	cffi_ce_C_Void = register_class_C_CVoid(cffi_ce_C_Type);
	cffi_ce_C_Void->backed_enum_table = (HashTable*)&ffi_type_void;

	cffi_ce_C_Int8 = register_class_C_I8(cffi_ce_C_Type);
	cffi_ce_C_Int8->backed_enum_table = (HashTable*)&ffi_type_sint8;

	cffi_ce_C_Char = register_class_C_Char(cffi_ce_C_Int8);
	cffi_ce_C_Char->backed_enum_table = (HashTable*)&ffi_type_schar;

	cffi_ce_C_Int16 = register_class_C_I16(cffi_ce_C_Type);
	cffi_ce_C_Int16->backed_enum_table = (HashTable*)&ffi_type_sint16;

	cffi_ce_C_Int32 = register_class_C_I32(cffi_ce_C_Type);
	cffi_ce_C_Int32->backed_enum_table = (HashTable*)&ffi_type_sint32;

	cffi_ce_C_Int64 = register_class_C_I64(cffi_ce_C_Type);
	cffi_ce_C_Int64->backed_enum_table = (HashTable*)&ffi_type_sint64;

	cffi_ce_C_Float32 = register_class_C_F32(cffi_ce_C_Type);
	cffi_ce_C_Float32->backed_enum_table = (HashTable*)&ffi_type_float;

	cffi_ce_C_Double = register_class_C_F64(cffi_ce_C_Type);
	cffi_ce_C_Double->backed_enum_table = (HashTable*)&ffi_type_double;

#ifdef HAVE_LONG_DOUBLE
	cffi_ce_C_LongDouble = register_class_C_FL(cffi_ce_C_Type);
	cffi_ce_C_LongDouble->backed_enum_table = (HashTable*)&ffi_type_longdouble;
#endif
	return SUCCESS;
}

/* {{{ ZEND_RSHUTDOWN_FUNCTION */
ZEND_RSHUTDOWN_FUNCTION(cffi) {
	printf("ZEND_RSHUTDOWN_FUNCTION\n");
	zend_class_entry *class_ce;
	zend_function* method;
	ZEND_HASH_FOREACH_PTR(cffi_globals.dl_class_table, class_ce) {
		ZEND_HASH_FOREACH_PTR(&class_ce->function_table, method) {
			if (method->common.fn_flags & (ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)) {
				efree(method->internal_function.arg_info - 1);
				efree(method->internal_function.reserved[2]);
				method->common.type = ZEND_USER_FUNCTION;
				method->internal_function.handler = NULL;
				method->common.arg_info = method->internal_function.reserved[0];
				method->internal_function.reserved[0] = NULL;
				method->internal_function.reserved[1] = NULL;
				method->internal_function.reserved[2] = NULL;
			}
		}ZEND_HASH_FOREACH_END();
		class_ce->backed_enum_table = NULL;
	} ZEND_HASH_FOREACH_END();
	return SUCCESS; }
/* }}} */

/* {{{ ZEND_MINFO_FUNCTION */
ZEND_MINFO_FUNCTION(cffi) {
	php_info_print_table_start();
	php_info_print_table_row(2, "CFFI support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ ZEND_GINIT_FUNCTION */
static ZEND_GINIT_FUNCTION(cffi) {
	memset(cffi_globals, 0, sizeof(*cffi_globals));
	cffi_globals->dl_class_table = malloc(sizeof(HashTable));
	zend_hash_init(cffi_globals->dl_class_table, 0, NULL, NULL, 1);
}
/* }}} */

/* {{{ ZEND_GINIT_FUNCTION */
static ZEND_GSHUTDOWN_FUNCTION(cffi) {
	printf("ZEND_GSHUTDOWN_FUNCTION\n");
	zend_hash_destroy(cffi_globals->dl_class_table);
	cffi_ce_C_P1->backed_enum_table = NULL;
	cffi_ce_C_P2->backed_enum_table = NULL;
	cffi_ce_C_P3->backed_enum_table = NULL;
	cffi_ce_C_P4->backed_enum_table = NULL;
	cffi_ce_C_Void->backed_enum_table = NULL;
	cffi_ce_C_Int8->backed_enum_table = NULL;
	cffi_ce_C_Char->backed_enum_table = NULL;
	cffi_ce_C_Float32->backed_enum_table = NULL;
	cffi_ce_C_Double->backed_enum_table = NULL;
	cffi_ce_C_Int16->backed_enum_table = NULL;
	cffi_ce_C_Int32->backed_enum_table = NULL;
	cffi_ce_C_Int64->backed_enum_table = NULL;
#ifdef HAVE_LONG_DOUBLE
	cffi_ce_C_LongDouble->backed_enum_table = NULL;
#endif
}
/* }}} */

/* {{{ cffi_module_entry */
zend_module_entry cffi_module_entry = {STANDARD_MODULE_HEADER,
                                       "CFFI",               /* Extension name */
                                       NULL,                 /* zend_function_entry */
                                       ZEND_MINIT(cffi),     /* PHP_MINIT - Module initialization */
                                       NULL,                 /* PHP_MSHUTDOWN - Module shutdown */
                                       NULL,                 /* PHP_RINIT - Request initialization */
                                       ZEND_RSHUTDOWN(cffi), /* PHP_RSHUTDOWN - Request shutdown */
                                       ZEND_MINFO(cffi),     /* PHP_MINFO - Module info */
                                       PHP_VERSION,          /* Version */
                                       ZEND_MODULE_GLOBALS(cffi),
                                       ZEND_GINIT(cffi),
                                       ZEND_GSHUTDOWN(cffi),
                                       NULL,
                                       STANDARD_MODULE_PROPERTIES_EX};
/* }}} */

#ifdef COMPILE_DL_CFFI
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(cffi)
#endif
