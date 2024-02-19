#include <iostream>

#ifndef PATH_BASIC
#define PATH_BASIC
inline std::string pathQuote(std::string x) { return "\"" + x + "\""; }
inline void pathDelete(std::string &data) {
	while (data.front() == '\"') data.erase(data.begin());
	while (data.back() == '\"') data.erase(data.end() - 1);
	while (data.front() == '\\' || data.front() == '/') data.erase(data.begin());
	while (data.back() == '\\' || data.back() == '/') data.erase(data.end() - 1);
	for (size_t i = 1; i < data.size(); ++i)
		if ((data[i] == '\\' && data[i - 1] == data[i]) || (data[i] == '/' && data[i - 1] == data[i]))
			data.erase(data.begin() + i), --i;
}
inline void pathSplit(std::string x, std::string &path, std::string &name) {
	name = path = "";
	size_t pos = x.find_last_of("\\");
	if (pos == std::string::npos)
		path = ".", name = x;
	else {
		name = x.substr(pos);
		pathDelete(name);
		path = x.substr(0, pos);
		pathDelete(path);
	}
}
inline std::string pathMerge(const std::string &path, const std::string &name) { return path + "\\" + name; }
#endif

#ifndef SYSTEM_H
#define SYSTEM_H
inline void makedir(std::string path) {
	system(("md " + pathQuote(path) + " >nul 2>&1").c_str());
}
inline void removedir(std::string path) {
	system(("rd " + pathQuote(path) + " >nul 2>&1").c_str());
}
inline void copyfile(std::string file_path, std::string direction_path) {
	bool ret = system(("xcopy /q/g/k/r/h/y/-i " + pathQuote(file_path) + " " + pathQuote(direction_path) + " >nul 2>&1").c_str());
	if (ret) {
		std::string _path, _name;
		pathSplit(direction_path, _path, _name);
		makedir(_path);
		system(("copy /d/y " + pathQuote(file_path) + " " + pathQuote(direction_path) + " >nul 2>&1").c_str());
	}
}
inline void movefile(std::string file_path, std::string direction_path) {
	system(("move /y " + pathQuote(file_path) + " " + pathQuote(direction_path) + " >nul 2>&1").c_str());
}
inline void executefile(std::string file) { system(file.c_str()); }
inline void deletefile(std::string file) {
	system(("del /f/s/q " + pathQuote(file) + " >nul 2>&1").c_str());
}
#endif
