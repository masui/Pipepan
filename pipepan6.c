//
//	$Date: 2002/08/20 12:04:28 $
//	$Revision: 1.1.1.1 $
//
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <GLUT/glut.h>
#include "search.h"
#include "graphics.h"

void display(void);
void reshape(int w, int h);
void search();

void SendToActiveWin(unsigned char *p); ////

//
//	辞書関連
//
#define WORDDIC "word.pp" // 曖昧検索辞書
#define MAXWORDS 40000
unsigned char *word[MAXWORDS],*yomi[MAXWORDS];
int nwords = 0;

#define PHRASEDIC "phrase.pp" // 例文辞書
#define MAXPHRASES 40000
unsigned char *pcontext[MAXPHRASES];
unsigned char *pword[MAXPHRASES];
unsigned char *pyomi[MAXPHRASES];
int nphrases = 0;
//
//	候補関連
//
#define MAXLINELEN 1000
#define MAXCANDS 50
#define MAXWLEN 100
unsigned char cands[MAXCANDS][MAXWLEN];
int ncands = 0;
int curcand = -1;  // 現在選択されている候補のインデクス
char outbuf[1000] = "";  // 出力文字列
int outbuflen = 0;
int begin = 0;
//
//	文字入力関連
//
char pat[100] = "";
int exact = 0;

// Windows
#ifdef WINDOWS
#include <windows.h>

// ターゲットウィンドウに文字列を送信
HWND targetWin;
DWORD targetThread;
DWORD selfThread;
HWND activeWin;
#endif

void resetpat()
{
	strcpy(pat,"");
	exact = 0;
	curcand = -1;
}

void kbdfunc(unsigned char c, int x, int y)
{
	char buf[2] = " ";
	switch(c){
	case 'c': // パタンクリア
		resetpat();
		search();
		break;
	case 'z': // 確定/改行
		strcat(outbuf,cands[curcand]);
		resetpat();
		display();
		strcpy(outbuf,"");
		search();
		break;
	case 'v': // 完全一致検索指定
		if(! exact){
			exact = 1;
			search();
			curcand = 0;
		}
		break;
	case 'x': // 前候補/後退
		if(curcand >= 0){ // 前候補
			curcand--;
		}
		else { // 後退
			outbuflen = strlen(outbuf);
			if(outbuflen > 1) outbuf[outbuflen-2]='\0';
			resetpat();
			search();
		}
		break;
	case ' ': // 次候補
		curcand++;
		break;
	case 'a': case 's': case 'd': case 'f':
		if(curcand>=0 && strlen(pat) > 0){ // 前の候補を確定
			strcat(outbuf,cands[curcand]);
			resetpat();
		}
		buf[0] = c;
		strcat(pat,buf);
		search();
		break;
	case 'q':
		SendToActiveWin(outbuf);
		break;
	}
	display();
}

void search()
{
	int i,j;
	int plen,patlen;
	outbuflen = strlen(outbuf);
	patlen = strlen(pat);
	ncands = 0;
	for(i=0;i<nphrases && ncands < MAXCANDS-1;i++){
		plen = strlen(pcontext[i]);
		if(outbuflen >= plen &&
			 strncmp(pcontext[i],outbuf+outbuflen-plen,plen) == 0 &&
			 (exact ? strcmp(pat,pyomi[i]) : strncmp(pat,pyomi[i],patlen)) == 0){
			for(j=0;j<ncands;j++)
				if(strcmp(pword[i],cands[j])==0) break;
			if(j < ncands) continue;
			strcpy(cands[ncands++],pword[i]);
		}
	}
	for(i=0;i<nwords && ncands < MAXCANDS-1;i++){
		if((exact ? strcmp(pat,yomi[i]) : strncmp(pat,yomi[i],patlen)) == 0){
			for(j=0;j<ncands;j++)
				if(strcmp(word[i],cands[j])==0) break;
			if(j < ncands) continue;
			strcpy(cands[ncands++],word[i]);
		}
	}
}

void readdic()
{
	FILE *f;
	char linebuf[MAXLINELEN];
	char context[MAXWLEN],y[MAXWLEN],w[MAXWLEN];

	if((f = fopen(WORDDIC,"r")) == NULL) exit(0);
	while(fgets(linebuf,MAXLINELEN,f) && nwords < MAXWORDS){
		sscanf(linebuf,"%s %s",y,w);
		word[nwords] = strdup(w);
		yomi[nwords] = strdup(y);
		nwords++;
	}
	fclose(f);

	if((f = fopen(PHRASEDIC,"r")) == NULL) exit(0);
	while(fgets(linebuf,MAXLINELEN,f) && nphrases < MAXPHRASES){
		sscanf(linebuf,"%s %s %s",context,y,w);
		pcontext[nphrases] = strdup(context);
		pword[nphrases] = strdup(w);
		pyomi[nphrases] = strdup(y);
		nphrases++;
	}
	fclose(f);
}

main(int argc, char **argv)
{
	char c;
	int i,n;
	int x,y;

#ifdef WINDOWS
	targetWin = GetForegroundWindow();
	targetThread = GetWindowThreadProcessId(targetWin, NULL);
#endif

	readdic();

	glutInit(&argc, argv);
	glutInitWindowSize(550,170);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("One-hand KB");
	glEnable(GL_BLEND);

	glColor3f(1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pat[0] = '\0';

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(kbdfunc);
	glutMainLoop();
}

void
reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

void
display(void)
{
	int i;
	float x,y;
	float candx;

	//	glClearColor(0.5,0.5,0.75,1.0);
	glClearColor(0.1,0.7,0.2,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textbox(10,72,540,98,0xffffff);
	x = 14.0; y = 80.0;
	glColor3f(0.0,0.0,0.0);
	setasciifont(GLUT_BITMAP_HELVETICA_18);
	x += printstr(x,y,outbuf,1000.0);
	candx = x;

	if(curcand >= 0){
		glColor3f(0.0,0.0,1.0);
		x += printstr(x,y,cands[curcand],1000.0);
	}

	x = candx; y = 52;
	glColor3f(1.0,1.0,1.0);
	for(i=curcand+1;i<ncands && i < curcand+6;i++){
		x += printstr(x,y,cands[i],1000.0);
		x += 10.0;
	}
	x = candx; y = 112;
	glColor3f(1.0,1.0,1.0);
	for(i=curcand-1;i>=0 && i > curcand-6;i--){
		x += printstr(x,y,cands[i],1000.0);
		x += 10.0;
	}
	x = candx; y = 30;
	glColor3f(0.0,0.0,0.0);
//	printstr(x,y,aiupat,1000.0);
	glutSwapBuffers();
}

//////////////////////////////////////////////////////////

void SendToActiveWin(unsigned char *p)
{
#ifdef WINDOWS
	selfThread = GetCurrentThreadId();
	AttachThreadInput(selfThread, targetThread, TRUE);
	activeWin = GetFocus();

	for(;*p;p++){
		PostMessage(activeWin, WM_CHAR, *p, 1);
	}

	AttachThreadInput(selfThread, targetThread, FALSE);
#endif
}

