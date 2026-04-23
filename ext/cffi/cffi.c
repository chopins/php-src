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

#include <ffi.h>
#include <sys/types.h>

#include "cffi_arginfo.h"
#include "ext/standard/info.h"
#include "main/SAPI.h"
#include "zend_API.h"
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
typedef struct _cffi_dl_object {
	zend_object std;
	ffi_abi abi;
	HashTable* variable;
	bool is_init;
} cffi_dl_object;
typedef struct _cffi_type_object {
	zend_object std;
	int type;
	union {
		void* p;
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
// #ifdef HAVE_FFI_STDCALL
static zend_class_entry* cffi_ce_C_ABI_Stdcall;
// #endif
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
#ifdef HAVE_FFI_UNIX64
static zend_class_entry* cffi_ce_C_ABI_Unix64;
#endif
#ifdef HAVE_FFI_WIN64
static zend_class_entry* cffi_ce_C_ABI_Win64;
#endif
#ifdef HAVE_FFI_GNUW64
static zend_class_entry* cffi_ce_C_ABI_Gnuw64;
#endif

static zend_class_entry* cffi_ce_C_Unsigned;
static zend_class_entry* cffi_ce_C_Pointer;
static zend_class_entry* cffi_ce_C_P1;
static zend_class_entry* cffi_ce_C_P2;
static zend_class_entry* cffi_ce_C_P3;
static zend_class_entry* cffi_ce_C_P4;
static zend_class_entry* cffi_ce_C_CVoid;
static zend_class_entry* cffi_ce_C_Char;
static zend_class_entry* cffi_ce_C_F32;
static zend_class_entry* cffi_ce_C_F64;
static zend_class_entry* cffi_ce_C_I8;
static zend_class_entry* cffi_ce_C_I16;
static zend_class_entry* cffi_ce_C_I32;
static zend_class_entry* cffi_ce_C_I64;
#ifdef HAVE_LONG_DOUBLE
static zend_class_entry* cffi_ce_C_FL;
#endif

static zend_object_handlers cffi_dl_handlers;
static zend_object_handlers cffi_type_handlers;

#define CFFI_P(var) printf("Pname:" #var ";Addr: %lx\n", var);

#define CFFI_IMP_REQUIRE_ERROR(error, tname) \
	zend_error_at_noreturn(E_COMPILE_ERROR, class_type->info.user.filename, class_type->info.user.line_start, error, ZSTR_VAL(class_type->name), tname)

#define CFFI_THROWS(error, ...) zend_throw_error(cffi_ce_C_Exception, error, __VA_ARGS__);

#define CFFI_FN_VAL(func, name) func->internal_function.name
#define CFFI_EX_FN_VAL(name) CFFI_FN_VAL(EX(func), name)
#define CFFI_EX_FN_ARGC CFFI_EX_FN_VAL(num_args)

/* C\DL and ABI interface: zend_class_entry.enum_backing_type is FFI_ABI */
#define CFFI_CE_EXT_TYPE(ce) ce->enum_backing_type

static zend_always_inline ffi_type* cffi_get_ffi_type(zend_class_entry *ce) {
	switch (CFFI_CE_EXT_TYPE(ce)) {
		case FFI_TYPE_SINT8:
			return &ffi_type_sint8;
		case FFI_TYPE_SINT16:
			return &ffi_type_sint16;
		case FFI_TYPE_SINT32:
			return &ffi_type_sint32;
		case FFI_TYPE_SINT64:
			return &ffi_type_sint64;
		case FFI_TYPE_UINT8:
			return &ffi_type_uint8;
		case FFI_TYPE_UINT16:
			return &ffi_type_uint16;
		case FFI_TYPE_UINT32:
			return &ffi_type_uint32;
		case FFI_TYPE_UINT64:
			return &ffi_type_uint64;
		case FFI_TYPE_FLOAT:
			return &ffi_type_float;
		case FFI_TYPE_DOUBLE:
			return &ffi_type_double;
		case FFI_TYPE_POINTER:
			return &ffi_type_pointer;
		case FFI_TYPE_LONGDOUBLE:
			return &ffi_type_longdouble;
		case FFI_TYPE_VOID:
			return &ffi_type_void;
		default:
			return NULL;
	}
}

#define CFFI_CE_TYPE_SET(ce, type) CFFI_CE_EXT_TYPE(ce) = type
#define CFFI_CE_TYPE_CHECK(ce, type) cffi_get_ffi_type(ce) == &type
#define CFFI_CE_TYPE_COPY(ce1, ce2) CFFI_CE_EXT_TYPE(ce1) = CFFI_CE_EXT_TYPE(ce2);

#define CFFI_REG_FFI_TYPE(cename, pce, type)             \
	cffi_ce_C_##cename = register_class_C_##cename(pce); \
	CFFI_CE_EXT_TYPE(cffi_ce_C_##cename) = FFI_TYPE_##type; \

#define CFFI_REG_FFI_TYPE_H(cename, pce, type)	\
	CFFI_REG_FFI_TYPE(cename, pce, type)	\
	cffi_ce_C_##cename->create_object = cffi_type_new;	\
	cffi_ce_C_##cename->default_object_handlers = &cffi_type_handlers;

#define CFFI_REG_ABI_INTERFACE(class, abi)                                              \
	cffi_ce_C_ABI_##class = register_class_C___##class();                               \
	cffi_ce_C_ABI_##class->interface_gets_implemented = cffi_implement_c_func_abi_type; \
	CFFI_CE_EXT_TYPE(cffi_ce_C_ABI_##class) = FFI_##abi;

static int cffi_early_class_implements_interface(zend_class_entry* class_type, zend_class_entry* interface_ce) {
	uint32_t i;
	ZEND_ASSERT(interface_ce->ce_flags & ZEND_ACC_INTERFACE);
	uint32_t n = 0;
	if (class_type->num_interfaces) {
		uint32_t arr[class_type->num_interfaces];
		ZEND_ASSERT(class_type->ce_flags & ZEND_ACC_RESOLVED_INTERFACES);
		for (i = 0; i < class_type->num_interfaces; i++) {
			if (class_type->interfaces[i] == interface_ce) {
				return 1;
			} else if(class_type->interfaces[i]->num_interfaces) {
				arr[n] = i;
				n++;
			}
		}

		if(n > 0) {
			for(int k=0;k< n;k++) {
				if(cffi_early_class_implements_interface(class_type->interfaces[arr[k]], interface_ce)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

static int cffi_implement_check_require(zend_class_entry* interface, zend_class_entry* class_type) /* {{{ */
{
	if(class_type->type == ZEND_INTERNAL_CLASS) {
		return SUCCESS;
	}
	if(interface == cffi_ce_C_Type) {
		if(class_type->parent == NULL || !zend_class_implements_interface(class_type->parent, cffi_ce_C_Type)) {
			CFFI_IMP_REQUIRE_ERROR("User class %s cannot implement %s", ZSTR_VAL(cffi_ce_C_Type->name));
			return FAILURE;
		}

		if(!cffi_early_class_implements_interface(class_type, cffi_ce_C_Pointer) && instanceof_function(class_type, cffi_ce_C_CVoid)) {
			CFFI_IMP_REQUIRE_ERROR("Void class %s must implements interface %s", ZSTR_VAL(cffi_ce_C_Pointer->name));
			return FAILURE;
		}
		return SUCCESS;
	}

	if ((interface == cffi_ce_C_CArray || interface == cffi_ce_C_Pointer) && !cffi_early_class_implements_interface(class_type, cffi_ce_C_Type)) {
		CFFI_IMP_REQUIRE_ERROR("Class %s must first inherit a class that implements interface %s", ZSTR_VAL(cffi_ce_C_Type->name));
		return FAILURE;
	}

	if (interface == cffi_ce_C_Unsigned) {
		if (!EXPECTED(CFFI_CE_EXT_TYPE(class_type) >= 0)) {
			return FAILURE;
		};
		ffi_type *ftype = cffi_get_ffi_type(class_type);
		if (ftype == &ffi_type_sint8) {
			CFFI_CE_TYPE_SET(class_type, FFI_TYPE_UINT8);
		} else if (ftype == &ffi_type_sint16) {
			CFFI_CE_TYPE_SET(class_type, FFI_TYPE_UINT16);
		} else if (ftype == &ffi_type_sint32) {
			CFFI_CE_TYPE_SET(class_type, FFI_TYPE_UINT32);
		} else if (ftype == &ffi_type_sint64) {
			CFFI_CE_TYPE_SET(class_type, FFI_TYPE_UINT64);
		} else {
			return FAILURE;
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
	if (interface == cffi_ce_C_Pointer) {
		if (Z_TYPE(const_val->value) != IS_LONG) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be an integer", const_name);
			return FAILURE;
		}
		if (Z_LVAL(const_val->value) <= 0) {
			CFFI_IMP_REQUIRE_ERROR("Const %s::%s value must be greater 0", const_name);
			return FAILURE;
		}
		CFFI_CE_TYPE_SET(class_type, FFI_TYPE_POINTER);
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
	zend_class_entry* ce;
	if (ZEND_TYPE_IS_COMPLEX(*type)) {
		if (UNEXPECTED(ZEND_TYPE_HAS_LIST(*type))) {
			zend_type* list_type;
			if (ZEND_TYPE_LIST(*type)->num_types > 2) {
				return NULL;
			}
			ZEND_TYPE_LIST_FOREACH(ZEND_TYPE_LIST(*type), list_type) {
				ce = zend_lookup_class(ZEND_TYPE_NAME(*type));
				if (UNEXPECTED(!ce)) {
					return NULL;
				}
				if (zend_class_implements_interface(ce, cffi_ce_C_Pointer)) {
					return &ffi_type_pointer;
				}
			}
			ZEND_TYPE_LIST_FOREACH_END();
			return cffi_get_ffi_type(ce);
		} else if (ZEND_TYPE_HAS_NAME(*type)) {
			ce = zend_lookup_class(ZEND_TYPE_NAME(*type));
			if (UNEXPECTED(!ce)) {
				return NULL;
			}
			if (!zend_class_implements_interface(ce, cffi_ce_C_Type)) {
				return NULL;
			}
			return cffi_get_ffi_type(ce);
		}
	} else {
		uint32_t type_mask = ZEND_TYPE_FULL_MASK(*type);
		if (type_mask & MAY_BE_STRING) {
			return &ffi_type_pointer;
		} else if (type_mask & MAY_BE_LONG) {
			return &ffi_type_sint64;
		} else if (type_mask & MAY_BE_DOUBLE) {
			return &ffi_type_double;
		} else if (type_mask & MAY_BE_REF) {
			return &ffi_type_pointer;
		}
	}
	return NULL;
}

static int cffi_implement_c_func_abi_type(zend_class_entry* interface, zend_class_entry* class_type) {
	CFFI_CE_TYPE_COPY(class_type, interface);
	return SUCCESS;
}

static zend_object* cffi_c_dl_new(zend_class_entry* class_type) {
	cffi_dl_object* dl;
	dl = zend_hash_find_ptr(cffi_globals.dl_class_table, class_type->name);
	if(dl != NULL) {
		return &dl->std;
	}

	dl = zend_object_alloc(sizeof(cffi_dl_object), class_type);
	zend_object_std_init(&dl->std, class_type);
	object_properties_init(&dl->std, class_type);
	dl->abi = (int)CFFI_CE_EXT_TYPE(class_type);
	dl->variable = NULL;
	dl->is_init = false;
	return &dl->std;
}

zval* cffi_api_read_property(zend_object* object, zend_string* member, int type, void** cache_slot, zval* rv) {}
zval* cffi_api_write_property(zend_object* object, zend_string* member, zval* value, void** cache_slot) {}


static zend_object* cffi_type_new(zend_class_entry* class_type) /* {{{ */
{
	cffi_type_object* ctype;
	printf("New Class: %s\n", ZSTR_VAL(class_type->name));
	ctype = zend_object_alloc(sizeof(cffi_type_object), class_type);
	zend_object_std_init(&ctype->std, class_type);
	object_properties_init(&ctype->std, class_type);
	if (zend_class_implements_interface(class_type, cffi_ce_C_Pointer)) {
		ctype->type = FFI_TYPE_POINTER;
	} else if (instanceof_function(class_type, cffi_ce_C_Struct)) {
		ctype->type = FFI_TYPE_STRUCT;
	} else if (instanceof_function(class_type, cffi_ce_C_Union)) {
		ctype->type = FFI_TYPE_STRUCT;
	}
	if(zend_class_implements_interface(class_type, cffi_ce_C_CArray)) {

	}

	return &ctype->std;
}
/* }}} */

static void zend_ffi_pass_arg(void** arg_values, uint32_t n) { /* {{{ */
	printf("func ptr:%ld\n", arg_values[n]);
}

/* }}} */
#define ZEND_FFI_SIZEOF_ARG MAX(FFI_SIZEOF_ARG, sizeof(double))

static ZEND_NAMED_FUNCTION(cffi_call_api_trampoline) /* {{{ */
{
	// zend_long --> int64 int32 int16 int8
	// zend_string ---> char* pointer
	// zend_float ---> double
	// C\Type object --> pointer
	printf("CALL -> cffi_call_api_trampoline\n");
	uint32_t max_args = CFFI_EX_FN_VAL(fn_flags) & ZEND_ACC_VARIADIC ? EX_NUM_ARGS() : CFFI_EX_FN_ARGC;
	uint32_t func_abi = CFFI_CE_EXT_TYPE(CFFI_EX_FN_VAL(scope));
	zend_object* arg;
	void* addr = CFFI_EX_FN_VAL(reserved)[1];
	ffi_type** arg_types = CFFI_EX_FN_VAL(reserved)[2];
	ffi_status status;
	ffi_cif cif;
	void* ret;
	void** arg_values = do_alloca(sizeof(void*) * EX_NUM_ARGS(), cffi_args_use_heap);

	if (UNEXPECTED(EX_NUM_ARGS() < CFFI_EX_FN_ARGC)) {
		zend_wrong_parameters_count_error(CFFI_EX_FN_ARGC, -1);
		return;
	}

	printf("arg ptr S : %ld  Size:%d\n", (char*)arg_values, (sizeof(void*) + ZEND_FFI_SIZEOF_ARG) * EX_NUM_ARGS());
	for (uint32_t n = 0; n < EX_NUM_ARGS(); n++) {
		arg_values[n] = ((char*)arg_values) + (sizeof(void*) * EX_NUM_ARGS()) + (ZEND_FFI_SIZEOF_ARG * n);
		printf("arg ptr %d : %ld  Offset:%d, paddr:%d\n", n, arg_values[n], (sizeof(void*) * EX_NUM_ARGS()) + (ZEND_FFI_SIZEOF_ARG * n), (int64_t)(arg_values + n * 8));
		zend_ffi_pass_arg(arg_values, n);
	}

	zend_string* fname = CFFI_EX_FN_VAL(function_name);
	zend_class_entry* cls = CFFI_EX_FN_VAL(scope);
	printf("CALL: cffi_call_api_trampoline: class:%s, fname:%s\n", ZSTR_VAL(cls->name), ZSTR_VAL(fname));

	if ((status = ffi_prep_cif(&cif, func_abi, 2, arg_types[0], arg_types + 1)) != FFI_OK) {
		zend_throw_error(cffi_ce_C_Exception, "Failed call '%s' (%d)", ZSTR_VAL(fname), status);
		efree(arg_values);
		RETURN_THROWS();
	}
	efree(arg_values);
	// ffi_call(&cif, addr, ret, arg_values);
	RETURN_NULL();
	// RETURN_STR(fname);
}
/* }}} */

static void cffi_func_restore_var_destory(cffi_dl_object* dl)
{
	zend_function* method;
	ZEND_HASH_FOREACH_PTR(&(dl->std.ce->function_table), method) {
		if(method->common.fn_flags & ZEND_ACC_STATIC) {
			continue;
		}
		if(!(method->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) {
			continue;
		}
		if (method->common.type & ZEND_INTERNAL_FUNCTION) {
			efree(method->internal_function.arg_info - 1);
			efree(method->internal_function.reserved[2]);
			method->common.type = ZEND_USER_FUNCTION;
			method->internal_function.handler = NULL;
			method->common.arg_info = method->internal_function.reserved[0];
			method->internal_function.reserved[0] = NULL;
			method->internal_function.reserved[1] = NULL;
			method->internal_function.reserved[2] = NULL;
		}
	}
	ZEND_HASH_FOREACH_END();
	if(dl->variable != NULL) {
		zend_array_destroy(dl->variable);
	}
}

ZEND_METHOD(C_Type, __construct) {

}

ZEND_METHOD(C_DL, __construct) {
	DL_HANDLE handle = NULL;
	char* err;
	char* path = NULL;
	size_t pathlen;
	zend_function* method;

	ZEND_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_PATH(path, pathlen)
	ZEND_PARSE_PARAMETERS_END();

	cffi_dl_object* dl = (cffi_dl_object*)Z_OBJ_P(getThis());
	if(dl->is_init) {
		return;
	}
	zend_class_entry* ce = dl->std.ce;

	if (path) {
		handle = DL_LOAD(path);
		if(!handle) {
			err = GET_DL_ERROR();
#ifdef PHP_WIN32
			if (err && err[0]) {
				CFFI_THROWS("Failed loading '%s' (%s)", path, err);
				php_win32_error_msg_free(err);
			} else {
				CFFI_THROWS("Failed loading '%s' (Unknown reason)", path)
			}
#else
			CFFI_THROWS("Failed loading '%s' (%s)", path, err);
			GET_DL_ERROR(); /* free the buffer storing the error */
#endif
			RETURN_THROWS();
		}
#ifdef RTLD_DEFAULT
	} else {
		handle = RTLD_DEFAULT;
#endif
	}

	if (ce->default_properties_count > 0) {
		dl->variable = zend_new_array(0);
		zend_property_info* prop;
		zend_string* key;
		ZEND_HASH_MAP_FOREACH_STR_KEY_PTR(&ce->properties_info, key, prop){
			if(prop->flags & ZEND_ACC_STATIC) {
				continue;
			}
			if (prop->flags & ZEND_ACC_PUBLIC) {
				void* var = DL_FETCH_SYMBOL(handle, ZSTR_VAL(key));
				if (!var) {
					CFFI_THROWS("Class %s can not resolving C variable '%s'", ZSTR_VAL(ce->name), ZSTR_VAL(key))
					zend_array_destroy(dl->variable);
					RETURN_THROWS();
				}
				zend_hash_add_ptr(dl->variable, key, var);
			}
		} ZEND_HASH_FOREACH_END();
	}

	ZEND_HASH_FOREACH_PTR(&ce->function_table, method) {
		if(method->common.fn_flags & ZEND_ACC_STATIC) {
			continue;
		}

		if (method->common.type == ZEND_USER_FUNCTION && method->common.fn_flags & ZEND_ACC_PUBLIC) {
			void* addr = DL_FETCH_SYMBOL(handle, ZSTR_VAL(method->common.function_name));
			if (!addr) {
				CFFI_THROWS("Class %s can not resolving C function '%s'",ZSTR_VAL(ce->name), ZSTR_VAL(method->common.function_name))
				cffi_func_restore_var_destory(dl);
				RETURN_THROWS();
			}

			method->common.type = ZEND_INTERNAL_FUNCTION;
			method->common.fn_flags = (method->common.fn_flags | ZEND_ACC_CALL_VIA_TRAMPOLINE);
			method->internal_function.handler = cffi_call_api_trampoline;
			zend_arg_info* arg_ptr = method->common.arg_info - 1;
			zend_internal_arg_info* args = do_alloca(sizeof(zend_internal_arg_info), cffi_arginfo_use_heap);
			uint32_t num_args = method->common.num_args + 1;
			if (method->common.fn_flags & ZEND_ACC_VARIADIC) {
				num_args++;
			}

			zend_internal_arg_info* internal_arg_info = ecalloc(num_args + 1, sizeof(zend_internal_arg_info));
			ffi_type** args_ffi_type = emalloc(sizeof(ffi_type*) * num_args);
			uint32_t i;
			for (i = 0; i < num_args; i++) {
				args->name = ZSTR_VAL(arg_ptr[i].name);
				args->type = arg_ptr[i].type;
				args->default_value = ZSTR_VAL(arg_ptr[i].default_value);
				internal_arg_info[i] = *args;
				args_ffi_type[i] = cffi_ctype_ce_set_ffi_type(&arg_ptr[i].type);
			}
			free_alloca(args, cffi_arginfo_use_heap);
			method->internal_function.arg_info = internal_arg_info + 1;
			method->internal_function.reserved[0] = arg_ptr + 1;
			method->internal_function.reserved[1] = addr;
			method->internal_function.reserved[2] = args_ffi_type;
		}
	} ZEND_HASH_FOREACH_END();
	dl->is_init = true;
	zend_hash_add_ptr(cffi_globals.dl_class_table, ce->name, dl);
}


ZEND_METHOD(C_DL, isNull) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, free) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, memcmp) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, memcpy) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, memset) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, addr) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, sizeof) { RETURN_BOOL(1); }

ZEND_METHOD(C_DL, alignof) { RETURN_BOOL(1); }

ZEND_API zend_observer_fcall_handlers cffi_execute_get_c_defined(zend_execute_data* execute_data) {
	char* fn;
	printf("get c defined for observer\n");
	if (EX_CALL_KIND()) {
		printf("execute top code\n");
	}
	return (zend_observer_fcall_handlers){NULL, NULL};
}

ZEND_MINIT_FUNCTION(cffi) {
	// zend_observer_fcall_register(cffi_execute_get_c_defined);
	cffi_globals.is_cli = strcmp(sapi_module.name, "cli") == 0;

	cffi_ce_C_Exception = register_class_C_Exception(zend_ce_error);
	cffi_ce_C_DL = register_class_C_DL();
	CFFI_CE_EXT_TYPE(cffi_ce_C_DL) = FFI_DEFAULT_ABI;

	cffi_ce_C_DL->create_object = cffi_c_dl_new;

	cffi_ce_C_DL->default_object_handlers = &cffi_dl_handlers;
	memcpy(&cffi_dl_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	cffi_dl_handlers.read_property = cffi_api_read_property;
	cffi_dl_handlers.write_property = cffi_api_write_property;

#ifdef HAVE_FFI_STDCALL
	CFFI_REG_ABI_INTERFACE(Stdcall, STDCALL);
#endif
#ifdef HAVE_FFI_VECTORCALL_PARTIAL
	CFFI_REG_ABI_INTERFACE(Vectorcall, VECTORCALL_PARTIAL);
#endif
#ifdef HAVE_FFI_FASTCALL
	CFFI_REG_ABI_INTERFACE(Fastcall, FASTCALL);
#endif
#ifdef HAVE_FFI_THISCALL
	CFFI_REG_ABI_INTERFACE(Thiscall, THISCALL);
#endif
#ifdef HAVE_FFI_PASCAL
	CFFI_REG_ABI_INTERFACE(Pascal, PASCAL);
#endif
#ifdef HAVE_FFI_REGISTER
	CFFI_REG_ABI_INTERFACE(Register, REGISTER);
#endif
#ifdef HAVE_FFI_MS_CDECL
	CFFI_REG_ABI_INTERFACE(MSCdecl, MS_CDECL);
#endif
#ifdef HAVE_FFI_SYSV
	CFFI_REG_ABI_INTERFACE(Sysv, SYSV);
#endif

#ifdef HAVE_FFI_UNIX64
	CFFI_REG_ABI_INTERFACE(Unix64, UNIX64);
#endif

#ifdef HAVE_FFI_WIN64
	CFFI_REG_ABI_INTERFACE(Win64, WIN64);
#endif

#ifdef HAVE_FFI_GNUW64
	CFFI_REG_ABI_INTERFACE(Gnuw64, GNUW64);
#endif
	cffi_ce_C_CArray = register_class_C_CArray();
	cffi_ce_C_CArray->interface_gets_implemented = cffi_implement_check_require;

	cffi_ce_C_Unsigned = register_class_C_Unsigned();
	cffi_ce_C_Unsigned->interface_gets_implemented = cffi_implement_check_require;
	cffi_ce_C_Pointer = register_class_C_Pointer();
	cffi_ce_C_Pointer->interface_gets_implemented = cffi_implement_check_require;

	CFFI_REG_FFI_TYPE(P1, cffi_ce_C_Pointer, POINTER);
	CFFI_REG_FFI_TYPE(P2, cffi_ce_C_Pointer, POINTER);
	CFFI_REG_FFI_TYPE(P3, cffi_ce_C_Pointer, POINTER);
	CFFI_REG_FFI_TYPE(P4, cffi_ce_C_Pointer, POINTER);

	cffi_ce_C_Type = register_class_C_Type();
	cffi_ce_C_Type->interface_gets_implemented = cffi_implement_check_require;

	CFFI_REG_FFI_TYPE_H(Struct, cffi_ce_C_Type, STRUCT);
	memcpy(&cffi_type_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	// cffi_type_handlers.get_constructor = cffi_type_constructor;
	// // cffi_type_handlers.write_property = cffi_type_write_property;
	// // cffi_type_handlers.unset_property = cffi_type_unset_property;
	// // cffi_type_handlers.free_obj = cffi_type_free_obj;
	// // cffi_type_handlers.clone_obj = cffi_type_clone_obj;
	// // cffi_type_handlers.cast_object = cffi_type_cast_obj;
	// // cffi_type_handlers.do_operation = cffi_type_do_operation;
	// // cffi_type_handlers.compare = cffi_type_compare;

	CFFI_REG_FFI_TYPE_H(Union, cffi_ce_C_Type, STRUCT);
	CFFI_REG_FFI_TYPE_H(CVoid, cffi_ce_C_Type, VOID);
	CFFI_REG_FFI_TYPE_H(I8, cffi_ce_C_Type, SINT8);
	CFFI_REG_FFI_TYPE_H(Char, cffi_ce_C_I8, SINT8);
	CFFI_REG_FFI_TYPE_H(I16, cffi_ce_C_Type, SINT16);
	CFFI_REG_FFI_TYPE_H(I32, cffi_ce_C_Type, SINT32);
	CFFI_REG_FFI_TYPE_H(I64, cffi_ce_C_Type, SINT64);
	CFFI_REG_FFI_TYPE_H(F32, cffi_ce_C_Type, FLOAT);
	CFFI_REG_FFI_TYPE_H(F64, cffi_ce_C_Type, DOUBLE);
#ifdef HAVE_LONG_DOUBLE
	CFFI_REG_FFI_TYPE_H(FL, cffi_ce_C_Type, LONGDOUBLE);
#endif
	return SUCCESS;
}

/* {{{ ZEND_RSHUTDOWN_FUNCTION */
ZEND_RSHUTDOWN_FUNCTION(cffi) {
	printf("ZEND_RSHUTDOWN_FUNCTION begin\n");

	cffi_dl_object * dl;
	zend_function* method;
	ZEND_HASH_FOREACH_PTR(cffi_globals.dl_class_table, dl) {
		cffi_func_restore_var_destory(dl);
	}
	ZEND_HASH_FOREACH_END();
	printf("ZEND_RSHUTDOWN_FUNCTION end\n");
	return SUCCESS;
}
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
	zend_hash_destroy(cffi_globals->dl_class_table);
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
