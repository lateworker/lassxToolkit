#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
inline std::string strquote(std::string x) {
	return "\"" + x + "\"";
}
inline std::string strdelete(std::string data) {
	while (*data.begin() == '\"') data.erase(data.begin());
	while (*(data.end() - 1) == '\"') data.erase(data.end() - 1);
	while (*(data.end() - 1) == '\\' || *(data.end() - 1) == '/') data.erase(data.end() - 1);
	return data;
}
inline void ansi_to_utf8(std::string &data) {
	const char *src_str = data.c_str();
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	data = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
}
inline void utf8_to_ansi(std::string &data) {
	const char *src_str = data.c_str();
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	data = szGBK;
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
}
inline bool getData_utf8File(std::string path, std::vector<std::string> &data) {
	data.clear();
	std::ifstream file(path.c_str());
	if (!file.is_open()) return true;
	std::string tmp = "";
	while (getline(file, tmp)) {
		utf8_to_ansi(tmp);
		data.push_back(tmp);
	}
	file.close();
	return false;
}
inline bool putData_utf8File(std::string path, std::vector<std::string> data) {
	std::ofstream file(path.c_str());
	if (!file.is_open()) return true;
	for (size_t i = 0; i < data.size(); i++) {
		ansi_to_utf8(data[i]);
		file << data[i] << "\n";
	}
	file.close();
	return false;
}
inline bool addData_utf8File(std::string path, std::vector<std::string> data) {
	std::ofstream file(path.c_str(), std::ios::out | std::ios::app);
	if (!file.is_open()) return true;
	for (size_t i = 0; i < data.size(); i++) {
		ansi_to_utf8(data[i]);
		file << data[i] << "\n";
	}
	file.close();
	return false;
}
