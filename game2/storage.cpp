#include "storage.h"
#include <time.h>
#include <vector>

class StorageError : public std::exception
{
private:
	std::string msg;

public:
	StorageError(std::string msg):
		msg(msg)
	{}

	const char* what() const throw()
	{
		return this->msg.c_str();
	}
};

Storage::Storage(){}

Storage::Storage(std::string name, std::string database) :
	name(name)
{
	int exit = 0;
	int rc;
	std::string sql =
		"CREATE TABLE IF NOT EXISTS `records` ("
		"`id`    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
		"`name`  TEXT NOT NULL,"
		"`value` INTEGER NOT NULL,"
		"`date`  INTEGER NOT NULL";
	exit = sqlite3_open(database.c_str(), &this->db);
	if (exit)
	{
		throw StorageError(
			"Error open database " +
			sqlite3_errmsg(this->db));
	}
	rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		throw StorageError(
			"Error create table " +
			sqlite3_errmsg(this->db));
	}
}

record_db Storage::get_last_record()
{
	int rc;
	sqlite3_stmt *stmt;
	std::string sql =
		"SELECT name, value, date "
		"FROM records "
		"WHERE name=\"" + this->name + "\" "
		"ORDER BY \"id\" "
		"DESK LIMIT 1;";
	rc = sqlite3_prepare(
		this->db, sql.c_str(),
		sql.size(), &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		throw StorageError(
			"Error query select " +
			sqlite3_errmsg(this->db))
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW)
	{
		throw StorageError(
			"Error get data " +
			sqlite3_errmsg(this->db));
	}
	record_db record;
	record.name(sqlite3_column_text(stmt, 1));
	record.value = sqlite3_column_int(stmt, 2);
	char data[20];
	struct tm *tm = localtime(sqlite3_column_int(stmt, 3));
	strftime(data, 20, "%Y-%m-%d %H:%M:%S", tm);
	record.date(data);
	return record;
}

std::vector<record_db> Storage::get_records(int count)
{
	int rc;
	sqlite3_stmt *stmt;
	std::string sql =
		"SELECT name, value, date "
		"FROM records "
		"WHERE name=\"" + this->name + "\" "
		"ORDER BY \"id\" "
		"DESK LIMIT " + static_cast<std::string>(count) + ";";
	rc = sqlite3_prepare(
		this->db, sql.c_str(),
		sql.size(), &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		throw SotrageError(
			"Error query select " +
			sqlite3_errmsg(this->db))
	}
	std::vector<record_db> records;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		record_db record;
		record.name = sqlite3_column_text(stmt, 1);
		record.value = sqlite3_column_int(stmt, 2);
		char data[20];
		struct tm *tm = localtime(&sqlite3_column_int(stmt, 3));
		strftime(data, 20, "%Y-%m-%d %H:%M:%S", tm);
		record.date(data);
		records.push_back(record)
	}
	return records;
}

Storage::~Storage()
{
	sqlite3_close(this->db);
}
