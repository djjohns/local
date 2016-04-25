//  main.cpp
//  Test bed for BWLIb
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include <cstdio>
#include <vector>
#include <memory>
#include <map>
#include "BWUTest.h"
#include "BWKV.h"
#include "BWString.h"
#include "BWNumber.h"
#include "BWWebGet.h"
#include "BWCGI.h"

// Here be dragons
const char * const database_filename = "/Users/billw/sqlite3_data/test_database.db";

int main(int argc, const char * argv[])
{
	std::unique_ptr<BWKV> db( new BWKV(database_filename) );
	int counter = 0;
	
	// Versions n things
	printf("BWUTest version: %s\n", BWUTest::version());
	printf("BWDB version: %s\n", BWDB::version());
	printf("BWKV version: %s\n", BWKV::version());
	printf("BWString version: %s\n", BWString::version());
	printf("BWNumber version: %s\n", BWNumber::version());
	printf("BWWebGet version: %s\n", BWWebGet::version());
	printf("BWCGI version: %s\n", BWCGI::version());
	printf("SQLite version: %s\n",db->get_query_value("SELECT sqlite_version()").c_str());
	printf("BWKV table version: %s\n", db->get_query_value("SELECT value FROM kv WHERE key = ?", {"db_version"}).c_str());

	// let's test BWDB & BWKV !!
	BWUTest u("BWDB/KV");
	printf("\nTesting BWDB & BWKV\n");

	if (!db->status()) {
		printf("database failed to open.\n");
		return 1;
	}

	u.test("database open", db->status());
	u.test("table_exists", db->table_exists("kv") && !db->table_exists("foo"));

	const char * s = db->value("db_version");
	u.test("db->value()", s != nullptr);
	
	if(!s) {
		printf("error: %s\n", db->get_error());
		return 1;
	}

	db->value("this", "that");
	u.test("db->value (set/get (that))", memcmp(db->value("this"), "that", 5) == 0);

	db->value("this", "other");
	u.test("db->value (set/get (other))", memcmp(db->value("this"), "other", 6) == 0);

	u.test("db->value (undefined)", db->value("foo") == nullptr);

	db->value("foo", "bar");
	db->prepare_query("SELECT * FROM kv", {});
	counter = 0;
	while (const BWDB::dbrow * r = db->get_prepared_row()) {
		for (auto col : *r ) {
			printf("%s:%s ", col.first.c_str(), col.second.c_str());
		}
		printf("\n");
		++counter;
	}
	u.test("prepared query", counter >= 3);

	printf("SELECT value FROM kv -- \n");
	db->prepare_query("SELECT value FROM kv", {});
	counter = 0;
	while (BWString rs = db->get_prepared_value()) {
		printf("rs is [%s]\n", rs.c_str());
		++counter;
	}
	u.test("prepared query value", counter >= 3);

	printf("there are %ld rows in the kv table.\n", db->count("kv"));
	u.test("db->count is 3", db->count("kv") == 3);
	
	db->del("foo");
	printf("there are %ld rows in the kv table.\n", db->count("kv"));
	u.test("after delete foo db->count is 2", db->count("kv") == 2);

	u.report();
	db.reset();
	printf("\n");

	// BWString
	printf("\nTesting BWString -----\n\n");
	u.init("BWString");
	
	const char * _ctest = "   \tfoo   \r\n";
	BWString ttest = _ctest;
	u.test("cstring ctor", ttest.length() == 12);
	ttest.trim();
	u.test("trim", ttest.length() == 3);
	
	BWString x = "foo";
	BWString y = "bar";
	BWString z = x + "baz" + y;

	u.test("concat", z.length() == 9 && memcmp(z.c_str(), "foobazbar", 10) == 0);

	x = y = "foo";
	u.test("foo == foo",   (x == y));
	u.test("foo  > foo",  !(x  > y));
	u.test("foo >= foo",   (x >= y));
	u.test("foo  < foo",  !(x  < y));
	u.test("foo <= foo",   (x <= y));

	x = "bar";
	u.test("bar == foo",  !(x == y));
	u.test("bar  > foo",  !(x  > y));
	u.test("bar >= foo",  !(x >= y));
	u.test("bar  < foo",   (x  < y));
	u.test("bar <= foo",   (x <= y));

	u.test("foo == bar",  !(y == x));
	u.test("foo  > bar",   (y  > x));
	u.test("foo >= bar",   (y >= x));
	u.test("foo  < bar",  !(y  < x));
	u.test("foo <= bar",  !(y <= x));

	u.report();


	// BWNumber
	printf("\nTesting BWNumber -----\n\n");
	u.init("BWNumber");

	BWNumber a = 1;
	BWNumber b = 2.0;
	BWNumber c = 3;

	u.test("constructors", (double) a == 1.0 && (int) b == 2 && (int) c == 3);


	a = b + c;
	b += 2;
	c = b + (BWNumber)3;

	u.test("+ operator", (int) a == 5);
	u.test("+= operator", (int) b == 4);
	u.test("+ operator (BWNumber) ", (int) c == 7);
	
	printf("a: %ld\n", (long) a++);
	printf("a: %ld\n", (long) ++a);
	printf("a: %ld\n", (long) a--);
	printf("a: %ld\n", (long) --a);
	printf("a: %lf\n", (double) a++);
	printf("a: %lf\n", (double) ++a);
	printf("a: %lf\n", (double) a--);
	printf("a: %lf\n", (double) --a);

	u.test("pre ++", ++a == BWNumber(6));
	u.test("post ++", a++ == BWNumber(6));
	u.test("post ++ (part 2)", a == BWNumber(7));
	
	u.test("pre --", --a == BWNumber(6));
	u.test("post --", a-- == BWNumber(6));
	u.test("post -- (part 2)", a == BWNumber(5));

	b += 3.2;
	u.test("b += 3.2", b == BWNumber(7.2));

	u.test("7.2 == 7",  !(BWNumber(7.2) == BWNumber(7)));
	u.test("7.2  > 7",  !(BWNumber(7.2)  > BWNumber(7)));
	u.test("7.2 >= 7",   (BWNumber(7.2) >= BWNumber(7)));
	u.test("7.2  < 7",  !(BWNumber(7.2)  < BWNumber(7)));
	u.test("7.2 <= 7",  !(BWNumber(7.2) <= BWNumber(7)));
	
	a.reset();
	u.test("reset", a == BWNumber(0));

	u.report();
	
	// BWWebGet
	printf("\nTesting BWWebGet -----\n\n");
	u.init("BWWebGet");

	// cpp.bw.org/code-clinic-test/test-data-2014-02-27.txt
	// ... is a copy of ...
	// lpo.dt.navy.mil/data/DM/2014/2014_02_27/Wind_Gust
	
	BWWebGet web("cpp.bw.org");
	BWString body;

	printf("host: %s, port %d, address: %s\n", web.get_host().c_str(), (int) web.get_port(), web.get_address());
	u.test("host", memcmp("cpp.bw.org", web.get_host(), 11) == 0);
	u.test("address", web.get_address() != nullptr);
	u.test("port", (int) web.get_port() == 80);
	if (web.get_errno()) {
		printf("could not connect\n");
	} else {
		printf("connected...\n");
		printf("fetching data from http://cpp.bw.org/code-clinic-test/test-data-2014-02-27.txt\n");
		body = web.fetch("/code-clinic-test/test-data-2014-02-27.txt");
		u.test("web fetch", (int) web.http_status_code() == 200);
		if(web.get_errno()) printf("error: %s\n", web.get_errstr());
		else {
			std::vector<BWString> lines = body.split("\n");
			size_t l = lines.size();
			printf("size of vector: %ld\n", lines.size());
			u.test("lines in body", (l > 10) && (l < 500));
		}
	}

	u.report();
	
	// BWCGI
	printf("\nTesting BWCGI -----\n\n");
	u.init("BWCGI");

	setenv("QUERY_STRING", "a=foo&b=bar&c=baz", 0);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 0);
	
	BWCGI cgi;

	if(cgi.have_cgi()) {
		cgi.disp_page("text/plain", "this is the body.\n");
		if(cgi.status().have_value()) printf("%s\n", cgi.status().c_str());

		for ( auto m : cgi.env() ) {
			printf("%s [%s]\n", m.first.c_str(), m.second.c_str());
		}

		printf("CGI: %s\n", cgi.getvar("GATEWAY_INTERFACE").c_str());

		for ( auto q : cgi.qmap() ) {
			printf("%s [%s]\n", q.first.c_str(), q.second.c_str());
		}
	}

	u.test("a", memcmp(cgi.getq("a").c_str(), "foo", 4) == 0);
	u.test("b", memcmp(cgi.getq("b").c_str(), "bar", 4) == 0);
	u.test("c", memcmp(cgi.getq("c").c_str(), "baz", 4) == 0);
	u.report();
	
	return 0;	// Done. Yay!
}

