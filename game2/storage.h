#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <sqlite3.h>


class Storage
{
private:
	std::string name;
	sqlite3 *db;
public:
	Storage();
	Storage(std::string name, std::string database);
	get_last_record();
	get_records(int count);
	get_curent_record();
	add_record(int value);
	~Storage();
};

#endif // STORAGE_H
