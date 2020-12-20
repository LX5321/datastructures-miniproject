#include "classes.hpp"

int main(int argc, char const *argv[])
{
	studentDataBase sDB;
	queryStudentsDataBase qSDB;
	sDB.parseFile("data/courses.txt");
	sDB.buildStudentsData("data/students.txt");
	sDB.INFO("2015cs1325");
	sDB.SHARE("2015cs1325", "IT");
	sDB.SHARE("2015cs1325", "Himadri");
	sDB.SHARE("2015cs1325", "COL106");
	// qSDB.buildQueryStack("data/queries.txt");
	return 0;
}