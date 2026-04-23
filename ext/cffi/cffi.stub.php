<?php

/**
 * @generate-class-entries
 * @undocumentable
 */

namespace C;
interface Pointer {}
interface P1 extends Pointer
{
    const int LEVEL = 1;
}
interface P2 extends Pointer
{
    const int LEVEL = 2;
}
interface P3 extends Pointer
{
    const int LEVEL = 3;
}
interface P4 extends Pointer
{
    const int LEVEL = 4;
}

interface CArray
{
    const array SIZE = [];
}
interface Unsigned {}

#ifdef HAVE_FFI_STDCALL
interface __Stdcall {
}
#endif

#ifdef HAVE_FFI_VECTORCALL_PARTIAL
interface __Vectorcall {
}
#endif
#ifdef HAVE_FFI_FASTCALL
interface __Fastcall {
}
#endif
#ifdef HAVE_FFI_THISCALL
interface __Thiscall {
}
#endif
#ifdef HAVE_FFI_PASCAL
interface __Pascal {
}
#endif
#ifdef HAVE_FFI_REGISTER
interface __Register {}
#endif
#ifdef HAVE_FFI_MS_CDECL
interface __MSCdecl {
}
#endif
#ifdef HAVE_FFI_SYSV
interface __Sysv {
}
#endif

#ifdef HAVE_FFI_UNIX64
interface __Unix64 {
}
#endif

#ifdef HAVE_FFI_WIN64
interface __Win64 {
}
#endif

#ifdef HAVE_FFI_GNUW64
interface __Gnuw64 {
}
#endif

abstract class DL {
    final public function __construct()
    {
    }
    final static public function isNull(Pointer &$p): bool {}
    final static public function free(Pointer &$p): void {}
    final static public function memcmp(Pointer &$ptr1, Pointer &$ptr2, int $size): int {}
    final static public function memcpy(Pointer &$to, Pointer &$form, int $size): void {}
    final static public function memset(Pointer &$p, int $value, int $size): void {}
    final static public function addr(Type &$p): Pointer {}
    final static public function alignof(Type &$p): int {}
    final static public function sizeof(Type &$p): int {}
}
interface  Type
{

}
abstract class Struct implements Type {
}
abstract class Union implements Type {
}
abstract class CVoid implements Type {

}

class F64 implements Type {
}
class F32 implements Type {
}
#ifdef HAVE_LONG_DOUBLE
class FL implements Type {
}
#endif
class I8 implements Type {
}
class Char extends I8 {
}
class I16 implements Type {
}
class I32 implements Type {
}
class I64 implements Type {
}
class Exception extends \Error {}
