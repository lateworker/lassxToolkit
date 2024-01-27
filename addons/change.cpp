#include <iostream>
using namespace std;
int main(int n, char **x) {
	string f = x[2];
	if(f.find('.')!=string::npos) {
		string g = f.substr(0, f.size()-3);
		g += "webp";
		string command = "move " + f + " " + g;
//		cout<<command<<"\n";
		system(command.c_str());
	}
	return 0;
}
