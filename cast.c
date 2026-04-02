#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <Windows.h>



// ctx crack



#ifndef BASE_CTX_CRACK_H
#define BASE_CTX_CRACK_H

////////////////////////////////////////////////////////////////
//~ rjf: MSVC Extraction

#if defined(_MSC_VER)

# define COMPILER_MSVC 1

# if defined(_WIN32)
#  define OS_WINDOWS 1
# else
#  error _MSC_VER is defined, but _WIN32 is not. This setup is not supported.
# endif

# if defined(_M_AMD64)
#  define ARCH_X64 1
# elif defined(_M_IX86)
#  define ARCH_X86 1
# elif defined(_M_ARM64)
#  define ARCH_ARM64 1
# elif defined(_M_ARM)
#  define ARCH_ARM32 1
# else
#  error Target architecture is not supported. _MSC_VER is defined, but one of {_M_AMD64, _M_IX86, _M_ARM64, _M_ARM} is not.
# endif

#if _MSC_VER >= 1920
#define COMPILER_MSVC_YEAR 2019
#elif _MSC_VER >= 1910
#define COMPILER_MSVC_YEAR 2017
#elif _MSC_VER >= 1900
#define COMPILER_MSVC_YEAR 2015
#elif _MSC_VER >= 1800
#define COMPILER_MSVC_YEAR 2013
#elif _MSC_VER >= 1700
#define COMPILER_MSVC_YEAR 2012
#elif _MSC_VER >= 1600
#define COMPILER_MSVC_YEAR 2010
#elif _MSC_VER >= 1500
#define COMPILER_MSVC_YEAR 2008
#elif _MSC_VER >= 1400
#define COMPILER_MSVC_YEAR 2005
#else
#define COMPILER_MSVC_YEAR 0
#endif

////////////////////////////////////////////////////////////////
//~ rjf: Clang Extraction

#elif defined(__clang__)

# define COMPILER_CLANG 1

# if defined(__APPLE__) && defined(__MACH__)
#  define OS_MAC 1
# elif defined(__gnu_linux__)
#  define OS_LINUX 1
# else
#  error __clang__ is defined, but one of {__APPLE__, __gnu_linux__} is not. This setup is not supported.
# endif

# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# else
#  error Target architecture is not supported. __clang__ is defined, but one of {__amd64__, __amd64, __x86_64__, __x86_64, i386, __i386, __i386__, __aarch64__, __arm__} is not.
# endif

////////////////////////////////////////////////////////////////
//~ rjf: GCC Extraction

#elif defined(__GNUC__) || defined(__GNUG__)

# define COMPILER_GCC 1

# if defined(__gnu_linux__)
#  define OS_LINUX 1
# else
#  error __GNUC__ or __GNUG__ is defined, but __gnu_linux__ is not. This setup is not supported.
# endif

# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# else
#  error Target architecture is not supported. __GNU_C__ or __GNUG__ is defined, but one of {__amd64__, __amd64, __x86_64__, __x86_64, i386, __i386, __i386__, __aarch64__, __arm__} is not.
# endif

#else
# error Compiler is not supported. _MSC_VER, __clang__, __GNUC__, or __GNUG__ must be defined.
#endif

#if defined(ARCH_X64)
# define ARCH_64BIT 1
#elif defined(ARCH_X86)
# define ARCH_32BIT 1

#endif

////////////////////////////////////////////////////////////////
//~ rjf: Language

#if defined(__cplusplus)
# define LANG_CPP 1
#else
# define LANG_C 1
#endif

////////////////////////////////////////////////////////////////
//~ rjf: Sanitizers

#if defined(__SANITIZE_ADDRESS__)
# define ASAN_ENABLED 1
#else
# define ASAN_ENABLED 0
#endif

////////////////////////////////////////////////////////////////
//~ rjf: Zero

#if !defined(ARCH_32BIT)
# define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
# define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
# define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
# define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
# define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
# define ARCH_ARM32 0
#endif
#if !defined(COMPILER_MSVC)
# define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif
#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif
#if !defined(OS_MAC)
# define OS_MAC 0
#endif
#if !defined(LANG_CPP)
# define LANG_CPP 0
#endif
#if !defined(LANG_C)
# define LANG_C 0
#endif

////////////////////////////////////////////////////////////////
//~ rjf: Build Parameters

#if !defined(BUILD_DEBUG)
# define BUILD_DEBUG 1
#endif

#if !defined(BUILD_SUPPLEMENTARY_UNIT)
# define BUILD_SUPPLEMENTARY_UNIT 0
#endif

#if !defined(BUILD_ENTRY_POINT_DEFINING_UNIT)
# define BUILD_ENTRY_POINT_DEFINING_UNIT !BUILD_SUPPLEMENTARY_UNIT
#endif

#if !defined(BUILD_COMMAND_LINE_INTERFACE)
# define BUILD_COMMAND_LINE_INTERFACE 0
#endif

#if !defined(BUILD_TELEMETRY)
# define BUILD_TELEMETRY 0
#endif

#endif // BASE_CTX_CRACK_H



// atoi_yy.c
// https://github.com/ibireme/c_numconv_benchmark/blob/master/src/atoi/atoi_yy.c



typedef enum {
    atoi_result_suc = 0,
    atoi_result_fail = 1,
    atoi_result_overflow = 2,
} atoi_result;

/* compiler builtin check (clang) */
#ifndef yy_has_builtin
#   ifdef __has_builtin
#       define yy_has_builtin(x) __has_builtin(x)
#   else
#       define yy_has_builtin(x) 0
#   endif
#endif

/* compiler attribute check (gcc/clang) */
#ifndef yy_has_attribute
#   ifdef __has_attribute
#       define yy_has_attribute(x) __has_attribute(x)
#   else
#       define yy_has_attribute(x) 0
#   endif
#endif

/* inline */
#ifndef yy_inline
#   if _MSC_VER >= 1200
#       define yy_inline __forceinline
#   elif defined(_MSC_VER)
#       define yy_inline __inline
#   elif yy_has_attribute(always_inline) || __GNUC__ >= 4
#       define yy_inline __inline__ __attribute__((always_inline))
#   elif defined(__clang__) || defined(__GNUC__)
#       define yy_inline __inline__
#   elif defined(__cplusplus) || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L)
#       define yy_inline inline
#   else
#       define yy_inline
#   endif
#endif

/* likely */
#ifndef yy_likely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_likely(expr) __builtin_expect(!!(expr), 1)
#   else
#       define yy_likely(expr) (expr)
#   endif
#endif

/* unlikely */
#ifndef yy_unlikely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_unlikely(expr) __builtin_expect(!!(expr), 0)
#   else
#       define yy_unlikely(expr) (expr)
#   endif
#endif

#define repeat_in_1_8(x) { x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) }

#define repeat_in_1_17(x) { x(1) x(2) x(3) x(4) x(5) x(6) x(7) \
                            x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15) \
                            x(16) x(17) }

#define repeat_in_1_18(x) { x(1) x(2) x(3) x(4) x(5) x(6) x(7) \
                            x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15) \
                            x(16) x(17) x(18) }

/** Digit type */
typedef uint8_t digi_type;

/** Digit: '0'. */
static const digi_type DIGI_TYPE_ZERO       = 1 << 0;

/** Digit: [1-9]. */
static const digi_type DIGI_TYPE_NONZERO    = 1 << 1;

/** Minus sign (negative): '-'. */
static const digi_type DIGI_TYPE_NEG        = 1 << 3;


/** Digit type table (generate with misc/make_tables.c) */
static const digi_type digi_table[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x10, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** Match a character with specified type. */
static yy_inline bool digi_is_type(uint8_t d, digi_type type) {
    return (digi_table[d] & type) != 0;
}

/** Match a none zero digit: [1-9] */
static yy_inline bool digi_is_nonzero(uint8_t d) {
    return digi_is_type(d, DIGI_TYPE_NONZERO);
}

/** Match a digit: [0-9] */
static yy_inline bool digi_is_digit(uint8_t d) {
    return digi_is_type(d, DIGI_TYPE_ZERO | DIGI_TYPE_NONZERO);
}

uint32_t atoi_u32_yy(const char *str, size_t len, char **endptr, atoi_result *res) {
    if (yy_unlikely(!digi_is_nonzero(*str))) {
        if (*str == '0' && !digi_is_digit(str[1])) {
            *endptr = (char *)str + 1;
            *res = atoi_result_suc;
        } else {
            *endptr = (char *)str;
            *res = atoi_result_fail;
        }
        return 0;
    }
    
    const char *cur = str;
    uint32_t val = (uint32_t)(*cur - '0'), add;
    *res = atoi_result_suc;
    
#define expr_int(i) \
    if (yy_likely((add = (uint32_t)(cur[i] - '0')) <= 9)) val = add + val * 10; \
    else goto digi_end_##i;
    repeat_in_1_8(expr_int);
#undef expr_int
    goto digi_more;
    
#define expr_end(i) \
    digi_end_##i: *endptr = (char *)cur + i; return val;
    repeat_in_1_8(expr_end)
#undef expr_end
    
digi_more:
    cur += 9;
    if (digi_is_digit(*cur)) {
        add = *cur++ - '0';
        if ((val > UINT32_MAX / 10) ||
            ((val == UINT32_MAX / 10) && (add > UINT32_MAX % 10)) ||
            digi_is_digit(*cur)) {
            while (digi_is_digit(*cur)) cur++;
            *res = atoi_result_overflow;
            *endptr = (char *)cur;
            return UINT32_MAX;;
        } else {
            *endptr = (char *)cur;
            return val * 10 + add;
        }
    } else {
        *endptr = (char *)cur;
        return val;
    }
}



// base



#if COMPILER_MSVC
# define tls __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define tls __thread
#else
# error thread_static not defined for this compiler.
#endif

#if COMPILER_MSVC
# define AlignOf(T) __alignof(T)
#elif COMPILER_CLANG
# define AlignOf(T) __alignof(T)
#elif COMPILER_GCC
# define AlignOf(T) __alignof__(T)
#else
# error AlignOf not defined for this compiler.
#endif

#define Kib(N) N << 10
#define Mib(N) N << 20
#define Gib(N) N << 30
#define Tib(N) N << 40
#define Pib(N) N << 50

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)
#define Stringizing_(X) #X
#define Stringizing(X) Stringizing_(X)
#define DeferLoop(Begin, End) for (int _I_ = ((Begin), 0); !_I_; _I_ += 1, (End))
#define StaticAssert(Id, Cond) static uint8_t Glue(Id, __LINE__)[(Cond) ? 1 : -1]

#define Min(N1, N2) (((N1) < (N2)) ? (N1) : (N2))
#define Max(N1, N2) (((N1) > (N2)) ? (N1) : (N2))
#define Clamp(A,X,B) (((X)<(A))?(A):((X)>(B))?(B):(X))
#define AlignPow2(N, Align) (((N) + (Align) - 1) & (~((Align) - 1)))
#define ArrLen(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

#ifdef DEBUG_BUILD
#define Assert(Cond) (!(Cond) && *(unsigned char *)(0))
#else
#define Assert(Cond) ((void)0)
#endif


// array of 8 bit elements, string type
typedef struct a8 a8;
struct a8
{
 char *Mem;
 size_t Ln;
};

static a8
A8(char *S, size_t Ln)
{
 return (a8){.Mem = S, .Ln = Ln};
}
#define CStr(S) A8(S, sizeof(S "") - 1)

////////////////////////////////
//~ rjf: Linked List Building Macros

//- rjf: linked list macro helpers
#define CheckNil(nil, p) ((p) == 0 || (p) == nil)
#define SetNil(nil, p) ((p) = nil)

//- rjf: doubly-linked-lists
#define DLLInsert_NPZ(nil, f, l, p, n, next, prev) (CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next), SetNil(nil, (n)->prev)) : CheckNil(nil, p) ? ((n)->next = (f), (f)->prev = (n), (f) = (n), SetNil(nil, (n)->prev)) \
                                                                                                                                       : ((p) == (l))       ? ((l)->next = (n), (n)->prev = (l), (l) = (n), SetNil(nil, (n)->next)) \
                                                                                                                                                            : (((!CheckNil(nil, p) && CheckNil(nil, (p)->next)) ? (0) : ((p)->next->prev = (n))), ((n)->next = (p)->next), ((p)->next = (n)), ((n)->prev = (p))))
#define DLLPushBack_NPZ(nil, f, l, n, next, prev) DLLInsert_NPZ(nil, f, l, l, n, next, prev)
#define DLLPushFront_NPZ(nil, f, l, n, next, prev) DLLInsert_NPZ(nil, l, f, f, n, prev, next)
#define DLLRemove_NPZ(nil, f, l, n, next, prev) (((n) == (f) ? (f) = (n)->next : (0)),                             \
                                                 ((n) == (l) ? (l) = (l)->prev : (0)),                             \
                                                 (CheckNil(nil, (n)->prev) ? (0) : ((n)->prev->next = (n)->next)), \
                                                 (CheckNil(nil, (n)->next) ? (0) : ((n)->next->prev = (n)->prev)))

//- rjf: singly-linked, doubly-headed lists (queues)
#define SLLQueuePush_NZ(nil, f, l, n, next) (CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next)) : ((l)->next = (n), (l) = (n), SetNil(nil, (n)->next)))
#define SLLQueuePushFront_NZ(nil, f, l, n, next) (CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next)) : ((n)->next = (f), (f) = (n)))
#define SLLQueuePop_NZ(nil, f, l, next) ((f) == (l) ? (SetNil(nil, f), SetNil(nil, l)) : ((f) = (f)->next))

//- rjf: singly-linked, singly-headed lists (stacks)
#define SLLStackPush_N(f, n, next) ((n)->next = (f), (f) = (n))
#define SLLStackPop_N(f, next) ((f) = (f)->next)

//- rjf: doubly-linked-list helpers
#define DLLInsert_NP(f, l, p, n, Next, Prev) DLLInsert_NPZ(0, f, l, p, n, Next, Prev)
#define DLLPushBack_NP(f, l, n, Next, Prev) DLLPushBack_NPZ(0, f, l, n, Next, Prev)
#define DLLPushFront_NP(f, l, n, Next, Prev) DLLPushFront_NPZ(0, f, l, n, Next, Prev)
#define DLLRemove_NP(f, l, n, Next, Prev) DLLRemove_NPZ(0, f, l, n, Next, Prev)
#define DLLInsert(f, l, p, n) DLLInsert_NPZ(0, f, l, p, n, Next, Prev)
#define DLLPushBack(f, l, n) DLLPushBack_NPZ(0, f, l, n, Next, Prev)
#define DLLPushFront(f, l, n) DLLPushFront_NPZ(0, f, l, n, Next, Prev)
#define DLLRemove(f, l, n) DLLRemove_NPZ(0, f, l, n, Next, Prev)

//- rjf: singly-linked, doubly-headed list helpers
#define SLLQueuePush_N(f, l, n, Next) SLLQueuePush_NZ(0, f, l, n, Next)
#define SLLQueuePushFront_N(f, l, n, Next) SLLQueuePushFront_NZ(0, f, l, n, Next)
#define SLLQueuePop_N(f, l, Next) SLLQueuePop_NZ(0, f, l, Next)
#define SLLQueuePush(f, l, n) SLLQueuePush_NZ(0, f, l, n, Next)
#define SLLQueuePushFront(f, l, n) SLLQueuePushFront_NZ(0, f, l, n, Next)
#define SLLQueuePop(f, l) SLLQueuePop_NZ(0, f, l, Next)

//- rjf: singly-linked, singly-headed list helpers
#define SLLStackPush(f, n) SLLStackPush_N(f, n, Next)
#define SLLStackPop(f) SLLStackPop_N(f, Next)

//~ rjf: Vector Types

//- rjf: 2-vectors

typedef union v2u32 v2u32;
union v2u32
{
 struct
 {
  uint32_t X;
  uint32_t Y;
 };
 uint32_t V[2];
};

typedef union v2u8 v2u8;
union v2u8
{
 struct
 {
  uint8_t X;
  uint8_t Y;
 };
 uint8_t V[2];
};

static v2u8
V2U8(uint8_t X, uint8_t Y)
{
 return (v2u8){.X = X, .Y = Y};
}

static v2u32
V2U32(uint32_t X, uint32_t Y)
{
 return (v2u32){.X = X, .Y = Y};
}

// Mem stuff

static int32_t
MemCmp(void *MemA, void *MemB, size_t Len)
{
 int32_t Ret = 0;
 uint8_t *A = MemA;
 uint8_t *B = MemB;
 while (Len--)
 {
  if ((Ret = *A++ - *B++)) break;
 }
 return Ret;
}

static void *
MemCpy(void *Dst, void *Src, size_t N)
{
 void *Ret = Dst;
 for (size_t I = 0; I < N; ++I)
 {
  ((char *)Dst)[I] = ((char *)Src)[I];
 }

 return Ret;
}

static void
MemSet(void *Start, uint32_t Byte, size_t Len)
{
 for (size_t I = 0; I < Len; ++I)
 {
  uint8_t *Mem = ((uint8_t *)Start) + I;
  *Mem = (uint8_t)Byte;
 }
}

static void
MemZero(void *Start, size_t Len)
{
 MemSet(Start, 0, Len);
}


// os decl



typedef struct os_sys_info os_sys_info;
struct os_sys_info
{
 size_t PageSize;
 size_t LogicalProcessors;
};

typedef struct os_process_info os_process_info;
struct os_process_info
{
 uint32_t pid;
 uint32_t large_pages_allowed;
 // a1 binary_path;
 // a1 initial_path;
 // a1 user_program_data_path;
 // a1_list module_load_paths;
 // a1_list environment;
};

static os_sys_info *OS_GetSysInfo();
static os_process_info *OS_GetProcessInfo();

static void *
OS_Reserve(size_t Size);

static void *
OS_Commit(void *Mem, size_t Size);

static uint32_t
OS_Decommit(void *Mem, size_t Size);

static uint32_t
OS_Release(void *Mem, size_t Size);



// arena



#define AR_HEADER_SIZE 64
typedef struct ar ar;
struct ar
{
 ar *First;
 ar *Prev;
 size_t BaseLen;
 size_t Len;
 size_t OgCom;
 size_t Com;
 size_t Res;
};
StaticAssert(ar_size_check, sizeof(ar) < AR_HEADER_SIZE);

typedef struct ar_tmp ar_tmp;
struct ar_tmp
{
 ar *Ar;
 size_t Len;
};

typedef struct ar_params ar_params;
struct ar_params
{
 size_t BaseLen;
 size_t Com;
 size_t Res;
 size_t PageSize;
};

static ar *
_ArAlloc(ar_params In)
{
 Assert(In.Com && In.Res && In.PageSize && (In.Com <= In.Res));
 size_t Res = Max(AlignPow2(In.Res, In.PageSize), AR_HEADER_SIZE);
 size_t Com = Max(AlignPow2(In.Com, In.PageSize), AR_HEADER_SIZE);
 ar *Ar = OS_Commit(OS_Reserve(Res), Com);
 if (Ar)
 {
  *Ar = (ar){
   .First = Ar,
   .BaseLen = In.BaseLen,
   .Len = AR_HEADER_SIZE,
   .Com = Com,
   .OgCom = Com,
   .Res = Res,
  };
 }
 return Ar;
}
#define ArAlloc(...) _ArAlloc((ar_params){.Com = Kib(10), .Res = Mib(1), .PageSize = OS_GetSysInfo()->PageSize, __VA_ARGS__})

static void
ArRelease(ar *Ar)
{
 for (ar *N = Ar->First, *Prev = 0; N; N = Prev)
 {
  Prev = N->Prev; // hold this before release
  OS_Release(N, N->Res);
 }
}

static void *
ArPushEx(ar *Ar, size_t Len, size_t Aln, uint32_t Zero)
{
 void *Ret = 0;
 ar *Cur = Ar->First;
 size_t AlnLen = AlignPow2(Cur->Len, Aln);
 size_t NewLen = AlnLen + Len;
 if (NewLen > Ar->Res)
 {
  size_t NewArLen = AlignPow2(AR_HEADER_SIZE, Aln) + Len;
  size_t Com = (NewArLen > Ar->Res) ? NewArLen : Ar->OgCom;
  size_t Res = (NewArLen > Ar->Res) ? NewArLen : Ar->Res;
  ar *NewAr = ArAlloc(.BaseLen = Cur->BaseLen + Cur->Res, .Com = Com, .Res = Res);
  Cur = SLLStackPush_N(Ar->First, NewAr, Prev);
  AlnLen = AlignPow2(Cur->Len, Aln);
  NewLen = AlnLen + Len;
 }

 if (Cur->Com < NewLen)
 {
  size_t CmtLen = Min((NewLen + Cur->OgCom - 1) - ((NewLen + Cur->OgCom - 1) % Cur->OgCom) - Cur->Com, Cur->Res - Cur->Com);
  OS_Commit((char *)Cur + Cur->Com, CmtLen);
  Cur->Com += CmtLen;
 }

 Ret = (char *)Cur + AlnLen;
 Cur->Len = NewLen;

 size_t ZeroLen = Zero ? Min(Cur->Com, NewLen) - AlnLen : 0;
 MemZero(Ret, ZeroLen);

 return Ret;
}
#define ArPush(Ar, T, Len) ArPushEx((Ar), sizeof(T) * (Len), Max(8, AlignOf(T)), 1)
#define ArPushNoZero(Ar, T, Len) ArPushEx((Ar), sizeof(T) * (Len), Max(8, AlignOf(T)), 0)

#define ArPushA8(Ar, Len) A8(ArPush(Ar, int8_t, Len), Len)

static void
ArPopTo(ar *Ar, size_t Len)
{
 size_t AccLen = Max(AR_HEADER_SIZE, Len);

 ar *Cur = Ar->First;
 for (; Cur->BaseLen > AccLen; Cur = Cur->Prev)
 {
  OS_Release(Cur, Cur->Res);
 }
 Ar->First = Cur;
 Assert((AccLen - Cur->BaseLen) <= Cur->Len);
 Cur->Len = AccLen - Cur->BaseLen;
}

static void
ArClr(ar *Ar)
{
 ArPopTo(Ar, 0);
}

static size_t
ArAccLen(ar *Ar)
{
 return Ar->BaseLen + Ar->Len;
}

static void
ArPop(ar *Ar, size_t Len)
{
 ArPopTo(Ar, ArAccLen(Ar) - ((Len <= ArAccLen(Ar)) ? Len : 0));
}

static ar_tmp
ArTmpGet(ar *Ar)
{
 return (ar_tmp){.Ar = Ar, .Len = Ar->Len};
}

static void
ArTmpEnd(ar_tmp ArTmp)
{
 ArPopTo(ArTmp.Ar, ArTmp.Len);
}

// terms:
// ar: arena
// com: committed length
// res: reserved length
// ogcom: original committed length
// len: used length within this arena
// baselen: sum of all previous arena's reserved lengths
// acclen: acculative length, base length plus this arena's length



// thread



// primitives

typedef struct thread thread;
struct thread
{
 size_t U8[1];
};

typedef struct mx mx;
struct mx
{
 size_t U8[1];
};

typedef struct rw rw;
struct rw
{
 size_t U8[1];
};

typedef struct cv cv;
struct cv
{
 size_t U8[1];
};

typedef struct semaphore semaphore;
struct semaphore
{
 size_t U8[1];
};

typedef struct bar bar;
struct bar
{
 size_t U8[1];
};

typedef struct stripe stripe;
struct stripe
{
 ar *Ar;
 rw Rw;
 cv Cv;
 void *Free;
};

typedef struct stripe_arr stripe_arr;
struct stripe_arr
{
 stripe *V;
 size_t Len;
};

typedef struct lane_ctx lane_ctx;
struct lane_ctx
{
 size_t LaneIdx;
 size_t LaneCnt;
 bar Barrier;
 size_t *BroadcastMem;
};

typedef struct access_pt access_pt;
struct access_pt
{
 size_t AccessRefCnt;
 size_t LastTimeTouchedMs;
 size_t LastUpdateIdxTouched;
};

typedef struct access_pt_expire_params access_pt_expire_params;
struct access_pt_expire_params
{
 size_t Time;
 size_t UpdateIdxs;
};

typedef struct touch touch;
struct touch
{
 touch *Next;
 access_pt *Pt;
 cv Cv;
};

typedef struct access access;
struct access
{
 access *Next;
 touch *TopTouch;
};

typedef struct tctx tctx;
struct tctx
{
 ar *ArArr[2];
 lane_ctx LaneCtx;
};

tls tctx *TlsTctx;

static tctx *
TctxAlloc()
{
 ar *Ar = ArAlloc();
 tctx *Tctx = ArPush(Ar, tctx, 1);
 Tctx->ArArr[0] = Ar;
 Tctx->ArArr[1] = ArAlloc();
 Tctx->LaneCtx.LaneCnt = 1;
 return Tctx;
}

static void
TctxRelease(tctx *Tctx)
{
 ArRelease(Tctx->ArArr[1]);
 ArRelease(Tctx->ArArr[0]);
}

static ar *
TctxGetAr(ar **ConflictArr, size_t Len)
{
 ar *Ret = 0;
 tctx *Tctx = TlsTctx;
 for (size_t I = 0; I < ArrLen(Tctx->ArArr); ++I)
 {
  uint32_t HasConflict = 0;
  for (size_t J = 0; J < Len; ++J)
  {
   if ((HasConflict = (Tctx->ArArr[I] == ConflictArr[J])))
   {
    break;
   }
  }
  if (!HasConflict)
  {
   Ret = Tctx->ArArr[I];
   break;
  }
 }
 return Ret;
}

#define ScratchGet(Conflicts, Len) ArTmpGet(TctxGetAr((Conflicts), (Len)))
#define ScratchEnd(Tmp) ArTmpEnd(Tmp)



// str section



static uint32_t
ChrIsAlpha(char C)
{
 C |= 0x20;
 return C >= 'a' && C <= 'z';
}

static uint32_t
ChrIsNum(char C)
{
 return C >= '0' && C <= '9';
}

static uint32_t
ChrIsAlphaNum(char C)
{
 return ChrIsAlpha(C) || ChrIsNum(C);
}

static uint32_t
ChrIsSymbol(uint8_t C)
{
 return (C >= '!' && C <= '/') || (C >= ':' && C <= '@') || (C >= '[' && C <= '`') || (C >= '{' && C <= '~');
}

static uint32_t
StrEq(char *P1, size_t Ln1, char *P2, size_t Ln2)
{
 return Ln1 == Ln2 && !MemCmp(P1, P2, Ln1);
}

static uint32_t
StrStartsWith(char *P1, size_t Ln1, char *P2, size_t Ln2)
{
 return Ln1 && Ln2 && Ln1 >= Ln2 && !MemCmp(P1, P2, Ln2);
}



// os



typedef void thread_entry_point_fn(void *P);

typedef uint32_t os_w32_ent_kind;
enum
{
 OS_W32_EntityKind_Null,
 OS_W32_EntityKind_Thread,
 OS_W32_EntityKind_Mutex,
 OS_W32_EntityKind_RWMutex,
 OS_W32_EntityKind_ConditionVariable,
 OS_W32_EntityKind_Barrier,
};

typedef struct os_w32_ent os_w32_ent;
struct os_w32_ent
{
 os_w32_ent *Next;
 os_w32_ent_kind Kind;
 union
 {
  struct
  {
   thread_entry_point_fn *Fn;
   void *Ptr;
   HANDLE Handle;
   DWORD Tid;
  } Thread;
  CRITICAL_SECTION Mtx;
  SRWLOCK Rwmtx;
  CONDITION_VARIABLE Cv;
  SYNCHRONIZATION_BARRIER Sb;
 };
};

typedef struct os_w32_state os_w32_state;
struct os_w32_state
{
 ar *Ar;
  
 // rjf: info
 os_sys_info SysInfo;
 os_process_info ProcessInfo;
 size_t ResolutionUs;
  
 // rjf: entity storage
 CRITICAL_SECTION EntMtx;
 ar *EntAr;
 os_w32_ent *EntFree;
};
static os_w32_state OS_W32State = {0};

static void
OS_Init(os_w32_state *In)
{
 SYSTEM_INFO SysInfo = {0};
 GetSystemInfo(&SysInfo);
 In->SysInfo.PageSize = SysInfo.dwPageSize;
 In->SysInfo.LogicalProcessors = SysInfo.dwNumberOfProcessors;

 In->ResolutionUs = 1;
 {
  LARGE_INTEGER LargeInt;
  if (QueryPerformanceFrequency(&LargeInt))
  {
   In->ResolutionUs = LargeInt.QuadPart;
  }
 }
  
 // Ar is dependent on SysInfo init ^^^
 In->Ar = ArAlloc();
 // rjf: entity storage
 InitializeCriticalSection(&In->EntMtx);
 In->EntAr = ArAlloc();

 TlsTctx = TctxAlloc();
}

static os_sys_info *
OS_GetSysInfo()
{
 return &OS_W32State.SysInfo;
}

static void *
OS_Reserve(size_t Size)
{
 return VirtualAlloc(0, Size, MEM_RESERVE, PAGE_NOACCESS);
}

static void *
OS_Commit(void *Mem, size_t Size)
{
 return VirtualAlloc(Mem, Size, MEM_COMMIT, PAGE_READWRITE);
}

static uint32_t
OS_Decommit(void *Mem, size_t Size)
{
 return VirtualFree(Mem, Size, MEM_DECOMMIT);
}

static uint32_t
OS_Release(void *Mem, size_t Size)
{
 Size;
 return VirtualFree(Mem, 0, MEM_RELEASE);
}

static void *
DebugReadFile(LPCWSTR Name, uint32_t *BytesRead)
{
 void *Ret = 0;
 HANDLE FileHandle = CreateFileW(Name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
 if (FileHandle)
 {
  LARGE_INTEGER FileSize;
  if (GetFileSizeEx(FileHandle, &FileSize))
  {
   assert(FileSize.QuadPart < 0xffffffff);
   void *FileMemory = VirtualAlloc(0, FileSize.QuadPart, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
   if (FileMemory)
   {
    if (ReadFile(FileHandle, FileMemory, (uint32_t)FileSize.QuadPart, BytesRead, 0) && *BytesRead == (uint32_t)FileSize.QuadPart)
    {
     Ret = FileMemory;
    }
    else
    {
     VirtualFree(FileMemory, 0, MEM_RELEASE);
     FileMemory = 0;
     *BytesRead = 0;
    }
   }
  }
  CloseHandle(FileHandle);
 }

 return Ret;
}



// lex



typedef uint32_t jc_tkn_kind;
typedef enum
{
 // compiler tokens
 //todo not sure why err 0 and not eof 0, would be nice if jctknarreat returned jctknzero to signify eof, but matklad resil parse article does this way
 JcTknErr, 
 JcTknEof,

 // tokens for no loss
 JcTknHws,
 JcTknVws,
 JcTknNewlineEsc,
 JcTknSingleLineComment,
 JcTknMultiLineComment,

 JcTknIdent,

 // literals
 JcTknNum,
 JcTknChar,
 JcTknStr,

 // punctuators
 // ln 3
 JcTknShiftLeftAssignment,
 JcTknShiftRightAssignment,
 JcTknEllipsis,

 // ln 2
 JcTknLessThanEqual,
 JcTknGreaterThanEqual,
 JcTknLogicalAnd,
 JcTknLogicalOr,
 JcTknShiftLeft,
 JcTknShiftRight,

 JcTknPostfixIncrement,
 JcTknPostfixDecrement,
 JcTknPrefixIncrement,
 JcTknPrefixDecrement,

 JcTknPtrMemberAccess,
 JcTknDoubleColon,//?
 JcTknTokenPasting,
 JcTknAddAssignment,
 JcTknSubtractAssignment,
 JcTknMultiplyAssignment,
 JcTknDivideAssignment,
 JcTknModuloAssignment,
 JcTknBitwiseXorAssignment,
 JcTknBitwiseAndAssignment,
 JcTknBitwiseOrAssignment,
 JcTknCompareEqual,
 JcTknCompareNotEqual,

 // ln 1
 JcTknLCurly,
 JcTknRCurly,
 JcTknLBrack,
 JcTknRBrack,
 JcTknLParen,
 JcTknRParen,
 JcTknStringizing,
 JcTknSemicolon,
 JcTknColon,
 JcTknQuestion,
 JcTknMemberAccess,
 JcTknBitwiseNot,
 JcTknLogicalNot,

 JcTknAdd,
 JcTknUnaryAdd,
 JcTknSubtract,
 JcTknUnarySubtract,

 JcTknMultiply,
 JcTknDereference,

 JcTknDivide,
 JcTknModulo,
 JcTknBitwiseXor,
 
 JcTknBitwiseAnd,
 JcTknAddressOf,

 JcTknBitwiseOr,
 JcTknAssignment,
 JcTknLessThan,
 JcTknGreaterThan,
 JcTknComma,

 JcTknCount,
} jc_tkn_kind;

v2u8 JcBpTab[JcTknCount] = {0};

a8 JcTknTab[JcTknCount] = {0};
jc_tkn_kind JcPrefixTab[JcTknCount] = {0};

typedef struct jc_tkn jc_tkn;
struct jc_tkn
{
 jc_tkn_kind Kind;
 char *Mem;
 size_t Ln;

 jc_tkn *Next;
 jc_tkn *First;
};

typedef struct jc_tkn_arr jc_tkn_arr;
struct jc_tkn_arr
{
 ar *Ar;
 jc_tkn *Mem;
 size_t Ln;
};

static jc_tkn
JcTkn(jc_tkn_kind Kind, char *Mem, size_t Ln)
{
 return (jc_tkn){.Kind = Kind, .Mem = Mem, .Ln = Ln};
}

static void
JcBpTabInit()
{
 JcBpTab[JcTknPostfixIncrement] = V2U8(31, 0);
 JcBpTab[JcTknPostfixDecrement] = V2U8(31, 0);
 // todo fn call ()
 // todo array subscript []
 JcBpTab[JcTknMemberAccess] = V2U8(31, 32);
 JcBpTab[JcTknPtrMemberAccess] = V2U8(31, 32);
 // todo compound literal (type){list}

 JcBpTab[JcTknPrefixIncrement] = V2U8(0, 29);
 JcBpTab[JcTknPrefixDecrement] = V2U8(0, 29);
 JcBpTab[JcTknUnaryAdd] = V2U8(0, 29);
 JcBpTab[JcTknUnarySubtract] = V2U8(0, 29);
 JcBpTab[JcTknLogicalNot] = V2U8(30, 29);
 JcBpTab[JcTknBitwiseNot] = V2U8(30, 29);
 JcBpTab[JcTknDereference] = V2U8(0, 29);
 JcBpTab[JcTknAddressOf] = V2U8(0, 29);
 // todo sizeof, alignof (30, 29)

 // todo typecast (28, 27)

 JcBpTab[JcTknMultiply] = V2U8(25, 26);
 JcBpTab[JcTknDivide] = V2U8(25, 26);
 JcBpTab[JcTknModulo] = V2U8(25, 26);

 JcBpTab[JcTknAdd] = V2U8(23, 24);
 JcBpTab[JcTknSubtract] = V2U8(23, 24);

 JcBpTab[JcTknShiftLeft] = V2U8(21, 22);
 JcBpTab[JcTknShiftRight] = V2U8(21, 22);

 JcBpTab[JcTknLessThan] = V2U8(19, 20);
 JcBpTab[JcTknGreaterThan] = V2U8(19, 20);
 JcBpTab[JcTknLessThanEqual] = V2U8(19, 20);
 JcBpTab[JcTknGreaterThanEqual] = V2U8(19, 20);

 JcBpTab[JcTknCompareEqual] = V2U8(17, 18);
 JcBpTab[JcTknCompareNotEqual] = V2U8(17, 18);

 JcBpTab[JcTknBitwiseAnd] = V2U8(15, 16);

 JcBpTab[JcTknBitwiseXor] = V2U8(13, 14);

 JcBpTab[JcTknBitwiseOr] = V2U8(11, 12);

 JcBpTab[JcTknLogicalAnd] = V2U8(9, 10);

 JcBpTab[JcTknLogicalOr] = V2U8(7, 8);

 // todo ternary (6, 5)

 JcBpTab[JcTknAssignment] = V2U8(4, 3);
 JcBpTab[JcTknMultiplyAssignment] = V2U8(4, 3);
 JcBpTab[JcTknDivideAssignment] = V2U8(4, 3);
 JcBpTab[JcTknAddAssignment] = V2U8(4, 3);
 JcBpTab[JcTknSubtractAssignment] = V2U8(4, 3);
 JcBpTab[JcTknModuloAssignment] = V2U8(4, 3);
 JcBpTab[JcTknShiftLeftAssignment] = V2U8(4, 3);
 JcBpTab[JcTknShiftRightAssignment] = V2U8(4, 3);
 JcBpTab[JcTknBitwiseAndAssignment] = V2U8(4, 3);
 JcBpTab[JcTknBitwiseXorAssignment] = V2U8(4, 3);
 JcBpTab[JcTknBitwiseOrAssignment] = V2U8(4, 3);

 JcBpTab[JcTknComma] = V2U8(1, 2);
}

static void
JcPrefixTabInit()
{
 JcPrefixTab[JcTknAdd] = JcTknUnaryAdd;
 JcPrefixTab[JcTknSubtract] = JcTknUnarySubtract;
 JcPrefixTab[JcTknMultiply] = JcTknDereference;
 JcPrefixTab[JcTknBitwiseAnd] = JcTknAddressOf;
 JcPrefixTab[JcTknPostfixDecrement] = JcTknPrefixDecrement;
 JcPrefixTab[JcTknPostfixIncrement] = JcTknPrefixIncrement;
}

static void
JcTknTabInit()
{
 JcTknTab[JcTknLCurly] = CStr("{");
 JcTknTab[JcTknRCurly] = CStr("}");
 JcTknTab[JcTknLBrack] = CStr("[");
 JcTknTab[JcTknRBrack] = CStr("]");
 JcTknTab[JcTknStringizing] = CStr("#");
 JcTknTab[JcTknTokenPasting] = CStr("##");
 JcTknTab[JcTknLParen] = CStr("(");
 JcTknTab[JcTknRParen] = CStr(")");
 JcTknTab[JcTknSemicolon] = CStr(";");
 JcTknTab[JcTknColon] = CStr(":");
 JcTknTab[JcTknEllipsis] = CStr("...");
 JcTknTab[JcTknQuestion] = CStr("?");
 JcTknTab[JcTknDoubleColon] = CStr("::");
 JcTknTab[JcTknMemberAccess] = CStr(".");
 JcTknTab[JcTknPtrMemberAccess] = CStr("->");
 JcTknTab[JcTknBitwiseNot] = CStr("~");
 JcTknTab[JcTknLogicalNot] = CStr("!");
 JcTknTab[JcTknAdd] = CStr("+");
 JcTknTab[JcTknSubtract] = CStr("-");
 JcTknTab[JcTknMultiply] = CStr("*");
 JcTknTab[JcTknDivide] = CStr("/");
 JcTknTab[JcTknModulo] = CStr("%");
 JcTknTab[JcTknBitwiseXor] = CStr("^");
 JcTknTab[JcTknBitwiseAnd] = CStr("&");
 JcTknTab[JcTknBitwiseOr] = CStr("|");
 JcTknTab[JcTknAssignment] = CStr("=");
 JcTknTab[JcTknAddAssignment] = CStr("+=");
 JcTknTab[JcTknSubtractAssignment] = CStr("-=");
 JcTknTab[JcTknMultiplyAssignment] = CStr("*=");
 JcTknTab[JcTknDivideAssignment] = CStr("/=");
 JcTknTab[JcTknModuloAssignment] = CStr("%=");
 JcTknTab[JcTknBitwiseXorAssignment] = CStr("^=");
 JcTknTab[JcTknBitwiseAndAssignment] = CStr("&=");
 JcTknTab[JcTknBitwiseOrAssignment] = CStr("|=");
 JcTknTab[JcTknCompareEqual] = CStr("==");
 JcTknTab[JcTknCompareNotEqual] = CStr("!=");
 JcTknTab[JcTknLessThan] = CStr("<");
 JcTknTab[JcTknGreaterThan] = CStr(">");
 JcTknTab[JcTknLessThanEqual] = CStr("<=");
 JcTknTab[JcTknGreaterThanEqual] = CStr(">=");
 JcTknTab[JcTknLogicalAnd] = CStr("&&");
 JcTknTab[JcTknLogicalOr] = CStr("||");
 JcTknTab[JcTknShiftLeft] = CStr("<<");
 JcTknTab[JcTknShiftRight] = CStr(">>");
 JcTknTab[JcTknShiftLeftAssignment] = CStr("<<=");
 JcTknTab[JcTknShiftRightAssignment] = CStr(">>=");
 JcTknTab[JcTknPostfixIncrement] = CStr("++");
 JcTknTab[JcTknPostfixDecrement] = CStr("--");
 JcTknTab[JcTknComma] = CStr(",");

 JcTknTab[JcTknUnaryAdd] = CStr("+");
 JcTknTab[JcTknUnarySubtract] = CStr("-");
 JcTknTab[JcTknDereference] = CStr("*");
 JcTknTab[JcTknAddressOf] = CStr("&");
 JcTknTab[JcTknPrefixDecrement] = CStr("--");
 JcTknTab[JcTknPrefixIncrement] = CStr("++");
}

static void
A8ShlMut(a8 *A, size_t Ln)
{
 size_t N = Min(A->Ln, Ln);
 A->Mem += N;
 A->Ln -= N;
}

static uint32_t
A8EatMut(a8 *A, a8 Prefix)
{
 if (StrStartsWith(A->Mem, A->Ln, Prefix.Mem, Prefix.Ln))
 {
  A8ShlMut(A, Prefix.Ln);
  return 1;
 }
 return 0;
}

static uint32_t
A8EatNumMut(a8 *A, jc_tkn *Out)
{
 // todo maybe store N in the tkn?
 atoi_result Res = 0;
 char *End = 0;
 uint32_t N = atoi_u32_yy(A->Mem, A->Ln, &End, &Res);
 if (Res == atoi_result_suc)
 {
  char *Start = A->Mem;
  size_t Ln = End - A->Mem;
  A8ShlMut(A, Ln);
  *Out = JcTkn(JcTknNum, Start, Ln);
  return 1;
 }
 return 0;
}

static uint32_t
A8EatPunctuatorMut(a8 *A, jc_tkn *Out)
{
 // loop through enum
 char *Start = A->Mem;
 for (size_t I = 0; I < JcTknCount; ++I)
 {
  a8 Prefix = JcTknTab[I];
  if (A8EatMut(A, Prefix))
  {
   *Out = (jc_tkn){
    .Kind = (jc_tkn_kind)I,
    .Mem = Start,
    .Ln = Prefix.Ln,
   };
   return 1;
  }
 }
 return 0;
}

static uint32_t
A8EatIdentMut(a8 *A, jc_tkn *Out)
{
 if (ChrIsAlpha(A->Mem[0]))
 {
  char *Start = A->Mem;
  size_t I = 1;
  while (I < A->Ln)
  {
   char C = A->Mem[I];
   // todo replace with better check
   if (!ChrIsAlphaNum(C))
   {
    break;
   }
   I++;
  }
  A8ShlMut(A, I);
  *Out = JcTkn(JcTknIdent, Start, I);
  return 1;
 }
 return 0;
}

static uint32_t 
A8EatHwsMut(a8 *A, jc_tkn *Out)
{
 char *Start = A->Mem;
 a8 Spc = CStr(" ");
 a8 Tab = CStr("\t");
 if (A8EatMut(A, Spc))
 {
  *Out = JcTkn(JcTknHws, Start, Spc.Ln);
  return 1;
 }
 else if (A8EatMut(A, Tab))
 {
  *Out = JcTkn(JcTknHws, Start, Tab.Ln);
  return 1;
 }
 return 0;
}


static uint32_t
A8EatVwsMut(a8 *A, jc_tkn *Out)
{
 char *Start = A->Mem;
 a8 Lf = CStr("\n");
 a8 Crlf = CStr("\r\n");
 if (A8EatMut(A, Lf))
 {
  *Out = JcTkn(JcTknVws, Start, Lf.Ln);
  return 1;
 }
 else if (A8EatMut(A, Crlf))
 {
  *Out = JcTkn(JcTknVws, Start, Crlf.Ln);
  return 1;
 }
 return 0;
}

static void
TknArrPush(jc_tkn_arr *Arr, jc_tkn *Tkn)
{
 jc_tkn *Mem = ArPush(Arr->Ar, jc_tkn, 1);
 if (!Arr->Mem)
 {
  Arr->Mem = Mem;
 }
 *Mem = *Tkn;
 Arr->Ln++;
}

static jc_tkn JcTknGlobalZero = {0};
static jc_tkn JcTknGlobalEof = { .Kind = JcTknEof };

static uint32_t
JcTknKindIsExprRelevant(jc_tkn_kind Kind)
{
 return Kind != JcTknHws && Kind != JcTknVws;
}

static jc_tkn *
JcTknArrPeek(jc_tkn_arr *TknArr)
{
 jc_tkn *Ret = &JcTknGlobalEof;
 if (TknArr->Ln)
 {
  jc_tkn *Ret = TknArr->Mem;
 }
 return Ret;
}

// skips whitespace
static jc_tkn *
JcTknArrPeekRelevant(jc_tkn_arr *TknArr)
{
 jc_tkn *Ret = &JcTknGlobalEof;
 for (size_t I = 0; I < TknArr->Ln; ++I)
 {
  if (JcTknKindIsExprRelevant(TknArr->Mem[I].Kind))
  {
   Ret = TknArr->Mem + I;
   break;
  }
 }
 return Ret;
}

static jc_tkn *
JcTknArrEat(jc_tkn_arr *TknArr)
{
 jc_tkn *Ret = &JcTknGlobalEof;
 if (TknArr->Ln)
 {
  Ret = TknArr->Mem;
  TknArr->Mem++;
  TknArr->Ln--;
 }
 return Ret;
}

// skips whitespace
static jc_tkn *
JcTknArrEatRelevant(jc_tkn_arr *TknArr)
{
 jc_tkn *Ret = 0;
 while ((Ret = JcTknArrEat(TknArr)) && !JcTknKindIsExprRelevant(Ret->Kind))
 {
 }
 return Ret;
}

static void
JcTknPrintMut(a8 *A, jc_tkn *Tkn)
{
 if (Tkn->First && Tkn->First->Next)
 {
  // binary op
  a8 TknStr = JcTknTab[Tkn->Kind];
  int WriteLn = sprintf_s(A->Mem, A->Ln, "(%s ", TknStr.Mem);
  A8ShlMut(A, WriteLn);

  JcTknPrintMut(A, Tkn->First);

  WriteLn = sprintf_s(A->Mem, A->Ln, " ");
  A8ShlMut(A, WriteLn);

  JcTknPrintMut(A, Tkn->First->Next);

  WriteLn = sprintf_s(A->Mem, A->Ln, ")");
  A8ShlMut(A, WriteLn);
 }
 else if (Tkn->First)
 {
  // unary op
  a8 TknStr = JcTknTab[Tkn->Kind];
  int WriteLn = sprintf_s(A->Mem, A->Ln, "(%s ", TknStr.Mem);
  A8ShlMut(A, WriteLn);

  JcTknPrintMut(A, Tkn->First);

  WriteLn = sprintf_s(A->Mem, A->Ln, ")");
  A8ShlMut(A, WriteLn);
 }
 else
 {
  // atomic
  int WriteLn = sprintf_s(A->Mem, A->Ln, "%.*s", (int)Tkn->Ln, Tkn->Mem);
  A8ShlMut(A, WriteLn);
 }
}

static void
JcTknPrint(a8 A, jc_tkn *Tkn)
{
 JcTknPrintMut(&A, Tkn);
}

static uint32_t
JcOpInfixRightBindsTighter(jc_tkn_kind OpL, jc_tkn_kind OpR)
{
 if (OpL >= JcTknCount || OpR >= JcTknCount)
 {
  return 0;
 }

 uint8_t BpL = JcBpTab[OpL].Y;
 uint8_t BpR = JcBpTab[OpR].X;

 return BpR > BpL;
}

static jc_tkn_arr *
JcLex(ar *Ar, ar *TknAr, char *Mem, size_t Ln)
{
 jc_tkn_arr *TknArr = ArPush(Ar, jc_tkn_arr, 1);
 TknArr->Ar = TknAr;

 a8 Slice = A8(Mem, Ln);
 while (Slice.Ln)
 {
  jc_tkn Tkn = {0};
  uint32_t Res = (
   A8EatPunctuatorMut(&Slice, &Tkn) ||
   A8EatHwsMut(&Slice, &Tkn) ||
   A8EatVwsMut(&Slice, &Tkn) ||
   A8EatIdentMut(&Slice, &Tkn) ||
   A8EatNumMut(&Slice, &Tkn)
  );
  if (Res)
  {
   TknArrPush(TknArr, &Tkn);
  }
  else
  {
   puts("failed");
   break;
  }
 }

 return TknArr;
}

static jc_tkn *
JcExprRecursive(jc_tkn_arr *TknView, jc_tkn_kind OpL)
{
 jc_tkn *Lhs = JcTknArrEatRelevant(TknView);

 // is Lhs a prefix?
 jc_tkn_kind PrefixKind = JcPrefixTab[Lhs->Kind];
 if (PrefixKind)
 {
  Lhs->Kind = PrefixKind;
  jc_tkn *Rhs = JcExprRecursive(TknView, Lhs->Kind);
  Lhs->First = Rhs;
 }
 else if (Lhs->Kind != JcTknNum)
 {
  puts("Error wrong start tkn");
  return 0;
 }

 for (;;)
 {
  jc_tkn *Op = JcTknArrPeekRelevant(TknView);
  if (Op->Kind == JcTknEof)
  {
   break;
  }

  if (JcOpInfixRightBindsTighter(OpL, Op->Kind))
  {
   JcTknArrEatRelevant(TknView);

   // postfix unary or binary?
   if (Op->Kind == JcTknPostfixIncrement || Op->Kind == JcTknPostfixDecrement)
   {
    // todo proper inserts
    Op->First = Lhs;
    Lhs = Op;
   }
   else
   {
    jc_tkn *Rhs = JcExprRecursive(TknView, Op->Kind);
    // todo proper inserts
    Op->First = Lhs;
    Lhs->Next = Rhs;
    Lhs = Op;
   }
  }
  else
  {
   break;
  }
 }
 return Lhs;
}

#ifdef STANDALONE
int
wmain(int Argc, wchar_t **Argv)
{
 OS_Init(&OS_W32State);
 JcBpTabInit();
 JcTknTabInit();
 JcPrefixTabInit();

 uint32_t FileLn = 0;
 char *File = DebugReadFile(L".\\sample.c", &FileLn);
 if (!File || !FileLn)
 {
  return 1;
 }

 ar *Ar = ArAlloc();
 ar *TknAr = ArAlloc();
 jc_tkn_arr *TknArr = JcLex(Ar, TknAr, File, FileLn);

 jc_tkn_arr TknView = *TknArr;
 jc_tkn *Res = JcExprRecursive(&TknView, JcTknHws);

 puts("done");

 a8 A = ArPushA8(Ar, 100000);
 JcTknPrint(A, Res);
 printf("%s", A.Mem);

 return 0;
}
#endif
