#include <iostream>
#include <sqlite3.h>

using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

int main()
{
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open("example.db", &DB);
	sqlite3_stmt *stmt;

	string sql("SELECT * FROM PERSON;");
	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl;

	// int rc = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	sqlite3_prepare(DB, sql.c_str(), sizeof sql, &stmt, NULL);
	while (sqlite3_step (stmt) == SQLITE_ROW)
	{
		printf("%s ", sqlite3_column_text(stmt, 0));
		printf("%s ", sqlite3_column_text(stmt, 1));
		printf("%s ", sqlite3_column_text(stmt, 2));
		printf("%s\n", sqlite3_column_text(stmt, 3));
	}
	// if (rc != SQLITE_OK)
	// 	cerr << "Error SELECT" << endl;
	// else {
	// 	cout << "Operation OK!" << endl;
	// }

	sqlite3_close(DB);
	return (0);
}
