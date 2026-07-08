/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b7b6bd023178b3a2697f0676357606c7f4acec73 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_C_DL___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_isNull, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(1, p, C\\Pointer, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, p, C\\Pointer, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_memcmp, 0, 3, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, ptr1, C\\Pointer, 0)
	ZEND_ARG_OBJ_INFO(1, ptr2, C\\Pointer, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_memcpy, 0, 3, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, to, C\\Pointer, 0)
	ZEND_ARG_OBJ_INFO(1, form, C\\Pointer, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_memset, 0, 3, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, p, C\\Pointer, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_C_DL_addr, 0, 1, C\\Pointer, 0)
	ZEND_ARG_OBJ_INFO(1, p, C\\Type, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_C_DL_alignof, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, p, C\\Type, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_C_DL_sizeof arginfo_class_C_DL_alignof

ZEND_METHOD(C_DL, __construct);
ZEND_METHOD(C_DL, isNull);
ZEND_METHOD(C_DL, free);
ZEND_METHOD(C_DL, memcmp);
ZEND_METHOD(C_DL, memcpy);
ZEND_METHOD(C_DL, memset);
ZEND_METHOD(C_DL, addr);
ZEND_METHOD(C_DL, alignof);
ZEND_METHOD(C_DL, sizeof);

static const zend_function_entry class_C_DL_methods[] = {
	ZEND_ME(C_DL, __construct, arginfo_class_C_DL___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, isNull, arginfo_class_C_DL_isNull, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, free, arginfo_class_C_DL_free, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, memcmp, arginfo_class_C_DL_memcmp, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, memcpy, arginfo_class_C_DL_memcpy, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, memset, arginfo_class_C_DL_memset, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, addr, arginfo_class_C_DL_addr, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, alignof, arginfo_class_C_DL_alignof, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(C_DL, sizeof, arginfo_class_C_DL_sizeof, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_C_Type(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Type", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

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

#if defined(HAVE_FFI_STDCALL)
static zend_class_entry *register_class_C___Stdcall(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Stdcall", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

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

#if defined(HAVE_FFI_UNIX64)
static zend_class_entry *register_class_C___Unix64(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Unix64", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_WIN64)
static zend_class_entry *register_class_C___Win64(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Win64", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

#if defined(HAVE_FFI_GNUW64)
static zend_class_entry *register_class_C___Gnuw64(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "__Gnuw64", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
#endif

static zend_class_entry *register_class_C_DL(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "DL", class_C_DL_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT);

	return class_entry;
}

static zend_class_entry *register_class_C_Struct(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Struct", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_Union(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Union", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_CVoid(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "CVoid", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_F64(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "F64", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_F32(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "F32", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

#if defined(HAVE_LONG_DOUBLE)
static zend_class_entry *register_class_C_FL(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "FL", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}
#endif

static zend_class_entry *register_class_C_I8(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I8", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

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
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_I32(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I32", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_I64(zend_class_entry *class_entry_C_Type)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "I64", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, 0);
	zend_class_implements(class_entry, 1, class_entry_C_Type);

	return class_entry;
}

static zend_class_entry *register_class_C_Exception(zend_class_entry *class_entry_Error)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "C", "Exception", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Error, 0);

	return class_entry;
}
