#include <iostream>
#include <regex>
#include <fstream>
#include <stdlib.h>
#include "include/configor/json.hpp"
#include "./include/path.h"

using namespace std;
using namespace path;
using namespace configor;

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
inline void strsplit(string x, string &path, string &name) {
	name = path = "";
	size_t pos = x.find_last_of("\\");
	if (pos == string::npos)
		path = ".", name = x;
	else {
		name = x.substr(pos);
		strdelete(name);
		path = x.substr(0, pos);
		strdelete(path);
	}
}

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
struct RegexRule {
	struct _rule {
		string _name, _path, _name_mode, _path_mode;
		_rule() { _name = _path = _name_mode = _path_mode = ""; }
		CONFIGOR_BIND(
		    json::value, _rule,
		    OPTIONAL(_name, "name"),
		    OPTIONAL(_path, "path"),
		    OPTIONAL(_name_mode, "mode_name"),
		    OPTIONAL(_path_mode, "mode_path")
		);
	};
	vector<_rule> rule;
	inline void load(const json::value &data) {
		try {
			for (json::value x : data)
				rule.push_back(x);
		} catch (exception &ERROR) {
			throw 0;
		}
	}
	void operator= (const json::value &data) { load(data); }
	RegexRule() { rule.clear(); }
};
struct Execute {
	vector<string> execute;
	inline void load(const json::value &data) {
		try {
			for (json::value x : data)
				execute.push_back(x);
		} catch (exception &ERROR) {
			throw 0;
		}
	}
	void operator= (const json::value &data) { load(data); }
	Execute() {execute.clear();}
};
struct Config {
	string input_path, output_path, temp_path, traverse_file, selector;
	string model, scale, denoise, syncgap;
	size_t pos_slash, pos_dot, is_file;
	RegexRule match;
	Execute addon;
	bool tree_restore, subdir_find, emptydir_find;
	inline bool init(const json::value &data) {
		try {
			input_path		= data["input_path"]; 					strdelete(input_path);
			output_path		= data["output_path"]; 					strdelete(output_path);
			temp_path		= data["temp_path"]; 					strdelete(temp_path);
			temp_path		= output_path + "\\" + temp_path;		strdelete(temp_path);
			traverse_file	= data["traverse_file"]; 				strdelete(traverse_file);
			traverse_file	= output_path + "\\" + traverse_file; 	strdelete(traverse_file);
			selector		= data["selector"]; 					strdelete(selector);
			model			= data["model"];
			scale			= data["scale"];
			denoise 		= data["denoise"];
			syncgap 		= data["syncgap"];
			pos_slash	 	= input_path.find_last_of('\\');
			pos_dot 		= input_path.find_last_of('.');
			is_file 		= pos_dot != string::npos && (pos_slash == string::npos || pos_slash < pos_dot);
			match			= data["match"];
			tree_restore	= data["tree_restore"];
			subdir_find		= data["subdir_find"];
			emptydir_find	= data["emptydir_find"];
			addon			= data["addon"];
			if (is_file) {
				temp_path = temp_path + "\\" + input_path.substr(pos_slash);
				strdelete(temp_path);
			}
		} catch (exception &ERROR) {
			return true;
		}
		return false;
	}
	Config() {
		input_path = output_path = temp_path = traverse_file = selector = "";
		model = scale = denoise = syncgap = "";
		pos_slash = pos_dot = is_file = 0;
		match = RegexRule();
		tree_restore = subdir_find = emptydir_find = false;
	}
};

string removePathPrefix(const string &inputPath, const string &fullPath) { return fullPath.substr(inputPath.length()); }
bool checkinRegex(const RegexRule &rules, const string &name, const string &path) {
	for (RegexRule::_rule x : rules.rule) {
		bool res1, res2;
		res1 = res2 = true;
		if (x._name != "") {
			regex match(x._name);
			if (x._name_mode == "search")
				res1 = regex_search(name, match);
			else
				res1 = regex_match(name, match);
		}
		if (x._path != "") {
			regex match(x._path);
			if (x._path_mode == "search")
				res2 = regex_search(name, match);
			else
				res2 = regex_match(name, match);
		}
		if (res1 && res2) return false;
	}
	return true;
}
inline void makedir(string path) {
	system(("md " + strquote(path) + " >nul 2>&1").c_str());
}
inline void removedir(string path) {
	system(("rd " + strquote(path) + " >nul 2>&1").c_str());
}
inline void removesubdir(string path) {
	system(("@for /f \"delims=\" %d in ('dir /ad /s /b " + strquote(path) + " ^| sort /r') do @(rd \"%d\" >nul 2>&1)").c_str());
}
inline void copyfile(string file_path, string direction_path) {
	system(("xcopy /c/q/g/k/r/h/y/-i " + strquote(file_path) + " " + strquote(direction_path) + " >nul 2>&1").c_str());
}
inline void executefile(string file) { system(file.c_str()); }
inline void deletefile(string file) {
	system(("del /f/s/q " + strquote(file) + " >nul 2>&1").c_str());
}
#define Waifu2x_Anime "models-upconv_7_anime_style_art_rgb"
#define Waifu2x_Photo "models-upconv_7_photo"
inline int getgroup_model(const string &model) {
	if (model == "realesrgan" || model == "realesrnet" || model == "realesrgan-anime" || model == "DF2K" || model == "DF2K-JPEG") return 1;
	if (model == "waifu2x-anime" || model == "waifu2x-photo") return 2;
	if (model == "realcugan") return 3;
	return 0;
}
inline void core(const Config &file) {
	string command;
	if (getgroup_model(file.model) == 1) {
		command = (string)"models\\realesrgan.exe"
		          + " -i " + strquote(file.temp_path)
		          + " -o " + strquote(file.temp_path)
		          + " -n " + strquote(file.model + "-x" + file.scale)
		          + " -s " + file.scale;
	}
	if (getgroup_model(file.model) == 2) {
		command = (string)"models\\waifu2x.exe"
		          + " -i " + strquote(file.temp_path)
		          + " -o " + strquote(file.temp_path)
		          + " -m " + strquote(file.model == "waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
		          + " -s " + file.scale
		          + " -n " + file.denoise;
	}
	if (getgroup_model(file.model) == 3) {
		command = (string)"models\\realcugan.exe"
		          + " -i " + strquote(file.temp_path)
		          + " -o " + strquote(file.temp_path)
		          + " -s " + file.scale
		          + " -n " + file.denoise
		          + " -c " + file.syncgap;
	}
	executefile(command);
}
void putTraverseData(const Config &config, const vector<TraverseData> &data) {
	json::value jsondata;
	for (TraverseData x : data) {
		if (config.tree_restore)
			x.path = removePathPrefix(config.input_path, x.path);
		x.path = removePathPrefix(config.output_path, config.temp_path) + "\\" + x.path;
		strdelete(x.path);
		if (config.is_file) x.path = x.path.substr(0, x.path.find_last_of(x.name) - x.name.size() + 1);
		if (x.path == "") x.path = ".";
		strdelete(x.path);
		jsondata.push_back(
		json::object( {
			{ "path", x.path },
			{ "name", x.name },
			{ "size", x.size }
		}));
	}
	ofstream file(config.traverse_file.c_str());
	if (!file.is_open()) return;
	file << json::wrap(jsondata);
	file.close();
}
void process(const Config &config, const string &configPath, const string &selfPath) {
	makedir(config.output_path);
	copyfile(configPath, config.output_path);

	vector<TraverseData> data;
	if (config.is_file) {
		copyfile(config.input_path, config.temp_path);
		core(config);
		struct stat file;
		stat(config.temp_path.c_str(), &file);
		string filepath, filename;
		filepath = config.input_path; strdelete(filepath);
		strsplit(filepath, filepath, filename);
		data.push_back({_A_NORMAL, (unsigned)file.st_size, config.input_path, filename});
	} else {
		makedir(config.temp_path);
		traverse(config.input_path, data, config.selector, {true, true, config.subdir_find, config.emptydir_find});
		for (TraverseData x : data) {
			if (config.tree_restore) {
				string newPath = config.temp_path + removePathPrefix(config.input_path, x.fullpath());
				strdelete(newPath);
				if (x.attrib.is_folder())
					makedir(newPath);
				else if (checkinRegex(config.match, x.name, x.path)) {
					copyfile(x.fullpath(), newPath);
					Config config_subfile = config;
					config_subfile.temp_path = newPath;
					core(config_subfile);
				}
			} else {
				if (!x.attrib.is_folder() && checkinRegex(config.match, x.name, x.path)) {
					string newPath = config.temp_path + "\\" + x.name;
					strdelete(newPath);
					copyfile(x.fullpath(), newPath);
					Config config_subfile = config;
					config_subfile.temp_path = newPath;
					core(config_subfile);
				}
			}
		}
	}
	putTraverseData(config, data);

	for (string x : config.addon.execute) {
		copyfile("execute\\" + x, config.output_path + "\\" + x);
		chdir(config.output_path.c_str());
		executefile((x + " " + strquote(configPath)));
		deletefile(x);
		chdir(selfPath.c_str());
	}

	chdir(selfPath.c_str());
	string name, path;
	strsplit(configPath, path, name);
	path = config.output_path + "\\" + name;
	strdelete(path);
	deletefile(path);
	deletefile(config.traverse_file);
	removesubdir(config.temp_path);
	removedir(config.temp_path);
}

int main(int n, char **configPath) {
	string selfPath, selfName;
	strsplit(_pgmptr, selfPath, selfName);
	strdelete(selfPath), strdelete(selfName);
	for (int i = 1; i < n; ++i) {
		Config config;
		json::value configData;
		bool is_failed;
		is_failed = loadConfig(configPath[i], configData);
		is_failed = config.init(configData);
		if (is_failed) {
			puts("Error");
//			Create config file as default && reload
			continue;
		}
		process(config, configPath[i], selfPath);
	}
	return 0;
}
