#include <time.h>
#include <iostream>
#include <string>


int main()
{
	time_t the_time;
	the_time = time((time_t *)0);
	struct tm *tm = localtime(&the_time);
	char date[40];
	strftime(date, 20, "%Y-%m-%d %H:%M:%S", tm);
	std::cout << "The time is: " << date << '\n';
	return 0;
}



// time_t t = time(NULL);
// struct tm *tm = localtime(&t);
// char date[20];
// strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);

// 2018-12-26 21:12:23
