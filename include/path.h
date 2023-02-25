#include <iostream>
#include <vector>
#include <io.h>
using namespace std;
class File {
	public:
		File() : attrib(0), path(""), name(""), size(0) {}
		File(unsigned attrib, string path, string name, _fsize_t size) : attrib(attrib), path(path), name(name), size(size) {}
		~File() {}

		void testout() {
			cout << attrib << " " << path << " " << name << " " << size << endl;
		}

		unsigned attrib;
		string path;
		string name;
		_fsize_t size;
};
class Dir {
	public:
		Dir() : attrib(0), path("") {}
		Dir(string path) : path(path) {}
		Dir(unsigned attrib, string path) : attrib(attrib), path(path) {}
		~Dir() {}

		void testout() {
			cout << attrib << " " << path << endl;
		}

		bool check() {
			return system(("cd \"" + path + "\"").c_str());
		}
		bool create() {
			return system(("mkdir \"" + path + "\"").c_str());
		}
		bool operator== (const Dir &o) const {
			if ((attrib == o.attrib) and (path == o.path)) return true;
			return false;
		}
		void operator= (const string &o) {
			attrib = _A_SUBDIR;
			path = o;
		}
		operator string() {
			return path;
		}

		unsigned attrib;
		string path;
};
class Path {
	public:
		Dir path;
		string selector;
		vector<Dir> excepts;
		void (*fileFunction)(File &data);
		void (*dirFunction)(Dir &data);

		Path() : selector(""), fileFunction(nullptr), dirFunction(nullptr) {}
		Path(string path, string selector) : path(Dir(_A_SUBDIR, path)), selector(selector), fileFunction(nullptr), dirFunction(nullptr) {}
		Path(Dir path, string selector) : path(path), selector(selector), fileFunction(nullptr), dirFunction(nullptr) {}
		~Path() {}

		void setAll(string _path = "", string _selector = "", void (*_fileFunction)(File &data) = nullptr, void (*_dirFunction)(Dir &data) = nullptr) {
			fileFunction = _fileFunction;
			dirFunction = _dirFunction;
			path = _path;
			selector = _selector;
		}
		bool get() {
			return _get(path, selector);
		}
	protected:
		bool _get(Dir _path, string _choose) {
			for (size_t i = 0; i < excepts.size(); i++) {
				if (_path == excepts[i]) return true;
			}
			bool _findFiles_file = _get_file(_path, _choose);
			bool _findFiles_dir = _get_dir(_path, _choose);
			return _findFiles_file or _findFiles_dir;
		}
		bool _get_file(Dir _path, string _choose) {
			_finddata_t file_info;
			string current_path = (string)_path + _choose;
			intptr_t handle = _findfirst(current_path.c_str(), &file_info);
			if (handle == -1) {
				return false;
			}
			do {
				if (file_info.attrib == _A_SUBDIR) continue;
				File newfile(file_info.attrib, (string)_path, file_info.name, file_info.size);
				if (fileFunction != nullptr) (*fileFunction)(newfile);
//				newfile.testout();
			} while (!_findnext(handle, &file_info));
			return true;
		}
		bool _get_dir(Dir _path, string _choose) {
			_finddata_t file_info;
			string current_path = (string)_path + "\\*.*";
			intptr_t handle = _findfirst(current_path.c_str(), &file_info);
			if (handle == -1) {
				return false;
			}
			bool return_value = false;
			do {
				if (file_info.attrib != _A_SUBDIR or !strcmp(file_info.name, "..") or !strcmp(file_info.name, "."))
					continue;
				Dir newdir(file_info.attrib, (string)_path + "\\" + file_info.name);
				if (dirFunction != nullptr) (*dirFunction)(newdir);
//				newdir.testout();
				if (_get(newdir, _choose)) {
					return_value = true;
				}
			} while (!_findnext(handle, &file_info));
			return return_value;
		}
};

void filetestout(File &data) {
	cout << data.attrib << " " << data.path << " " << data.name << " " << data.size << endl;
}
void dirtestout(Dir &data) {
	cout << data.attrib << " " << data.path << endl;
}
