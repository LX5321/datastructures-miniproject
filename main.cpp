#include "classes.hpp"
int main(int argc, char const *argv[])
{
	processCoursesFile pF;
	pF.parseFile("data/courses.txt");
	pF.printCourseTable();
	return 0;
}