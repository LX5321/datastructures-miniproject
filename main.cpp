#include "classes.hpp"
int main(int argc, char const *argv[])
{
	processFile pF;
	pF.parseFile("examples/example.txt");
	pF.printCourseTable();
	return 0;
}