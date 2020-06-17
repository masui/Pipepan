#
#	$Date: 2002/08/20 12:03:58 $
#	$Revision: 1.1.1.1 $
#
#	片手キーボードの実験
#
#	(asdfキー使用)
#	pipepan2:	GL版
#	pipepan3:	UNIX TTY版
#	pipepan4:	pipepan2のOpenGL版
#
#	(テンキー使用)
#	pipepan5:	OpenGL版
#

# CFLAGS = -Xcpluscomm -g
CFLAGS = -I/usr/X11R6/include -DCOCOA

# GLLIBS = -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm
# GLLIBS = -L/lib -L/usr/local/lib -lglut32 -lglu32 -lopengl32
GLLIBS = -framework GLUT -framework OpenGL

pipepan6: pipepan6.o search.o graphics.o
	$(CC) -o pipepan6 *.o $(GLLIBS)
#	$(CC) -o pipepan6 pipepan6.o search.o graphics.o $(GLLIBS)
pipepan5: pipepan5.o search.o graphics.o
	$(CC) -o pipepan5 pipepan5.o search.o graphics.o $(GLLIBS)
pipepan4: pipepan4.o search.o
	$(CC) -o pipepan4 pipepan4.o search.o $(GLLIBS)
pipepan3: pipepan3.o search.o ttymodes.o
	$(CC) -o pipepan3 pipepan3.o search.o ttymodes.o
pipepan2: pipepan2.o search.o
	$(CC) -o pipepan2 pipepan2.o search.o -lfm -lgl

all: pipepan5 windows word.pp5 phrase.pp5
windows: pipepan5x.c
pipepan5x.c: pipepan5.c
	/user/masui/TVsearch/remote2/kanjiconv pipepan5.c > pipepan5x.c

onehand: onehand.o ttymodes.o
	$(CC) -o onehand onehand.o ttymodes.o
#POBOXDIC = /user/masui/POBox/word.bak
POBOXDIC = /user/masui/POBox/word.new
POBOXPHRASEDIC = /user/masui/POBox/phrase.chasen
word.pp:
	pipepandic < ${POBOXDIC} > word.pp
phrase.pp:
	pipepanphrase < ${POBOXPHRASEDIC} > phrase.pp
word.pp5:
	pipepan5dic < ${POBOXDIC} | uniq > word.pp5
phrase.pp5:
	pipepan5phrase < ${POBOXPHRASEDIC} | uniq > phrase.pp5
clean:
	\rm -f -r pipepan? pipepan?.exe *.o core #* *~
