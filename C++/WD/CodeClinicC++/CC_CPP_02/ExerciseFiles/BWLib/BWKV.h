//  BWKV.cpp
//  A simple key-value system using BWDB
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLIB__BWKV__
#define __BWLIB__BWKV__

#include "BWDB.h"

#define __BWKV_VERSION "1.1.1"

static const char * _bwkv_drop_sql = "DROP TRIGGER IF EXISTS stampUpdateKv; DROP TABLE IF EXISTS kv;";
static const char * _bwkv_create_table_sql = "CREATE TABLE kv (id INTEGER PRIMARY KEY, stamp TEXT DEFAULT CURRENT_TIMESTAMP, key TEXT UNIQUE NOT NULL, value TEXT);";
static const char * _bwkv_create_trigger_sql = "CREATE TRIGGER stampUpdateKv BEFORE UPDATE ON kv BEGIN UPDATE kv SET stamp = DATETIME('now') WHERE kv.id = NEW.id; END;";
static const char * _bwkv_insert_sql = "INSERT INTO kv ( key, value ) VALUES ( 'db_version', '1.0.3' );";

class BWKV : public BWDB {

public:
	static const char * version() { return __BWKV_VERSION; }
	BWKV ( const char * fn );
	const char * value( const char * key );
	bool value( const char * key, const char * value );
	const BWString operator [] ( const BWString & key );
	const char * operator [] ( const char * key );
	void del( const char * key );
	void del( const BWString & key );
};

#endif /* defined(__BWLIB__BWKV__) */
