/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0491a04000004ad94a6dd256ed1f65d239a051ce */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_C_Type___construct, 0, 0, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, value, C\\Type, MAY_BE_LONG|MAY_BE_DOUBLE|MAY_BE_STRING|MAY_BE_ARRAY|MAY_BE_BOOL|MAY_BE_NULL, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, owned, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, persistent, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_isNull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_free, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_C_Type_getValue, 0, 0, MAY_BE_LONG|MAY_BE_DOUBLE|MAY_BE_STRING|MAY_BE_ARRAY|MAY_BE_BOOL|MAY_BE_NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_memcmp, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, ptr2, C\\Type, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_memcpy, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, to, C\\P, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_memset, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_C_Type_addr, 0, 0, C\\P, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_C_Type_cast, 0, 0, C\\Type, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_Type_sizeof, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_C_Type_getAlignment arginfo_class_C_Type_sizeof

ZEND_METHOD(C_Type, __construct);
ZEND_METHOD(C_Type, isNull);
ZEND_METHOD(C_Type, free);
ZEND_METHOD(C_Type, getValue);
ZEND_METHOD(C_Type, memcmp);
ZEND_METHOD(C_Type, memcpy);
ZEND_METHOD(C_Type, memset);
ZEND_METHOD(C_Type, addr);
ZEND_METHOD(C_Type, cast);
ZEND_METHOD(C_Type, sizeof);
ZEND_METHOD(C_Type, getAlignment);

static const zend_function_entry class_C_Type_methods[] = {
	ZEND_ME(C_Type, __construct, arginfo_class_C_Type___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, isNull, arginfo_class_C_Type_isNull, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, free, arginfo_class_C_Type_free, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, getValue, arginfo_class_C_Type_getValue, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, memcmp, arginfo_class_C_Type_memcmp, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, memcpy, arginfo_class_C_Type_memcpy, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, memset, arginfo_class_C_Type_memset, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, addr, arginfo_class_C_Type_addr, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, cast, arginfo_class_C_Type_cast, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, sizeof, arginfo_class_C_Type_sizeof, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_Type, getAlignment, arginfo_class_C_Type_getAlignment, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_C_Pointer(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Pointer", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

static zend_class_entry *register_class_C_P1(zend_class_entry *class_entry_C_Pointer)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "P1", NULL);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_C_Pointer);

	zval const_LEVEL_value;
	ZVAL_LONG(&const_LEVEL_value, 1);
	zend_string *const_LEVEL_name = zend_string_init_interned("LEVEL", sizeof("LEVEL") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_name, &const_LEVEL_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_name);

	return class_entry;
}

static zend_class_entry *register_class_C_P2(zend_class_entry *class_entry_C_Pointer)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "P2", NULL);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_C_Pointer);

	zval const_LEVEL_value;
	ZVAL_LONG(&const_LEVEL_value, 2);
	zend_string *const_LEVEL_name = zend_string_init_interned("LEVEL", sizeof("LEVEL") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_name, &const_LEVEL_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_name);

	return class_entry;
}

static zend_class_entry *register_class_C_P3(zend_class_entry *class_entry_C_Pointer)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "P3", NULL);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_C_Pointer);

	zval const_LEVEL_value;
	ZVAL_LONG(&const_LEVEL_value, 3);
	zend_string *const_LEVEL_name = zend_string_init_interned("LEVEL", sizeof("LEVEL") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_name, &const_LEVEL_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_name);

	return class_entry;
}

static zend_class_entry *register_class_C_P4(zend_class_entry *class_entry_C_Pointer)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "P4", NULL);
	class_entry = zend_register_internal_interface(&ce);
	zend_class_implements(class_entry, 1, class_entry_C_Pointer);

	zval const_LEVEL_value;
	ZVAL_LONG(&const_LEVEL_value, 4);
	zend_string *const_LEVEL_name = zend_string_init_interned("LEVEL", sizeof("LEVEL") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_name, &const_LEVEL_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_name);

	return class_entry;
}

static zend_class_entry *register_class_C_CArray(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "CArray", NULL);
	class_entry = zend_register_internal_interface(&ce);

	zval const_SIZE_value;
	ZVAL_EMPTY_ARRAY(&const_SIZE_value);
	zend_string *const_SIZE_name = zend_string_init_interned("SIZE", sizeof("SIZE") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_SIZE_name, &const_SIZE_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(const_SIZE_name);

	return class_entry;
}

static zend_class_entry *register_class_C_Unsigned(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Unsigned", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

static zend_class_entry *register_class_C___Stdcall(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Stdcall", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

#if defined(HAVE_FFI_VECTORCALL_PARTIAL)
static zend_class_entry *register_class_C___Vectorcall(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Vectorcall", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_FASTCALL)
static zend_class_entry *register_class_C___Fastcall(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Fastcall", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_THISCALL)
static zend_class_entry *register_class_C___Thiscall(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Thiscall", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_PASCAL)
static zend_class_entry *register_class_C___Pascal(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Pascal", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_REGISTER)
static zend_class_entry *register_class_C___Register(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Register", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_MS_CDECL)
static zend_class_entry *register_class_C___MSCdecl(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__MSCdecl", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_SYSV)
static zend_class_entry *register_class_C___Sysv(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Sysv", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

static zend_class_entry *register_class_C_DL(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "DL", NULL);
	class_entry = zend_register_internal_interface(&ce);

	zval const_DL_FILE_value;
	ZVAL_EMPTY_STRING(&const_DL_FILE_value);
	zend_string *const_DL_FILE_name = zend_string_init_interned("DL_FILE", sizeof("DL_FILE") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_DL_FILE_name, &const_DL_FILE_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(const_DL_FILE_name);

	return class_entry;
}

static zend_class_entry *register_class_C_Type(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Type", class_C_Type_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT);

	return class_entry;
}

static zend_class_entry *register_class_C_Struct(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Struct", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, ZEND_ACC_ABSTRACT);

	return class_entry;
}

static zend_class_entry *register_class_C_Union(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Union", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, ZEND_ACC_ABSTRACT);

	return class_entry;
}

static zend_class_entry *register_class_C_CVoid(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "CVoid", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_F64(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "F64", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_F32(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "F32", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

#if defined(HAVE_LONG_DOUBLE)
static zend_class_entry *register_class_C_FL(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "FL", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}
#endif

static zend_class_entry *register_class_C_I8(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I8", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_Char(zend_class_entry *class_entry_C_I8)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Char", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_I8, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_I16(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I16", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_I32(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I32", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_I64(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I64", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_C_Type, 0);

	return class_entry;
}

static zend_class_entry *register_class_C_Exception(zend_class_entry *class_entry_Error)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Exception", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Error, 0);

	return class_entry;
}
