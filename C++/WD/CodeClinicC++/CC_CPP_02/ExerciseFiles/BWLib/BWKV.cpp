//  BWKV.cpp
//  A simple key-value system using BWDB
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWKV.h"

#pragma mark - class BWKV

BWKV::BWKV( const char * fn ) : BWDB(fn) {
	if (!table_exists("kv")) {
		do_query(_bwkv_drop_sql);
		do_query(_bwkv_create_table_sql);
		do_query(_bwkv_create_trigger_sql);
		do_query(_bwkv_insert_sql);
	}
}

const char * BWKV::value ( const char * key ) {
	get_query_value("SELECT value FROM kv WHERE key = ?", std::vector<BWString>({BWString(key)}));
	if(have_error()) return nullptr;
	else return _get_value();
}

bool BWKV::value( const char * k, const char * v ) {
	BWString q;
	if (value(k)) {
		q = "UPDATE kv SET value = ? WHERE key = ?";
	} else {
		q = "INSERT INTO kv (value, key) VALUES (?, ?)";
	}
	std::vector<BWString> p({BWString(v), BWString(k)});
	return do_query(q, p);
}

const BWString BWKV::operator [] ( const BWString & key ) {
	return BWString(value( key.c_str() ));
}

const char * BWKV::operator [] ( const char * key ) {
	return value( key );
}

void BWKV::del( const char * key ) {
	if (value(key)) {
		BWString q = "DELETE FROM kv WHERE key = ?";
		std::vector<BWString> p({key});
		do_query(q, p);
	}
}

void BWKV::del( const BWString & key ) {
		del(key.c_str());
}
