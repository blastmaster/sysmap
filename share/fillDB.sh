#!/bin/bash

if [ $# -eq 0 ]
  then
    echo "No argument supplied"
    echo "Syntax: fillDB [path/to/sql/files/identifier] [path/to/db/file]"
    echo "This Script appends a '*' after the given path, dont do it by your self"
    exit 1
fi

echo "create table if not exists Hosttable ("\
   "HostID integer primary key autoincrement not null,"\
   "Hostname text UNIQUE"\
");"\
"create table if not exists Host2Data ("\
   "HostID integer,"\
   "DID integer"\
");"\
"create table if not exists Datatable ("\
   "DID integer primary key autoincrement not null,"\
   "EID integer,"\
   "Data text UNIQUE"\
");"\
"create table if not exists Extractortable ("\
   "EID integer primary key autoincrement not null,"\
   "Name text UNIQUE"\
");" | sqlite3 $2;

echo "sysmap.db Initialized"

cat $1* | sqlite3 $2;

echo "sysmap.db filled with given data"
