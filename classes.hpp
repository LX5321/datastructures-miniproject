#include<bits/stdc++.h>
#include<chrono>

class processFile
{
private:
	// line buffer
	std::vector<std::string> linesOfFiles;
	// map for course and course_number
	std::map<std::string, std::string> courseTable;
	// roll number -> [ course_number, grade]
	std::multimap<std::string, std::pair<std::string, std::string> > coursesEnrolledIn;


	bool iterateCourseFile(std::string fileName, std::function<void (const std::string & )> callback)
	{
		std::ifstream in(fileName.c_str());
		// fail if no file
		assert(in);

		// read each line
		std::string str;
		while (std::getline(in, str)){callback(str);}

		// close stream
		in.close();
		return true;
	}
	void runRegex(){
		auto start = std::chrono::steady_clock::now();
		for (short i = 0; i < linesOfFiles.size(); ++i)
		{
			auto const str = linesOfFiles[i];
			auto const re = std::regex{R"(\s+)"};
			auto const vec = std::vector<std::string>(
				std::sregex_token_iterator{begin(str), end(str), re, -1},
				std::sregex_token_iterator{}
				);
			// std::cout<<vec[0]<<"\n";    
			// std::cout<<"  `-"<<vec[1]<<"\n";    
			// std::cout<<"  `-"<<vec[2]<<"\n";    
			// std::cout<<"  `-";

			std::string courseName = "";
			for (int i = 3; i < vec.size()-1; ++i)
			{
				// for every word add a space
				courseName+=vec[i];
				courseName+=" ";
			}
			// add ending word without spaces
			courseName+=vec[vec.size()-1];
			// push course code and course name to the map
			courseTable.insert(std::make_pair(vec[1], courseName));
			// reset string buffer
			courseName = "";

			// insert student and courses relation
			coursesEnrolledIn.insert(std::make_pair(vec[0], std::make_pair(vec[1], vec[2])));

		}
		auto end = std::chrono::steady_clock::now();
		std::cout<<"Finished courseTable Build in "<<std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()<<" ns\n";

	}

public:
	void printCourseTable(){
		// print (courses, name) table
		for(auto elem : courseTable)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		}
		std::cout<<"===========\n";
		
		// print out whole students(courses, grade) table 
		for(auto elem : coursesEnrolledIn)
		{
			std::cout << elem.first << " " << elem.second.first << " " << elem.second.second << "\n";
		}
		std::cout<<"\nFinding 2015cs2622.\n";
		// make an iterator object to search the multimap
		for (auto itr = coursesEnrolledIn.find("2015cs2622"); itr != coursesEnrolledIn.end(); ++itr) { 
			std::cout << itr->first << '\t' << itr->second.first << '\n'; 
		} 

	}
	bool parseFile(std::string filename){
		iterateCourseFile(filename, [&](const std::string & str){linesOfFiles.push_back(str);});
		runRegex();
	}
};
