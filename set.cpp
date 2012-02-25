#include "set.h"


Set::Set() {
	
	
}

Set::Set(string str) {
	set.push_back(str);	
}

Set::Set(vector<string> vec) {
	set = vec;	
}

void Set::add(string str) {
	set.push_back(str);
}

vector<string> Set::returnVec() {
	return set;	
}


Set Set::munion(Set second_set) {	
	vector<string> sset = second_set.returnVec();
	vector<string> temp = set;
	temp.insert(temp.end(), sset.begin(), sset.end());	
	return Set(temp);
}

string Set::toString() {
	string str = "{";
	for(int i=0; i < set.size(); i++) {
		str += "'";
		str += set.at(i);
		str += "', ";
	}
	str += "}";
	
	return str;
	
}

bool Set::isMember(string str) {
	
	for(int i=0; i < set.size(); i++) {
		if (set.at(i) == str) {
			return true;	
		}
	}
	return false;
}