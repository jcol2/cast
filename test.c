#include "cast.c"

static uint32_t
TestExprRecursive(size_t *FailCnt, a8 Input, a8 Expect)
{
 printf("Start %.*s..........", (int)Input.Ln, Input.Mem);
 fflush(stdout);
 ar_tmp Tmp = ScratchGet(0, 0);
 jc_tkn_arr *TknArr = JcLex(Tmp.Ar, Tmp.Ar, Input.Mem, Input.Ln);

 jc_tkn_arr TknView = *TknArr;
 jc_tkn *Res = JcExprRecursive(&TknView, JcTknEof);

 a8 ResStr = ArPushA8(Tmp.Ar, 100000);
 JcTknPrint(ResStr, Res);

 ScratchEnd(Tmp);
 uint32_t Ret = StrEq(ResStr.Mem, strlen(ResStr.Mem), Expect.Mem, Expect.Ln);
 if (Ret)
 {
  printf("OK\n");
 }
 else
 {
  (*FailCnt)++;
  printf("FAIL, expected: %.*s, got: %.*s\n", (int)Expect.Ln, Expect.Mem, (int)ResStr.Ln, ResStr.Mem);
 }
 return Ret;
}

int foo(){return 0;}
int boo(){return 0;}

static void
Test()
{
 size_t FailCnt = 0;
 TestExprRecursive(&FailCnt, CStr("foo + 1 * 3"), CStr("(+ foo (* 1 3))"));
 TestExprRecursive(&FailCnt, CStr("5 * 1 + 3"), CStr("(+ (* 5 1) 3)"));
 TestExprRecursive(&FailCnt, CStr("5 * 6 + 1 * 3"), CStr("(+ (* 5 6) (* 1 3))"));
 TestExprRecursive(&FailCnt, CStr("++5-- + +3"), CStr("(+ (++ (-- 5)) (+ 3))"));
 TestExprRecursive(&FailCnt, CStr("(((3)))"), CStr("3"));
 TestExprRecursive(&FailCnt, CStr("10[1]"), CStr("([ 10 1)"));
 TestExprRecursive(&FailCnt, CStr("1 ? 100 : 5 ? 2 : 3"), CStr("(? 1 100 (? 5 2 3))"));
 TestExprRecursive(&FailCnt, CStr("+ - + - + 3"), CStr("(+ (- (+ (- (+ 3)))))"));
 TestExprRecursive(&FailCnt, CStr("(long long)3 + (unsigned int)5"), CStr("(+ (long (long 3)) (int (unsigned 5)))"));
 TestExprRecursive(&FailCnt, CStr("(long *)3 + (unsigned *)5"), CStr("(+ (* (long 3)) (* (unsigned 5)))"));
 TestExprRecursive(&FailCnt, CStr("(long (*))3"), CStr("(* (long 3))"));

 printf("%zd failed\n", FailCnt);

 int (*bar)() = (int (*)())boo;
 (int (*(*)(double (*)(int, char)))(long, ...))0;
 (int (*(*(*)(double, char *))[5])(long, ...))0;
 (int (* const volatile) [5])0;
 (int (*****)())0;
 (int (*(*))())0;
 (int (* const))0;
//  1. Pointer to function returning pointer to array of function pointers
// (int (*(*f1)(double, char *))[5])(long, ...)
// 2. Cast involving const/volatile qualifiers and nested function pointers
// (void (*(* const volatile)(int (*)(char, short)))(float, double))(int (*)(char, short))
// 3. Function pointer returning another function pointer with array parameter
// char *(*(*f3)(int, char (*)[10]))(double (*)(long), ...)
// 4. Deeply nested pointer-to-function returning pointer-to-function
// int (*(*(*f4)(void))(int (*)(int)))(char, double)
// 5. Function pointer with multiple levels of arrays and pointers
// double (*(*f5[3])(int (*)(int, int)))[7]
// 6. Cast of a function pointer taking another function pointer returning array
// (int (*)(char *(*)(int, int (*)(double)) )) f6
// 7. Variadic function pointer returning pointer to const function pointer
// const int (*(*f7)(int, ...))(char, long)
// 8. Pointer to function returning pointer to function returning pointer
// void *(*(*(*f8)(int))(double))(char *)
// 9. Function pointer taking array of function pointers
// int (*f9)(int (*[4])(double, double))
// 10. Cast with typedef-like complexity inline
// (long (*(*)(short, int (*)(int)))(char, long (*)(double)))(short, int (*)(int))
// 11. Pointer to function returning pointer to array of pointers to functions
// int (*(*f11)(void))[3][4]
// 12. Function pointer with restrict-qualified pointer arguments (C99+)
// void (*f12)(int *restrict, double *restrict, void (*)(int))
// 13. Function returning pointer to function returning array pointer
// int (*(*f13(void))(double))[5]
// 14. Insane cast expression (good for parser edge cases)
// ((int (*(*)(double (*)(int, char)))(long, ...))0)
// 15. Mixed qualifiers, arrays, and function pointers
// volatile int (*(*const f15[2])(char (*)(int)))(double, float)
}

int
wmain(int Argc, wchar_t **Argv)
{
 OS_Init(&OS_W32State);
 JcBpTabInit();
 JcTknTabInit();
 JcPrefixTabInit();

 Test();

 return 0;
}