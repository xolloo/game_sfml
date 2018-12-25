#include <time.h>
#include <iostream>


int main()
{
	time_t the_time;
	the_time = time((time_t *)0);
	std::cout << "The time is: " << the_time << '\n';
	return 0;
}
