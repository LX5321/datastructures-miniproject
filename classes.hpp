#include<bits/stdc++.h>

#define courseCredits 3.00

class studentDataBase
{
private:
	/*
		buffer vector to share data between functions (ignore for JAVA code)
	*/
	std::vector<std::string> linesOfFiles;
	/*
		Map for course and course number.
		Since course number can come only once and will have
		a single key, duplicates will be ignored.
	*/
	std::map<std::string, std::string> courseTable;
	/*
		roll number -> [ course_number, grade]
		Multimap because roll number will occur more than one time for
		each of the courses enrolled in.
	*/
	std::multimap<std::string, std::pair<std::string, std::string> > coursesEnrolledIn;
	/*	
	temporary stack to hold unique students
	Stack contains non repetive student numbers
	*/	
	std::stack<std::string> uniqueStudentNumbers;
	/*	
	temporary stack to hold grades of each students course.
	Unique student roll number will be obtained from the uniqueStudentNumbers stack
	*/	
	std::stack<std::string> studentGradeStack;
	/*
	map to hold roll number and cgpa of student post calculation
	*/
	std::map<std::string, float> studentCGPAMap;
	/* 
	store student roll number along with the subject codes
	student and stream multimap [student_roll, subj_chosen]
	*/
	std::multimap<std::string, std::string> studentAndSubjectMap;
	/*
	store student roll number along with the department(CSE, IT) & hostel(Sutlej)
	student_roll[dept, hostel] 
	*/
	std::map<std::string, std::pair<std::string, std::string> > RollStreamHostelMap;
	/*
	Store Roll Number and Name of Student.
	*/
	std::map<std::string, std::string> rollAndNameMap;
	/*
		store 2 sets to make search of dept and hostel faster
		otherwise we will have to loop through all elements of RollStreamHostelMap
	*/
	std::set<std::string> deptList; // cse, it, tech
	std::set<std::string> hostelList; // sutlej,

	/*=======unused===========ds==================================*/
	// student and hostel multimap [student_roll, hostel]
	// std::multimap<std::string, std::string> studentAndHostelMap;
	// store calculated cgpa
	// std::map<std::string, int> studentsTotalCGPA;
	// // roll, stream map
	// std::map<std::string, std::string> rollAndStreamMap;
	// // roll, hostel map
	// std::map<std::string, std::string> rollAndHostelMap;


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
		{
			/* push to stack */
			uniqueStudentNumbers.push(it->first);
		}

			// pop stack and search for each student
			while(!uniqueStudentNumbers.empty()){
				// create a typedef to make an iterator for the map object			
				typedef std::multimap<std::string, std::pair<std::string, std::string>>::iterator MMAPIterator;
    			// It returns a pair representing the range of elements with key equal to the top of the stack
				std::pair<MMAPIterator, MMAPIterator> result = coursesEnrolledIn.equal_range(uniqueStudentNumbers.top());
    			// Iterate over the range
				for (MMAPIterator it = result.first; it != result.second; it++)
				{
					studentGradeStack.push(it->second.second);
				}
				// send to the function that calculates the cgpa based
				// on string value of grade
				computeCGPA(studentGradeStack);
				// remove students whose grade has been calculated
				uniqueStudentNumbers.pop();
			}
		}

		void readStudentsFile(std::string studentFileDestination){
			std::ifstream infile(studentFileDestination);
			std::string line;
			while (std::getline(infile, line)) {
				std::stringstream iss(line);
				std::string a, b, c, d;
				if (!(iss >> a >> b >> c >> d)) break;
				// push a, b, c, d buffers to maps
				rollAndNameMap.insert(std::make_pair(a, b));
				RollStreamHostelMap.insert(std::make_pair(a, std::make_pair(c, d)));
				deptList.insert(c);
				hostelList.insert(d);
			}
		}

		void runRegex(){
			for (short i = 0; i < linesOfFiles.size(); ++i)
			{
				auto const str = linesOfFiles[i];
				auto const re = std::regex{R"(\s+)"};
				auto const vec = std::vector<std::string>(std::sregex_token_iterator{begin(str), end(str), re, -1},std::sregex_token_iterator{});

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
		void buildStudentsData(std::string studentFileDestination){
			readStudentsFile(studentFileDestination);
		}

		bool parseFile(std::string filename){
			iterateCourseFile(filename, [&](const std::string & str){linesOfFiles.push_back(str);});
			runRegex();
			buildCGPAMap();
			return true;
		}

		void INFO(std::string queryParameter){
			std::cout<<std::endl;
			/*
			TODO:
			Check if name or roll number is given as parameter
			*/

			/*
			Entry number, Name, Department, Hostel, CGPA, and the course-numbers of all courses 
			taken with the obtained grades in each, sorted lexicographically by the course number
			*/

			// find roll no, name 
			std::map<std::string, std::string>::iterator RollNumberAndName = rollAndNameMap.find(queryParameter);
			std::cout<<RollNumberAndName->first<<" "<<RollNumberAndName->second<<" ";
			
			// find stream, hostel
			// make an iterator object to search the map
			auto itr = RollStreamHostelMap.find(queryParameter);
			std::cout<<itr->second.first <<" " << itr->second.second; 

			// find cgpa
			auto CGPAitr = studentCGPAMap.find(queryParameter);
			std::cout<<std::setprecision(3)<<" "<<CGPAitr->second<<" ";

			// FIND COURSES ENROLLED IN
			// make an iterator object to search the multimap sort by coursenumber
			typedef std::multimap<std::string, std::pair<std::string, std::string> >::iterator MMAPIterator;
    		// It returns a pair representing the range of elements with key equal to the query parameter
			std::pair<MMAPIterator, MMAPIterator> result = coursesEnrolledIn.equal_range(queryParameter);
    		// create a temporary map to sort the key(course name), value(grade)
    		std::map<std::string, std::string> tempMap;
    		// Iterate over the range
			for (MMAPIterator it = result.first; it != result.second; it++)
			{
				// push to the map so that course numbers are sorted
				tempMap.insert(std::make_pair(it->second.first, it->second.second));
			}
			// iterate again and print in sorted order
			for(auto sortedOrder:tempMap){
				std::cout<<sortedOrder.first<<" "<<sortedOrder.second<<" ";
			}
			// clear map so no conflicts
			tempMap.clear();

		}

		void SHARE(std::string studentRollNumber, std::string queryParameter){
			std::cout<<std::endl<<std::endl;
			// first find the details of the student
			// ie dept and hostel
			// if hostel given, find dept and get all students from hostel and dept
			// if course given, find hostel and get all students from course and hostel
			// if course id given, find all people from the hostel of student
			// 		get the roll numbers of all in the hostel with that roll number
			//		query the rollnumber, courses table to get the people enrolled in the course
			auto studentObj 	   = RollStreamHostelMap.find(studentRollNumber);
			std::string department = studentObj->second.first;
			std::string hostel     = studentObj->second.second;

			if(deptList.find(queryParameter)!=deptList.end()){
				// it is a dept (cse, it)
				// find all the people from the same location and same dept as the roll no
				auto itr = RollStreamHostelMap.begin();
				while(itr != RollStreamHostelMap.end()){
					if(itr->second.first == department && itr->second.second == hostel){
						std::cout<<itr->first<<" "; 
					}
					++itr;
				}
			}
			else if(hostelList.find(queryParameter)!=hostelList.end()){
				// it is a hostel
				// find all the people from the same hostel across any dept
				auto itr = RollStreamHostelMap.begin();
				while(itr != RollStreamHostelMap.end()){
					if(itr->second.second == hostel){
						std::cout<<itr->first<<" "; 
					}
					++itr;
				}
			}
			else if(courseTable.find(queryParameter)!=courseTable.end()){
				// it is a course number
				// check if the student is enrolled in the course
				// find all people from the same hostel as the roll number who are enrolled in the
				// course

				// get all people who are in the hostel
				std::set<std::string> hostelBuffer;
				auto itr = RollStreamHostelMap.begin();
				while(itr != RollStreamHostelMap.end()){
					if(itr->second.second == hostel){
						hostelBuffer.insert(itr->first);
					}
					++itr;
				}
				// get all people who are in course
				std::set<std::string> courseBuffer;
				auto itr2 = coursesEnrolledIn.begin();
				while(itr2 != coursesEnrolledIn.end()){
					if(itr2->second.first == queryParameter){
						courseBuffer.insert(itr2->first);
					}
					++itr2;
				}
				// get intersection of both sets
				std::vector<std::string> intersectionBuffer; 
				std::set_intersection(hostelBuffer.begin(),hostelBuffer.end(),courseBuffer.begin(),courseBuffer.end(),
                          back_inserter(intersectionBuffer));
				
				if(intersectionBuffer.size()==0){
					std::cout<<"No Intersection";
				}
				else{
					for(auto el:intersectionBuffer){
						std::cout<<el<<" ";
					}
				}
				// clear up
				intersectionBuffer.clear();
				courseBuffer.clear();
				hostelBuffer.clear();
			}
			else{
				std::cout<<"Error";
			}
		}

		void COURSETITLE(std::string courseNumber){
			// find in course table
			std::cout<<"";
		}
	};


	class queryStudentsDataBase
	{
	private:
		std::vector<std::string> linesOfFiles;

	bool iterateCourseFile(std::string fileName, std::function<void (const std::string & )> callback)
	{
		std::ifstream in(fileName.c_str()); // create stream to read file
		assert(in); // fail and dump core if no file
		std::string str; // buffer for each line
		while (std::getline(in, str)){callback(str);} // run lambda
		in.close(); // close stream
		return true;
	}

	void parseQueries(std::string queryFileDestination){
		std::vector<std::string> vecOfStrs;
		std::stack<std::string> parameter1, parameter2, parameter3;
		std::ifstream in(queryFileDestination.c_str());
		assert(in);
		std::string str;

		while (std::getline(in, str))
		{
        	// Line contains string of length > 0 then save it in vector
			if(str.size() > 0)
				vecOfStrs.push_back(str);
		}
    	//Close The File
		in.close();
		for (short i = 0; i < vecOfStrs.size(); ++i)
		{
			auto const str = linesOfFiles[i];
			auto const re = std::regex{R"(\s+)"};
			auto const vec = std::vector<std::string>(std::sregex_token_iterator{begin(str), end(str), re, -1},std::sregex_token_iterator{});

			if (vec[0] == "SHARE")
			{
				parameter1.push(vec[0]);
				parameter2.push(vec[1]);
				parameter3.push(vec[2]);
			}
			else if(vec[0] == "COURSETITLE"){
				parameter1.push(vec[0]);
				parameter2.push(vec[1]);

			}
			else if(vec[0] == "INFO"){
				parameter1.push(vec[0]);
				parameter2.push(vec[1]);
			}

		}
		while(parameter1.size()!=0){
			std::cout<<parameter1.size()<<" "<<parameter1.top()<<"\n";
			// parameter1.pop();
			// call all queries
			
		}

	}

public:
	void buildQueryStack(std::string queryFileDestination){
		// erase any prior data in linesOfFiles vect
		// linesOfFiles.clear();
		iterateCourseFile(queryFileDestination, [&](const std::string & str){linesOfFiles.push_back(str);});
		assert(linesOfFiles.size() > 0);
		parseQueries(queryFileDestination);
	}

};
