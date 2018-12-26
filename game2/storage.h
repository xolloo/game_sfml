#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <sqlite3.h>
#include <time.h>
#include <vector>


struct record_db
{
	std::string name;
	int value;
	std::string date;
};

class Storage
{
private:
	std::string name;
	sqlite3 *db;
public:
	Storage();
	Storage(std::string name, std::string database);
	record_db get_last_record();
	std::vector<record_db> get_records(int count);
	void add_record(int value);
	~Storage();
};

#endif // STORAGE_H
