#include "jc.c"



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
 JcTknTypeCast,

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
 JcTknTernary,
 JcTknMemberAccess,
 JcTknBitwiseNot,
 JcTknLogicalNot,

 JcTknAdd,
 JcTknUnaryAdd,
 JcTknSubtract,
 JcTknUnarySubtract,

 JcTknMultiply,
 JcTknDereference,
 JcTknPointer,

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
 // Length in elements, not bytes
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
 JcBpTab[JcTknLBrack] = V2U8(31, 0);
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

 JcBpTab[JcTknTypeCast] = V2U8(0, 27);

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

 JcBpTab[JcTknTernary] = V2U8(6, 5);

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
 JcTknTab[JcTknTernary] = CStr("?");
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

// 
// Tkn Arr API
// 

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
  Ret = TknArr->Mem;
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
 jc_tkn *Ret;
 while ((Ret = JcTknArrEat(TknArr)) && !JcTknKindIsExprRelevant(Ret->Kind))
 {
 }
 return Ret;
}

// todo maybe this could be an optimisation over jctknarreatrelevant but I might make eatrelevant handle attaching whitespace in the future
// static jc_tkn *
// JcTknArrEatToTkn(jc_tkn_arr *TknArr, jc_tkn *Tkn)
// {
//  jc_tkn *Ret = &JcTknGlobalEof;
//  if (TknArr->Ln && Tkn >= TknArr->Mem && Tkn < (TknArr->Mem + TknArr->Ln))
//  {
//   Ret = Tkn;
//   size_t Diff = Tkn - TknArr->Mem;
//   TknArr->Mem += Diff;
//   TknArr->Ln -= Diff;
//  }
//  return Ret;
// }

static jc_tkn *
JcTknArrEatIfKind(jc_tkn_arr *TknArr, jc_tkn_kind Kind)
{
 jc_tkn *Peek = JcTknArrPeekRelevant(TknArr);
 if (Peek->Kind == Kind)
 {
  JcTknArrEatRelevant(TknArr);
  return Peek;
 }
 return &JcTknGlobalEof;
}

static void
JcA8PrintMut(a8 *A, a8 Str)
{
 int WriteLn = sprintf_s(A->Mem, A->Ln, "%.*s", (int)Str.Ln, Str.Mem);
 A8ShlMut(A, WriteLn);
}

static void
JcTknPrintMut(a8 *A, jc_tkn *Tkn)
{
 if (!Tkn) return;
 a8 TknStr;
 switch(Tkn->Kind)
 {
  case JcTknTypeCast:
  {
   TknStr = CStr("cast");
  } break;
  default: 
  {
   TknStr = A8(Tkn->Mem, Tkn->Ln);
  } break;
 }

 if (Tkn->First && Tkn->First->Next && Tkn->First->Next->Next)
 {
  // ternary
  JcA8PrintMut(A, CStr("("));
  JcA8PrintMut(A, TknStr);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First->Next);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First->Next->Next);
  JcA8PrintMut(A, CStr(")"));
 }
 else if (Tkn->First && Tkn->First->Next)
 {
  // binary op
  JcA8PrintMut(A, CStr("("));
  JcA8PrintMut(A, TknStr);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First->Next);
  JcA8PrintMut(A, CStr(")"));
 }
 else if (Tkn->First)
 {
  // unary op
  JcA8PrintMut(A, CStr("("));
  JcA8PrintMut(A, TknStr);
  JcA8PrintMut(A, CStr(" "));
  JcTknPrintMut(A, Tkn->First);
  JcA8PrintMut(A, CStr(")"));
 }
 else
 {
  // atomic
  JcA8PrintMut(A, A8(Tkn->Mem, Tkn->Ln));
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

 v2u8 BpL = JcBpTab[OpL];
 v2u8 BpR = JcBpTab[OpR];
 // both should have at least one set, let JcTknEof through
 if ((OpL == JcTknEof || (BpL.X || BpL.Y)) && (BpR.X || BpR.Y))
 {
  return BpR.X > BpL.Y;
 }
 return 0;
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

static uint32_t
StrIsType(char *Mem, size_t Ln)
{
 uint32_t Ret = 0;
 a8 Types[] =
 {
  CStr("unsigned"),
  CStr("char"),
  CStr("short"),
  CStr("int"),
  CStr("long"),
  CStr("float"),
  CStr("double"),
  CStr("struct"),
  CStr("enum"),
 };

 for (size_t I = 0; I < ArrLen(Types); ++I)
 {
  if (StrEq(Mem, Ln, Types[I].Mem, Types[I].Ln))
  {
   Ret = 1;
   break;
  }
 }
 return Ret;
}

static uint32_t
StrIsPtrQualifier(char *Mem, size_t Ln)
{
 uint32_t Ret = 0;
 a8 Types[] =
 {
  CStr("const"),
  CStr("volatile"),
  CStr("restrict"),
 };

 for (size_t I = 0; I < ArrLen(Types); ++I)
 {
  if (StrEq(Mem, Ln, Types[I].Mem, Types[I].Ln))
  {
   Ret = 1;
   break;
  }
 }
 return Ret;
}

static jc_tkn *
JcExprRecursive(jc_tkn_arr *TknView, jc_tkn_kind OpL);

static jc_tkn *
JcEatTypeChain(jc_tkn_arr *TknView)
{
 jc_tkn *Prev = 0;
 jc_tkn *Cur = 0;
 while ((Cur = JcTknArrPeekRelevant(TknView)) && ((Cur->Kind == JcTknIdent && StrIsType(Cur->Mem, Cur->Ln)) || (Prev && Cur->Kind == JcTknMultiply)))
 {
  if (Cur->Kind == JcTknMultiply)
  {
   Cur->Kind = JcTknPointer;
  }
  JcTknArrEatRelevant(TknView);
  Cur->First = Prev;
  Prev = Cur;
 }
 return Prev;
}

// eat all const / volatile / restrict
static jc_tkn *
JcEatPtrChain(jc_tkn_arr *TknView)
{
 jc_tkn *Prev = 0;
 jc_tkn *Cur = 0;
 while ((Cur = JcTknArrPeekRelevant(TknView))
  && (((Prev && Cur->Kind == JcTknIdent && StrIsPtrQualifier(Cur->Mem, Cur->Ln))
  || Cur->Kind == JcTknMultiply)))
 {
  if (Cur->Kind == JcTknMultiply)
  {
   Cur->Kind = JcTknPointer;
  }
  JcTknArrEatRelevant(TknView);
  Cur->First = Prev;
  Prev = Cur;
 }
 return Prev;
}


static jc_tkn *
JcEatNestedPtr(jc_tkn_arr *TknView)
{
 jc_tkn *LParen = JcTknArrEatIfKind(TknView, JcTknLParen);
 if (LParen->Kind == JcTknLParen)
 {
  jc_tkn *PtrChain = JcEatPtrChain(TknView);
  if (PtrChain)
  {
   jc_tkn *PtrChainBottom = PtrChain;
   while (PtrChainBottom->First) PtrChainBottom = PtrChainBottom->First;
   jc_tkn *Peek = JcTknArrPeekRelevant(TknView);
   switch (Peek->Kind)
   {
    case JcTknRParen:
    {
     JcTknArrEatRelevant(TknView);

     // check for param list and brack list
     jc_tkn *SuffixPeek = JcTknArrPeekRelevant(TknView);
     switch (SuffixPeek->Kind)
     {
      case JcTknLBrack:
      {
       JcTknArrEatRelevant(TknView);
       jc_tkn *Expr = JcExprRecursive(TknView, JcTknEof);
       if (Expr->Kind != JcTknEof) //todo error detection?
       {
        PtrChainBottom->First = SuffixPeek;
        SuffixPeek->First = Expr;
        if (JcTknArrEatIfKind(TknView, JcTknRBrack)->Kind != JcTknRBrack)
        {
         puts("Error expected closing bracket");
        }
       }
       else
       {
        puts("Error expected expression");
       }
      } break;
      case JcTknLParen:
      {
       puts("error unimplemented");
      } break;
      default:
      {
       // do nothing
      } break;
     }
     return PtrChain;
    } break;
    case JcTknLParen:
    {
     return JcEatNestedPtr(TknView);
    } break;
    default: 
    {
     puts("Error expected closing paren");
    } break;
   }
  }
  else
  {
   puts("Error expected pointer");
  }
 }
 return 0;
}

// (typechain (optnestedptr) optparamlist optbracklist)
static jc_tkn *
JcEatTypeCast(jc_tkn_arr *TknView, jc_tkn *LParen)
{
 LParen->Kind = JcTknTypeCast;
 jc_tkn *TypeChain = JcEatTypeChain(TknView);
 jc_tkn *NestedPtr = JcEatNestedPtr(TknView);
 if (NestedPtr)
 {
  // get outermost ptr to append typechain to
  jc_tkn *LastPtr = NestedPtr;
  while (LastPtr->First && LastPtr->First->Kind != JcTknLParen && LastPtr->First->Kind != JcTknLBrack) LastPtr = LastPtr->First;
  SLLStackPush(LastPtr->First, TypeChain);
  LParen->First = NestedPtr;
 }
 else
 {
  LParen->First = TypeChain;
 }
 return LParen;
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
  //todo proper insert
  Lhs->First = Rhs;
 }
 else if (Lhs->Kind == JcTknLParen)
 {
  // check if type cast or expression grouping
  jc_tkn *Peek = JcTknArrPeekRelevant(TknView);
  if (StrIsType(Peek->Mem, Peek->Ln))
  {
   Lhs = JcEatTypeCast(TknView, Lhs);
   jc_tkn *RParen = JcTknArrEatRelevant(TknView); //todo this may need to be a peek then eat
   if (RParen->Kind != JcTknRParen)
   {
    puts("Error no closing paren found");
    return 0;
   }
   jc_tkn *Rhs = JcExprRecursive(TknView, Lhs->Kind);
   SLLStackPush(Lhs->First, Rhs);
  }
  else
  {
   Lhs = JcExprRecursive(TknView, JcTknEof);
   jc_tkn *RParen = JcTknArrEatRelevant(TknView);
   if (RParen->Kind != JcTknRParen)
   {
    puts("Error no closing paren found");
    return 0;
   }
  }
 }
 else if (Lhs->Kind == JcTknIdent)
 {
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

   // postfix unary, arr subscript, or binary?
   if (Op->Kind == JcTknPostfixIncrement || Op->Kind == JcTknPostfixDecrement)
   {
    // todo proper inserts
    Op->First = Lhs;
    Lhs = Op;
   }
   else if (Op->Kind == JcTknLBrack)
   {
    jc_tkn *Rhs = JcExprRecursive(TknView, JcTknEof);
    jc_tkn *RBrack = JcTknArrEatRelevant(TknView);
    if (RBrack->Kind == JcTknRBrack)
    {
     // todo proper inserts
     Op->First = Lhs;
     Lhs->Next = Rhs;
     Lhs = Op;
    }
    else
    {
     puts("Error expected closing bracket");
     return 0;
    }
   }
   else if (Op->Kind == JcTknTernary)
   {
    jc_tkn *Mhs = JcExprRecursive(TknView, JcTknEof);
    jc_tkn *Colon = JcTknArrEatRelevant(TknView);
    if (Colon->Kind == JcTknColon)
    {
     jc_tkn *Rhs = JcExprRecursive(TknView, Op->Kind);
     // todo proper inserts
     Op->First = Lhs;
     Lhs->Next = Mhs;
     Mhs->Next = Rhs;
     Lhs = Op;
    }
    else
    {
     puts("Error expected colon for ternary");
     return 0;
    }
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
 OsInit(&OS_W32State);
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
 jc_tkn *Res = JcExprRecursive(&TknView, JcTknEof);

 puts("done");

 a8 A = ArPushA8(Ar, 100000);
 JcTknPrint(A, Res);
 printf("%s", A.Mem);

 return 0;
}
#endif
