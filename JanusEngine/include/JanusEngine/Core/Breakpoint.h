#pragma once

#if defined(__has_builtin) && !defined(__ibmxl__)
#if __has_builtin(__builtin_debugtrap)
#define JN_BREAKPOINT() __builtin_debugtrap()
#elif __has_builtin(__debugbreak)
#define JN_BREAKPOINT() __debugbreak()
#endif
#endif
#if !defined(JN_BREAKPOINT)
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define JN_BREAKPOINT() __debugbreak()
#elif defined(__ARMCC_VERSION)
#define JN_BREAKPOINT() __breakpoint(42)
#elif defined(__ibmxl__) || defined(__xlC__)
#include <builtins.h>
#define JN_BREAKPOINT() __trap(42)
#elif defined(__DMC__) && defined(_M_IX86)
static inline void JN_BREAKPOINT(void) { __asm int 3h; }
#elif defined(__i386__) || defined(__x86_64__)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__("int3"); }
#elif defined(__thumb__)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__(".inst 0xde01"); }
#elif defined(__aarch64__)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__(".inst 0xd4200000"); }
#elif defined(__arm__)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__(".inst 0xe7f001f0"); }
#elif defined(__alpha__) && !defined(__osf__)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__("bpt"); }
#elif defined(_54_)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__("ESTOP"); }
#elif defined(_55_)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__(";\n .if (.MNEMONIC)\n ESTOP_1\n .else\n ESTOP_1()\n .endif\n NOP"); }
#elif defined(_64P_)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__("SWBP 0"); }
#elif defined(_6x_)
static inline void JN_BREAKPOINT(void) { __asm__ __volatile__("NOP\n .word 0x10000000"); }
#elif defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 0) && defined(__GNUC__)
#define JN_BREAKPOINT() __builtin_trap()
#else
#include <signal.h>
#if defined(SIGTRAP)
#define JN_BREAKPOINT() raise(SIGTRAP)
#else
#define JN_BREAKPOINT() raise(SIGABRT)
#endif
#endif
#endif
