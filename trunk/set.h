#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>


using std::vector;
using std::string;
using std::cout;
using std::endl;

class Set {

	private:
		vector<string> set;
	
	public:
		Set();
		Set(string);
		Set(vector<string>);
		void add(string);
		vector<string> returnVec();
		Set munion(Set);
		string toString();
		bool isMember(string);
		Set remove(string);
		int indexAt(string);
	
		
	
};

#endif