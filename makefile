all:	test1 test2 test3

SANITIZE=
# SANITIZE=-fsanitize=undefined
# SANITIZE=-fsanitize=address
CFLAGS=-W -Wall -Wextra -O3 ${SANITIZE}
#CFLAGS=-W -Wall -Wextra -g

test1:	test1.c open-simplex-noise.o pngwriter.o
	${CC} ${CFLAGS} -o test1 open-simplex-noise.o pngwriter.o test1.c -lpng

test2:	test2.c open-simplex-noise.o pngwriter.o
	${CC} ${CFLAGS} -o test2 open-simplex-noise.o pngwriter.o test2.c -lpng

test3:	test3.c open-simplex-noise.o pngwriter.o
	${CC} ${CFLAGS} -o test3 open-simplex-noise.o pngwriter.o test3.c -lpng

open-simplex-noise.o:	open-simplex-noise.h open-simplex-noise.c makefile
	${CC} ${CFLAGS} -c open-simplex-noise.c

pngwriter.o:	pngwriter.h pngwriter.c makefile
	${CC} ${CFLAGS} -c pngwriter.c

clean:
	rm -f open-simplex-noise.o pngwriter.o
	rm -f out/*.png
	rm test1 test2 test3

