#include <iostream>
#include <regex>
#include <fstream>
#include <stdlib.h>
#include "include/configor/json.hpp"
#include "include/path.h"
#include "include/system.h"

using namespace std;
using namespace path;
using namespace configor;
string selfPath, selfName;

inline bool loadConfig(string path, json::value &data) {
	data.clear();
	ifstream file(path.c_str());
	if (!file.is_open()) return true;
	try {
		file >> json::wrap(data);
	} catch (exception &ERROR) { return true; }
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
		    OPTIONAL(_name_mode, "name_mode"),
		    OPTIONAL(_path_mode, "path_mode")
		);
	};
	vector<_rule> rule;
	inline void init(const json::value &data) {
		for (json::value x : data) rule.push_back(x);
	}
	inline bool check(const string &name, const string &path) const {
		for (RegexRule::_rule x : rule) {
			bool res1, res2;
			res1 = res2 = true;
			if (x._name != "") {
				regex match(x._name);
				if (x._name_mode == "search") res1 = regex_search(name, match);
				else res1 = regex_match(name, match);
			}
			if (x._path != "") {
				regex match(x._path);
				if (x._path_mode == "search") res2 = regex_search(name, match);
				else res2 = regex_match(name, match);
			}
			if (res1 && res2) return false;
		}
		return true;
	}
	void operator= (const json::value &data) { init(data); }
	RegexRule() { rule.clear(); }
};
#define Addon_config to_string(config.config_data)
#define Addon_first config.addons.first
#define Addon_second config.addons.second
#define Addon_third config.addons.third
struct Addon {
	vector<string> first, second, third;
	inline void init(const json::value &data) {
		if (data.count("first") > 0) for (json::value x : data["first"]) first.push_back(x);
		if (data.count("second") > 0) for (json::value x : data["second"]) second.push_back(x);
		if (data.count("third") > 0) for (json::value x : data["third"]) third.push_back(x);
	}
	void operator= (const json::value &data) { init(data); }
	Addon() { first.clear(), second.clear(), third.clear(); }
};
struct Config {
	json::value config_data;
	string config_path;
	string input_path, output_path, error_path, selector;
	string model, scale, denoise, syncgap;
	size_t pos_slash, is_file;
	bool tree_restore, subdir_find, emptydir_find;
	bool file_error, dir_error;
	RegexRule match;
	Addon addons;

	CONFIGOR_BIND(
	    json::value, Config,
	    REQUIRED(input_path, "input_path"),
	    REQUIRED(output_path, "output_path"),
	    OPTIONAL(error_path, "error_path"),
	    OPTIONAL(selector, "selector"),
	    REQUIRED(model, "model"),
	    REQUIRED(scale, "scale"),
	    OPTIONAL(denoise, "denoise"),
	    OPTIONAL(syncgap, "syncgap"),
	    REQUIRED(tree_restore, "tree_restore"),
	    REQUIRED(subdir_find, "subdir_find"),
	    OPTIONAL(emptydir_find, "emptydir_find"),
	    OPTIONAL(file_error, "file_error"),
	    OPTIONAL(dir_error, "dir_error")
	);
	inline void init() {
		pathDelete(input_path);
		pathDelete(output_path);
		pathDelete(error_path);
		pathDelete(selector);
		pos_slash	 	= input_path.find_last_of('\\');
		if (pos_slash == string::npos) pos_slash = 0;
		is_file 		= getFileAttribute(input_path).is_file();
	}
	inline bool init(const string &configPath) {
		json::value configData;
		bool is_failed = loadConfig(configPath, configData);
		if (is_failed) return true;
		*this = configData;

		config_data = configData;
		config_path = configPath;
		init();
		if (configData.count("match") > 0) match.init(configData["match"]);
		if (configData.count("addons") > 0) addons.init(configData["addons"]);
		return false;
	}
	inline Config getSubConfig(const TraverseData *data) const {
		Config _file = *this;
		if (_file.is_file) {
			_file.input_path = output_path + "\\" + input_path.substr(pos_slash);
		} else {
			if (tree_restore) _file.input_path = output_path + "\\" +  data->fullpath().substr(input_path.length());
			else _file.input_path = output_path + "\\" + data->name;
		}
		_file.output_path = _file.input_path;
		_file.init();
		return _file;
	}
	inline bool checkRegex(const string &name, const string &path) const { return match.check(name, path); }
	Config() {
		config_data = json::value();
		config_path = "";
		input_path = output_path = model = scale = "";
		error_path = "";
		selector = "*";
		denoise = syncgap = "0";
		pos_slash = is_file = 0;
		tree_restore = subdir_find = emptydir_find = false;
		file_error = true, dir_error = false;
		match = RegexRule();
		addons = Addon();
	}
};

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
		          + " -i " + pathQuote(file.input_path)
		          + " -o " + pathQuote(file.output_path)
		          + " -n " + pathQuote(file.model + "-x" + file.scale)
		          + " -s " + file.scale;
	}
	if (getgroup_model(file.model) == 2) {
		command = (string)"models\\waifu2x.exe"
		          + " -i " + pathQuote(file.input_path)
		          + " -o " + pathQuote(file.output_path)
		          + " -m " + pathQuote(file.model == "waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
		          + " -s " + file.scale
		          + " -n " + file.denoise;
	}
	if (getgroup_model(file.model) == 3) {
		command = (string)"models\\realcugan.exe"
		          + " -i " + pathQuote(file.input_path)
		          + " -o " + pathQuote(file.output_path)
		          + " -s " + file.scale
		          + " -n " + file.denoise
		          + " -c " + file.syncgap;
	}
	chdir(selfPath.c_str());	// 不知道是哪里出问题了, 这里必须手动切换目录.
	executefile(command);
}

string to_string(const json::value &x) {
	string res = json::dump(x);
	for (size_t i = 0; i < res.size(); ++i)
		if (res[i] == '\"')
			res.insert(i++, "\\");
	pathDelete(res);
	return res;
}
string Addon_getRelativePath(const Config &config, string x) {
	x = x.substr(config.output_path.size()); pathDelete(x);
	return x;
}
json::value to_json(const Config &config, const vector<string> &data) {
	json::value jsondata;
	for (string x : data)
		jsondata.push_back(Addon_getRelativePath(config, x));
	return jsondata;
}

inline void process_error(const Config &config, const Config &data) {
	string errorPath = config.error_path;
	errorPath = errorPath + "\\" + data.input_path.substr(0, data.pos_slash).substr(config.output_path.size());
	pathDelete(errorPath);
	makedir(errorPath);
	movefile(data.input_path, errorPath);
}
void executeAddon(const Config &config, const vector<string> &execute, const string &parameter1, const string &parameter2) {
	for (string x : execute) {
		copyfile("addons\\" + x, config.output_path + "\\" + x);
		chdir(config.output_path.c_str());
		executefile(x + " " + pathQuote(parameter1) + " " + pathQuote(parameter2));
		deletefile(x);
		chdir(selfPath.c_str());
	}
}

void process(const Config &config) {
	makedir(config.output_path);
	executeAddon(config, Addon_first, Addon_config, "");

	vector<string> data_addons;
	if (config.is_file) {
		Config _file = config.getSubConfig(nullptr);
		if (config.file_error && pathExist(_file.input_path)) {
			process_error(config, _file);
		}
		copyfile(config.input_path, _file.input_path);
		data_addons.push_back(_file.input_path);
		core(_file);
		executeAddon(config, Addon_second, Addon_config, Addon_getRelativePath(config, _file.input_path));
	} else {
		vector<TraverseData> data;
		traverse(config.input_path, data, config.selector, {true, true, config.subdir_find, config.emptydir_find});
		for (TraverseData x : data) {
			if (!x.attrib.is_folder() && config.checkRegex(x.name, x.path)) {
				Config _file = config.getSubConfig(&x);
				if (config.file_error && pathExist(_file.input_path)) {
					process_error(config, _file);
				}
				data_addons.push_back(_file.input_path);
				copyfile(x.fullpath(), _file.input_path);
				core(_file);
				executeAddon(config, Addon_second, Addon_config, Addon_getRelativePath(config, _file.input_path));
			}
			if (config.tree_restore && x.attrib.is_folder() && config.checkRegex(x.name, x.path)) {
				Config _dir = config.getSubConfig(&x);
				if (config.dir_error && pathExist(_dir.input_path)) {
					process_error(config, _dir);
				}
				data_addons.push_back(_dir.input_path);
				makedir(_dir.input_path);
				executeAddon(config, Addon_second, Addon_config, Addon_getRelativePath(config, _dir.input_path));
			}
		}
	}

	executeAddon(config, Addon_third, Addon_config, to_string(to_json(config, data_addons)));
}

int main(int n, char **configPath) {
	pathSplit(_pgmptr, selfPath, selfName);
	pathDelete(selfPath), pathDelete(selfName);
	for (int i = 1; i < n; ++i) {
		Config config;
		bool is_failed = config.init(configPath[i]);
		if (is_failed) {
			puts("ConfigFile Error");
			system("pause");
//			Create config file as default && reload
			continue;
		}
		process(config);
	}
	return 0;
}
