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

 printf("%zd failed\n", FailCnt);

 int (*bar)() = (int (*)())boo;
 (int (*(*)(double (*)(int, char)))(long, ...))0;
 (int (*(*(*)(double, char *))[5])(long, ...))0;
 (int (* const volatile) [5])0;
 (int (*****)())0;
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