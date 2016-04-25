//  BWUTest.cpp
//  A simple unit test class
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWUTest.h"

BWUTest::BWUTest( const char * tstr ) {
	init(tstr);
}

void BWUTest::init( const char * tstr ) {
	_tstr = tstr;
	_pass_count = _fail_count = 0;
}

void BWUTest::test ( const char * description, const int flag ) {
	const char * pf = nullptr;
	if (flag) {
		pf = _pstr;
		++_pass_count;
	} else {
		pf = _fstr;
		++_fail_count;
	}
	printf("%s: %s -> %s\n", _tstr, description, pf);
}

void BWUTest::report() const {
	printf("%s: pass: %ld, fail: %ld\n", _tstr, _pass_count, _fail_count);
}

