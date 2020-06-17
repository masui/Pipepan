#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

void moveto(int x, int y);
void rectangle(int x1, int y1, int x2, int y2);
void line(int x1, int y1, int x2, int y2);
void setasciifont(void *font);
float printstr(float x, float y, unsigned char *s, float limitwidth);

#endif
