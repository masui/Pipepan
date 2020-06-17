//
//	$Date: 2002/08/20 12:04:28 $
//	$Revision: 1.1.1.1 $
//
#include "search.h"

static int mismatch;
static unsigned int epsilon;
static unsigned int acceptpat;
static unsigned int shiftpat[MAXCHAR];

void makepat(unsigned char *pat, int m)
{
	int i;
	unsigned int mask = INITPAT;

// printf("makepat(<%s>,%d)\n",pat,m);
	mismatch = m;
	epsilon = 0;

	for(i=0;i<MAXCHAR;i++) shiftpat[i] = 0;
	for(;*pat;pat++){
		if(*pat == ' '){ // ワイルドカード文字
			epsilon |= mask;
		}
		else if (*pat == '['){
			for(pat++;*pat != ']';pat++){
				shiftpat[*pat] |= mask;
			}
			mask >>= 1;
		}
		else {
			shiftpat[*pat] |= mask;
			mask >>= 1;
		}
	}
	acceptpat = mask;
}

int match(register unsigned char *text)
{
	register unsigned int i0 = INITPAT, i1=0, i2=0, i3=0;
	register unsigned int mask;
	register unsigned int e = epsilon;
// char *t = text;

	for(;*text;text++){
		mask = shiftpat[*text];
		i3 = (i3 & e) | ((i3 & mask) >> 1) | (i2 >> 1) | i2;
		i2 = (i2 & e) | ((i2 & mask) >> 1) | (i1 >> 1) | i1;
		i1 = (i1 & e) | ((i1 & mask) >> 1) | (i0 >> 1) | i0;
		i0 = (i0 & e) | ((i0 & mask) >> 1);
		i1 |= (i0 >> 1);
		i2 |= (i1 >> 1);
		i3 |= (i2 >> 1);
	}
// printf("match(%s),%d\n",t,i0&acceptpat);
	switch(mismatch){
		case 0: return (i0 & acceptpat);
		case 1: return (i1 & acceptpat);
		case 2: return (i2 & acceptpat);
		case 3: return (i3 & acceptpat);
		default: return 0;
	}
}
