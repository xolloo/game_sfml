#include "storage.h"3

class SqliteError : public std::exception
{
private:
    std::string msg;

public:
    SqliteError(std::string msg):
        msg(msg)

    const char *what() const throw()
    {
        return this.msg;
    }
}


Storage::Storage(){}

Storage::Storage(std::string name, std::string database) :
    name(name)
{
    int exit = 0;
    std::string sql =
        "CREATE TABLE IF NOT EXISTS `records` ("
        "`id`    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "`name`  TEXT NOT NULL,"
        "`value` INTEGER NOT NULL,"
        "`date`  INTEGER NOT NULL";
    exit = sqlite3_open(database, &this->db);
    if (exit)
    {
        throw SqliteError("Error open database ""sqlite3_errmsg(this->db))
    }
    else
    {
        int rc = sqlite3_exec()
    }
}

Storage::~Storage()
{
    sqlite3_close(this->db);
}
