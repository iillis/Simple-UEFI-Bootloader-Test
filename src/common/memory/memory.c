#include "memory.h"

VOID *Memcpy(VOID *D1, CONST VOID *S1, UINTN n) {
    UINT8 *D2 = (UINT8 *)D1;
    CONST UINT8 *S2 = (CONST UINT8 *)S1;

    for (UINTN i = 0; i < n; i++) {
        D2[i] = S2[i];
    }

    return D1;
}

VOID *Memset(VOID *S1, INT c, UINTN n) {
    UINT8 *p = (UINT8 *)S1;

    for (UINTN i = 0; i < n; i++) {
        p[i] = (UINT8)c;
    }

    return S1;
}
VOID *Memmove(VOID *D1, CONST VOID *S1, UINTN n) {
    UINT8 *D2 = (UINT8 *)D1;
    CONST UINT8 *PrimaryS1 = (CONST UINT8 *)S1;

    if (S1 > D1) {
        for (UINTN i = 0; i < n; i++) {
            D2[i] = PrimaryS1[i];
        }
    } else if (S1 < D1) {
        for (UINTN i = n; i > 0; i--) {
            D2[i-1] = PrimaryS1[i-1];
        }
    }

    return D1;
}

INT Memcmp(CONST VOID *S1, CONST VOID *S2, UINTN n) {
    CONST UINT8 *p1 = (CONST UINT8 *)S1;
    CONST UINT8 *p2 = (CONST UINT8 *)S2;

    for (UINTN i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

INT Strncmp(CONST CHAR8 *S1, CONST CHAR8 *S2, UINTN n) {
    for (UINTN i = 0; i < n; i++) {
        CHAR8 C1 = S1[i], C2 = S2[i];
        if (C1 != C2)
            return C1 < C2 ? -1 : 1;
        if (!C1)
            return 0;
    }

    return 0;
}

INT Strcmp(CONST CHAR8 *S1, CONST CHAR8 *S2) {
  INT Ret = 0;
  while (!(Ret = *(CHAR8 *) S1 - *(CHAR8 *) S2) && *S2) ++S1, ++S2;

  if (Ret < 0) {
    Ret = -1;
  } else if (Ret > 0) {
    Ret = 1 ;
  }

  return Ret;
}

CHAR8 * Strrev(CHAR8 String[]) {
	INT i, j;
	CHAR8 Temp;
	
	for (i=0, j=Strlen(String)-1 ; i<j ; i++, j--) {
		Temp = String[i];
		String[i] = String[j];
		String[j] = Temp;	
	}	

	return String;
}

CHAR16 * Strrev16(CHAR16 String[]) {
	INT i, j;
	CHAR16 Temp;
	
	for (i=0, j=Strlen16(String)-1 ; i<j ; i++, j--) {
		Temp = String[i];
		String[i] = String[j];
		String[j] = Temp;	
	}	

	return String;
}

UINTN Strlen(const CHAR8 *str) {
	register const CHAR8 *s;

	for (s = str; *s; ++s);
	return(s - str);
}

UINTN Strlen16(const CHAR16 *str) {
	register const CHAR16 *s;

	for (s = str; *s; ++s);
	return(s - str);
}