#include "storage.h"
#include <time.h>
#include <vector>
#include <sstream>


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
	std::ostringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS `records` ("
		"`id`    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
		"`name`  TEXT NOT NULL,"
		"`value` INTEGER NOT NULL,"
		"`date`  INTEGER NOT NULL"
		");";
	std::string sql(ss.str());
	exit = sqlite3_open(database.c_str(), &this->db);
	if (exit)
	{
		std::ostringstream message;
		message << "Error open database " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
	rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		std::ostringstream message;
		message << "Error create table " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
}

record_db Storage::get_last_record()
{
	int rc;
	sqlite3_stmt *stmt;
	std::ostringstream ss;
	ss << "SELECT name, value, date "
		"FROM records "
		"ORDER BY \"value\" "
		"DESC LIMIT 1;";
	std::string sql = ss.str();
	rc = sqlite3_prepare(
		this->db, sql.c_str(),
		-1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::ostringstream message;
		message << "Error query select " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW)
	{
		std::ostringstream message;
		message << "Error get data " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
	record_db record;
	record.name = (char*)sqlite3_column_text(stmt, 0);
	record.value = sqlite3_column_int(stmt, 1);
	char data[20];
	time_t unix_time = (int)sqlite3_column_int(stmt, 2);
	tm *tm = localtime(&unix_time);
	strftime(data, 20, "%Y-%m-%d %H:%M:%S", tm);
	record.date = data;
	return record;
}

std::vector<record_db> Storage::get_records(int count)
{
	int rc;
	sqlite3_stmt *stmt;
	std::ostringstream ss;
	ss << "SELECT name, value, date "
		"FROM records "
		"ORDER BY \"value\" "
		"DESC LIMIT " << count << ";";
	std::string sql = ss.str();
	rc = sqlite3_prepare(
		this->db, sql.c_str(),
		-1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::ostringstream message;
		message << "Error query select " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
	std::vector<record_db> records;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		record_db record;
		record.name = (char*)sqlite3_column_text(stmt, 0);
		record.value = sqlite3_column_int(stmt, 1);
		char data[20];
		time_t unix_time = (int)sqlite3_column_int(stmt, 2);
		tm *tm = localtime(&unix_time);
		strftime(data, 20, "%Y-%m-%d %H:%M:%S", tm);
		record.date = data;
		records.push_back(record);
	}
	return records;
}

void Storage::add_record(int value)
{
	int rc;
//	sqlite3_stmt *stmt;
	std::ostringstream ss;
	time_t curent_time = time((time_t *)0);
	ss << "INSERT INTO `records`(`name`,`value`,`date`)"
		"VALUES ('" << this->name <<"'," << value << "," <<
		curent_time << ");";
	std::string sql = ss.str();
	rc = rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		std::ostringstream message;
		message << "Error query insert " << sqlite3_errmsg(this->db);
		throw StorageError(message.str());
	}
}

Storage::~Storage()
{
	sqlite3_close(this->db);
}
