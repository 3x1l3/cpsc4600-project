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
	
	for(int i=0; i < sset.size(); i++) {
		if(!Set(temp).isMember(sset.at(i)))
			temp.push_back(sset.at(i));	
	}
	
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

Set Set::remove(string str) {

	int index = indexAt(str);
	vector<string> temp;
	if (index != -1) {
		temp = set;
		temp.erase(temp.begin()+index);
		return temp;
	}
	return set;	
}

int Set::indexAt(string str) {
	for(int i=0; i < set.size(); i++) {
		if (set.at(i) == str) {
			
			return i;	
		}
	}
	return -1;	
}

bool Set::isMember(string str) {
	
	for(int i=0; i < set.size(); i++) {
		if (set.at(i) == str) {
			return true;	
		}
	}
	return false;
}