COMPILER = g++
FILE_NAME = main.cpp

all: main

main: main.cpp
	$(COMPILER) $(FILE_NAME) -std=c++11

clean: 
	rm a.exe