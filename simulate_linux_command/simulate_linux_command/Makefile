all:test demo
test:test.o ComRmind.o ExtCom.o ComSplit.o IntCom.o
	gcc test.o ComRmind.o ExtCom.o ComSplit.o IntCom.o -o test
demo:demo.o
	gcc -o demo demo.o
demo.o:demo.c
	gcc -g -c demo.c
test.o:test.c Myshell.h
	gcc -g -c test.c 

ComRmind.o:ComRmind.c Myshell.h
	gcc -g -c ComRmind.c

ExtCom.o:ExtCom.c Myshell.h
	gcc -g -c ExtCom.c

IntCom.o:IntCom.c Myshell.h
	gcc -g -c IntCom.c

ComSplit.o:ComSplit.c Myshell.h
	gcc -g -c ComSplit.c

clean:
	rm -rf *.o *.c~ *.c~~ *.c~~ *.h~ *.h~~ test demo