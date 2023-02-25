#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class Encode {
	public:
		Encode(): utf8(""), ansi("") {}
		~Encode() {}
		string utf8, ansi;
		void utf8_refresh() {
			const char *src_str = ansi.c_str();
			int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
			char* str = new char[len + 1];
			memset(str, 0, len + 1);
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
			string strTemp = str;
			if (wstr) delete[] wstr;
			if (str) delete[] str;
			utf8 = strTemp;
		}
		void ansi_refresh() {
			const char *src_str = utf8.c_str();
			int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
			wchar_t* wszGBK = new wchar_t[len + 1];
			memset(wszGBK, 0, len * 2 + 2);
			MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
			len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
			char* szGBK = new char[len + 1];
			memset(szGBK, 0, len + 1);
			WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
			string strTemp(szGBK);
			if (wszGBK) delete[] wszGBK;
			if (szGBK) delete[] szGBK;
			ansi = strTemp;
		}
};
class Record {
	public:
		string filename;
		size_t data_length;
		vector<string> data;

		Record() : data_length(0) {}
		Record(string filename, size_t data_length = 0) : filename(filename), data_length(data_length) {
			getRecord_utf8();
		}
		~Record() {}

		void setRecordPath(string _filename, size_t _data_length = 0) {
			filename = _filename;
			data_length = _data_length;
		}
		bool getRecord_utf8() {
			ifstream fin(filename.c_str());
			if (!fin.is_open()) {
				return true;//可以加提示
			}
			Encode tmp;
			while (getline(fin, tmp.utf8)) tmp.ansi_refresh(), data.push_back(tmp.ansi);
			fin.close();
			if (data.size() < data_length) {
				return true;//可以加提示
			}
			return false;
		}
		bool putRecord_utf8() {
			ofstream fout(filename.c_str());
			if (!fout.is_open()) {
				return true;//可以加提示
			}
			for (size_t i = 0; i < data.size(); i++) {
				Encode tmp;
				tmp.ansi = data[i];
				tmp.utf8_refresh();
				fout << tmp.utf8 << endl;
			}
			fout.close();
			return false;
		}
		bool addRecord_utf8() {
			ofstream fout(filename.c_str(), ios::out | ios::app);
			if (!fout.is_open()) {
				return true;//可以加提示
			}
			for (size_t i = 0; i < data.size(); i++) {
				Encode tmp;
				tmp.ansi = data[i];
				tmp.utf8_refresh();
				fout << tmp.utf8 << endl;
			}
			fout.close();
			return false;
		}
};
