#ifndef USER_H
#define USER_H

#define MAX_LEN		128


struct User {

	char name[MAX_LEN] {""};
	long long passHash;	

	bool operator==(const User& other)
	{
		bool cond1 = strcmp(name, other.name) == 0;
	       	bool cond2 = passHash == other.passHash;	
		return cond1 && cond2;
	}

};


#endif
