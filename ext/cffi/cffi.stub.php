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

interface __Stdcall {}

#ifdef HAVE_FFI_VECTORCALL_PARTIAL
interface __Vectorcall {}
#endif
#ifdef HAVE_FFI_FASTCALL
interface __Fastcall {}
#endif
#ifdef HAVE_FFI_THISCALL
interface __Thiscall {}
#endif
#ifdef HAVE_FFI_PASCAL
interface __Pascal {}
#endif
#ifdef HAVE_FFI_REGISTER
interface __Register {}
#endif
#ifdef HAVE_FFI_MS_CDECL
interface __MSCdecl {}
#endif
#ifdef HAVE_FFI_SYSV
interface __Sysv {}
#endif

interface DL {
    const string DL_FILE = '';
}
abstract class Type
{
    final public function __construct(int|float|string|array|bool|null|Type $value = null, bool $owned = true, bool  $persistent = false) {}
    final public function isNull(): bool {}
    final public function free(): void {}
    final public function getValue(): int|float|string|array|bool|null {}
    final public function memcmp(Type &$ptr2): int {}
    final public function memcpy(P &$to, int $size): void {}
    final public function memset(int $value, int $size): void {}
    final public function addr(): P {}
    final public function cast(): ?Type {}
    final static public function sizeof(): int {}
    final static public function getAlignment(): int {}
}
abstract class Struct extends Type {
}
abstract class Union extends Type {
}
class CVoid extends Type {
}

class F64 extends Type {

}
class F32 extends Type {

}
#ifdef HAVE_LONG_DOUBLE
class FL extends Type {
}
#endif
class I8 extends Type {
}
class Char extends I8 {
}
class I16 extends Type {
}
class I32 extends Type {
}
class I64 extends Type {
}
class Exception extends \Error {}
