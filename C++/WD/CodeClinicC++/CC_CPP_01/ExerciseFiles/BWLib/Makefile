# make file for BWLib
# by Bill Weinman <http://bw.org/>
# Copyright (c) 2014 The BearHeart Group LLC
#

TARGET=BWLib.a
CC=c++
CPPFLAGS=-O3 -std=gnu++11
LDFLAGS=-L. -lsqlite3

all: test.out

BWUTest.o: BWUTest.h BWUTest.cpp
	$(CC) $(CPPFLAGS) -c BWUTest.cpp

BWDB.o: BWDB.h BWDB.cpp
	$(CC) $(CPPFLAGS) -c BWDB.cpp

BWKV.o: BWKV.h BWKV.cpp
	$(CC) $(CPPFLAGS) -c BWKV.cpp

BWString.o: BWString.h BWString.cpp
	$(CC) $(CPPFLAGS) -c BWString.cpp

BWNumber.o: BWNumber.h BWNumber.cpp
	$(CC) $(CPPFLAGS) -c BWNumber.cpp

BWCGI.o: BWCGI.h BWCGI.cpp
	$(CC) $(CPPFLAGS) -c BWCGI.cpp

BWWebGet.o: BWWebGet.h BWWebGet.cpp
	$(CC) $(CPPFLAGS) -c BWWebGet.cpp

$(TARGET): BWUTest.o BWDB.o BWKV.o BWString.o BWNumber.o BWCGI.o BWWebGet.o
	ar rcs $(TARGET) BWUTest.o BWDB.o BWKV.o BWString.o BWNumber.o BWCGI.o BWWebGet.o

test.out: $(TARGET) main.cpp
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o test.out main.cpp $(TARGET)

clean:
	rm *.o test.out $(TARGET)

