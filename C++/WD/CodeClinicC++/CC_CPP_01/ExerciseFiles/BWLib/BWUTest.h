//  BWUTest.h
//  A simple unit test class
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLib__BWUTest__
#define __BWLib__BWUTest__

#include <cstdio>

#define __BWUTest_VERSION "1.0.0"

const static char * _pstr = "pass";
const static char * _fstr = "fail";

class BWUTest {
	unsigned long int _pass_count = 0;
	unsigned long int _fail_count = 0;

	const char * _tstr = nullptr;
	
	BWUTest( BWUTest & );					// no copy constructor
	BWUTest operator = ( BWUTest & );		// no assignment operator
	BWUTest(){}								// no default constructor
public:
	static const char * version() { return __BWUTest_VERSION; }

	BWUTest( const char * );
	void init ( const char * );
	void test ( const char * description, const int flag );
	void report() const;
};

#endif /* defined(__BWLib__BWUTest__) */
