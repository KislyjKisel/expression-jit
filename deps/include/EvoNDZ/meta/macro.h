#pragma once

#define EVO_COMMA() ,

#define EVO_REPEAT1(X) X
#define EVO_REPEAT2(X) X X
#define EVO_REPEAT3(X) X X X
#define EVO_REPEAT4(X) X X X X
#define EVO_REPEAT5(X) X X X X X
#define EVO_REPEAT6(X) X X X X X X
#define EVO_REPEAT7(X) X X X X X X X
#define EVO_REPEAT8(X) X X X X X X X X
#define EVO_REPEAT9(X) EVO_REPEAT_8(X) X
#define EVO_REPEAT10(X) EVO_REPEAT_9(X) X
#define EVO_REPEAT11(X) EVO_REPEAT_10(X) X
#define EVO_REPEAT12(X) EVO_REPEAT_11(X) X
#define EVO_REPEAT13(X) EVO_REPEAT_12(X) X
#define EVO_REPEAT14(X) EVO_REPEAT_13(X) X
#define EVO_REPEAT15(X) EVO_REPEAT_14(X) X
#define EVO_REPEAT16(X) EVO_REPEAT_15(X) X

#define EVO_REPEAT(N, X) EVO_REPEAT##N(X)