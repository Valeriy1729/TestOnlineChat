#ifndef DATABASE
#define DATABASE

#include <fstream>
#include <iostream>
#include <QString>
#include <string>
#include <vector>
#include "User.h"
#include "msg.h"

using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;


template<typename T>
class DataBase {
	string baseName;	
public:
	DataBase(string _baseName) :  baseName(_baseName)
	{
		ofstream ofs (_baseName.c_str(),
			std::ios::app | std::ios::binary);
		ofs.close();
	}	
	void writeObj(T* obj)
	{
		ofstream ofs(baseName.c_str(), std::ios::app | std::ios::binary);
		ofs.write(reinterpret_cast<char*>(obj), sizeof(T));
		cout << sizeof(T) << endl;
		ofs.close();	
	}
	bool findObj(T* obj)
	{
		T _obj {};
		ifstream ifs(baseName.c_str(), std::ios::in | std::ios::binary);
		while(!ifs.eof()) {
			cout << sizeof(T) << endl;
			ifs.read(reinterpret_cast<char*>(&_obj), sizeof(T));
			if(_obj == *obj) return true;
		}		
		ifs.close();
		return false;
	}
	vector<T> getAllObj()
	{
		vector<T> Res {};
		T _obj {};
		ifstream ifs(baseName.c_str(), std::ios::in | std::ios::binary);
		while(!ifs.eof()) {
			cout << sizeof(T) << endl;
			ifs.read(reinterpret_cast<char*>(&_obj), sizeof(T));
			Res.push_back(_obj);
		}		
		ifs.close();
		cout << Res.size() << endl;
		return Res;
	}
};


#endif
