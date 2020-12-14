#include<bits/stdc++.h>
#include<chrono>

#define courseCredits 3.00

class processCoursesFile
{
private:
	// line buffer
	std::vector<std::string> linesOfFiles;
	// map for course and course_number
	std::map<std::string, std::string> courseTable;
	// roll number -> [ course_number, grade]
	std::multimap<std::string, std::pair<std::string, std::string> > coursesEnrolledIn;
	// store calculated cgpa
	std::map<std::string, int> studentsTotalCGPA;
	// stack to hold unique students
	std::stack<std::string> uniqueStudentNumbers;
	// vector to hold grades of each students course
	std::stack<std::string> studentGradeStack;
	// map to hold roll no and cgpa
	std::map<std::string, float> studentCGPAMap;


	bool iterateCourseFile(std::string fileName, std::function<void (const std::string & )> callback)
	{
		std::ifstream in(fileName.c_str()); // create stream to read file
		assert(in); // fail and dump core if no file
		std::string str; // buffer for each line
		while (std::getline(in, str)){callback(str);} // run lambda
		in.close(); // close stream
		return true;
	}

	void computeCGPA(std::stack<std::string>& studentGrades){
		short temp = 0, totalCoursesOptedFor = studentGrades.size();
		// compute grades using if / switch
		while(!studentGrades.empty())
		{
			if(studentGrades.top() == "A"){temp += (courseCredits*10);}
			else if(studentGrades.top() == "Aminus"){temp += (courseCredits*9);}
			else if(studentGrades.top() == "B"){temp += (courseCredits*8);}
			else if(studentGrades.top() == "Bmin"){temp += (courseCredits*7);}
			else if(studentGrades.top() == "C"){temp += (courseCredits*6);}
			else if(studentGrades.top() == "Cmin"){temp += (courseCredits*5);}
			else if(studentGrades.top() == "D"){temp += (courseCredits*4);}
			else if(studentGrades.top() == "I"){--totalCoursesOptedFor;}
			studentGrades.pop(); // completed all checks, pop from the stack
		}
		float calculatedCGPA = temp/(totalCoursesOptedFor*courseCredits);
		if(std::isnan(calculatedCGPA)){calculatedCGPA=0;} // catch div by negative number
		studentCGPAMap.insert(std::make_pair(uniqueStudentNumbers.top(), calculatedCGPA));
	}

	void buildCGPAMap(){
		// get unique numbers from the course map with grades
		for(auto it = coursesEnrolledIn.begin(), end = coursesEnrolledIn.end(); it != end; it = coursesEnrolledIn.upper_bound(it->first))
		{/* push to stack*/uniqueStudentNumbers.push(it->first);}

		// pop stack and search for each student
		while(!uniqueStudentNumbers.empty()){
			// create a typedef to make an iterator for the map object			
			typedef std::multimap<std::string, std::pair<std::string, std::string>>::iterator MMAPIterator;
    		// It returns a pair representing the range of elements with key equal to the top of the stack
			std::pair<MMAPIterator, MMAPIterator> result = coursesEnrolledIn.equal_range(uniqueStudentNumbers.top());
    		// Iterate over the range
			for (MMAPIterator it = result.first; it != result.second; it++)
			{studentGradeStack.push(it->second.second);}
			// send to the function that calculates the cgpa based
			// on string value of grade
			computeCGPA(studentGradeStack);
			// remove students whose grade has been calculated
			uniqueStudentNumbers.pop();
		}
	}

	void runRegex(){
		for (short i = 0; i < linesOfFiles.size(); ++i)
		{
			auto const str = linesOfFiles[i];
			auto const re = std::regex{R"(\s+)"};
			auto const vec = std::vector<std::string>(
				std::sregex_token_iterator{
				begin(str), end(str), re, -1},std::sregex_token_iterator{}
				);

			std::string courseName = "";

			for (int i = 3; i < vec.size()-1; ++i){courseName+=vec[i]+" ";}
			// add ending word without spaces
			courseName+=vec[vec.size()-1];
			// push course code and course name to the map
			courseTable.insert(std::make_pair(vec[1], courseName));
			// reset string buffer
			courseName = "";
			// insert student and courses relation
			coursesEnrolledIn.insert(std::make_pair(vec[0], std::make_pair(vec[1], vec[2])));
		}		
	}

public:
	void printCourseTable(){
		// print (courses, name) table
		// for(auto elem : courseTable)
		// {
		// 	std::cout << elem.first << " " << elem.second << "\n";
		// }
		// std::cout<<"===========\n";
		
		// // print out whole students(courses, grade) table 
		// for(auto elem : coursesEnrolledIn)
		// {
		// 	std::cout << elem.first << " " << elem.second.first << " " << elem.second.second << "\n";
		// }
		// std::cout<<"\nFinding 2015cs2622.\n";
		// // make an iterator object to search the multimap
		// for (auto itr = coursesEnrolledIn.find("2015cs2622"); itr != coursesEnrolledIn.end(); ++itr) { 
		// 	std::cout << itr->first << '\t' << itr->second.first << '\n'; 
		// }
		for(auto elem:studentCGPAMap){
			std::cout<<elem.first<<"->"<<elem.second<<"\n";
		}
		

	}
	bool parseFile(std::string filename){
		iterateCourseFile(filename, [&](const std::string & str){linesOfFiles.push_back(str);});
		runRegex();
		buildCGPAMap();
		return true;
	}
};

// By Alexander R. Rodrigues