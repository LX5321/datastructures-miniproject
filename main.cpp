#include "classes.hpp"
int main(int argc, char const *argv[])
{
	processFile pF;
	pF.parseFile("data/students.txt");
	pF.printCourseTable();
	return 0;
}