all: mycp

mycp: mycp.c
	gcc -g -Wall -o mycp mycp.c

clean:
	\rm mycp
