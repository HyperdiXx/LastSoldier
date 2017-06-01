#include "Authorization.h"

Authorization::Authorization()
{
}

Authorization::~Authorization()
{
}

bool Authorization::enter(string Login, string Password)
{
	login = Login;
	password = Password;
	ifstream fin("autorization.txt");
	string log, pass, code;
	bool flag = false;
	while (fin) {
		fin >> log >> pass >> code;
		if (log == login && pass == password)
			flag = true;
	}
	fin.close();
	return flag;
}

void Authorization::registration(string Login, string Password)
{
	ofstream fout("autorization.txt", ios_base::app);
	login = Login;
	password = Password;
	fout << ' ' << login << ' ' << password << ' ' << '1';
	fout.close();
}

int Authorization::access(string Login)
{
	login = Login;
	ifstream fin("autorization.txt");
	string log, pass, code;
	while (fin) {
		fin >> log >> pass >> code;
		if (log == login)
			return atoi(code.c_str());
	}
	fin.close();
}
