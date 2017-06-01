#pragma once
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Authorization
{
	string login, password;
public:
	Authorization();
	~Authorization();
	bool enter(string Login, string Password);
	void registration(string Login, string Password);
	int access(string Login);
};

