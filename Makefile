SHELL = /usr/bin/env bash
NAME = mytftpclient
LOGIN = xgawro00
#---------------
all: 
	g++ -std=c++17 *.cpp -o mytftpclient.o 

zip:
	tar -zcvf $(LOGIN).tar.gz *.cpp *.h *.md Makefile

clean:
	rm -f *.o
