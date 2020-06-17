/*
	$Date: 2002/08/20 12:04:28 $
	$Revision: 1.1.1.1 $
*/
#ifndef _SEARCH_H_
#define _SEARCH_H_

#define MAXCHAR 0x100
#define INITPAT 0x4000
#define MAXMISMATCH 3

void makepat(unsigned char *pat, int m);
int match(unsigned char *text);

#define Match(x) match(x)
#define MakePat(x,y) makepat(x,y)

#endif
