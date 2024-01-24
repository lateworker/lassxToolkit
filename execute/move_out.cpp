#include <iostream>
#include <fstream>
#include "../include/configor/json.hpp"
using namespace std;
using namespace configor;
inline bool loadConfig(string path, json::value &data) {
	data.clear();
	ifstream file(path.c_str());
	if (!file.is_open()) return true;
	try {
		file >> json::wrap(data);
	} catch (exception &ERROR) {
		return true;
	}
	file.close();
	return false;
}
inline string strquote(string x) { return "\"" + x + "\""; }
inline void strdelete(string &data) {
	while (data.front() == '\"') data.erase(data.begin());
	while (data.back() == '\"') data.erase(data.end() - 1);
	while (data.front() == '\\' || data.front() == '/') data.erase(data.begin());
	while (data.back() == '\\' || data.back() == '/') data.erase(data.end() - 1);
	for (size_t i = 1; i < data.size(); ++i)
		if ((data[i] == '\\' && data[i - 1] == data[i]) || (data[i] == '/' && data[i - 1] == data[i]))
			data.erase(data.begin() + i), --i;
}
struct Data {
	unsigned long size;
	std::string path, name;
	inline std::string fullpath() const {
		return path + "\\" + name;
	}
	CONFIGOR_BIND(
	    json::value, Data,
	    REQUIRED(size, "size"),
	    REQUIRED(path, "path"),
	    REQUIRED(name, "name")
	);
};
inline void move(string f, string t) {
	system(("move " + strquote(f) + " " + strquote(t) + " >nul 2>&1").c_str());
}
int main(int n, char **configPath) {
	for (int i = 1; i < n; ++i) {
		json::value data, files;
		loadConfig(configPath[i], data);
		string tr_file = data["traverse_file"];
		loadConfig(tr_file, files);
		for (json::value x : files) {
			Data f = x;
			move(f.fullpath(), ".");
		}
	}
	return 0;
}
