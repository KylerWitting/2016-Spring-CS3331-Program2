CC       = gcc

OBJ_FILE  = main.o qsort.o merge.o 
EXE_FILE  = main qsort merge 
PROG 	  = main.c qsort.c merge.c 
TAGS      = -lm -o

all: ${EXE_FILE}

main: main.c 
	gcc main.c -o main 

qsort: qsort.c 
	gcc qsort.c -o qsort 

merge: merge.c 
	gcc merge.c -o merge 

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
