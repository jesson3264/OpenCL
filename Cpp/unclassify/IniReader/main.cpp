#include <iostream>
#include <assert.h>

#include "IniReader.h"
class TestC
{
private:
	
public:
	TestC() {

	}
	TestC(int i)
	{
		i_ = i;
	}
public:
	int i_;
};

bool Replace(string& src, char srcch, char dstch)
{
	for (size_t i = 0; i < src.length(); ++i)
		if (src[i] == srcch)
			src[i] = dstch;
	return true;
}

int main(int argc, char** argv)
{
	string src = "127.0.0.1@7001";
	if (Replace(src, '@', ':'))
		cout << src << endl;

	TestC* c = new TestC(100);
	cout << c->i_ << endl;
	IniReader ini_reader("./test.ini");
	string sret =  ini_reader.ParseIniFile();
	if (sret.length() != 0)
	{
		cerr << sret << endl;
		return -1;
	}
		
	string val;
	ini_reader.AddSection("newsection", " is new section");
	ini_reader.Set("newsection", "hello", "world", "comments");
	val = ini_reader.GetOptValue("newsection", "hello", "undefine");
	assert(val == "world");

	ini_reader.Set("newsection", "hello2", "world2", "comments");
	val = ini_reader.GetOptValue("newsection", "hello2", "default");
	assert(val == "world2");

	ini_reader.Set("newsection", "hello3", "world3", "comments");
	val = ini_reader.GetOptValue("newsection", "hello3", "undefine");
	assert(val == "world3");
	

	ini_reader.Set("section1", "key1", "val1-new", "comments");
	val = ini_reader.GetOptValue("section1", "key1", "undefine");
	assert(val == "val1-new");

	ini_reader.Set("section1", "key2", "val2-new", "comments");

	ini_reader.WriteToFile("./test2.ini");

	ini_reader.DoShow();
	bool ret = ini_reader.WriteToFile("./test2.ini");
	if (!ret)
	{
		cout << "write failured." << endl;
	}
	cout << "perfect" << endl;
	return 0;
}

