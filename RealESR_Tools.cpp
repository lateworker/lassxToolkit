#include ".\\include\\consolegui.hpp"
#include ".\\include\\path.h"
#include ".\\include\\record.h"
using namespace std;
Record record, lastrecord;
Path path;
string getSelector(string selector) {
	if (selector.find('.') == string::npos) {
		return "\\*." + selector;
	}
	return "\\" + selector;
}
class RealesrTools_Console {
	public:
		string title;
		Record menustyle;
		Menu mainMenu;

		RealesrTools_Console() {}
		~RealesrTools_Console() {}

		void loadfromConfig(Record config) {
			title = config.data[0];
			system(("title " + title).c_str());
			record.setRecordPath(config.data[1]);
			record.getRecord_utf8();
			menustyle.filename = config.data[2];
			menustyle.getRecord_utf8();
		}
};
class aiMode {
	public:
		string model;
		string scale;
		string inputpath;
		string outputpath;
		string selector;
		string noise;

		aiMode() {
			resetValue();
		}
		~aiMode() {}

		void resetValue() {
			model = "realesrgan-anime";
			scale = "4";
			inputpath = ".\\input";
			outputpath = ".\\output";
			selector = "*";
			noise = "0";
		}
		void loadfromConfig(const vector<string> &data) {
			model = data[0];
			scale = data[1];
			inputpath = data[2];
			outputpath = data[3];
			selector = data[4];
			noise = data[5];
		}
};
RealesrTools_Console RealesrTools;
aiMode aiConfig;
void processDir(Dir &data) {
	string newpath = aiConfig.outputpath + data.path.substr(aiConfig.inputpath.length());
	Dir dir(newpath);
	if (dir.check() and dir.create()) {
		exit(1);//可以加提示
	}
}
void processFile(File &data) {
	string newpath = aiConfig.outputpath + data.path.substr(aiConfig.inputpath.length());
	string command;
	if (aiConfig.model == "realesrgan" or aiConfig.model == "realesrnet" or aiConfig.model == "realesrgan-anime" or aiConfig.model == "DF2K" or aiConfig.model == "DF2K-JPEG") {
		command =
		    (string)"models\\realesrgan.exe" + " " +
		    "-i \"" + (data.path + "\\" + data.name) + "\" " +
		    "-o \"" + (newpath + "\\" + data.name) + "\" " +
		    "-n " + (aiConfig.model + "-x" + aiConfig.scale) + " " +
		    "-s " + aiConfig.scale;
	} else if (aiConfig.model == "realcugan") {
		command =
		    (string)"models\\realcugan.exe" + " " +
		    "-i \"" + (data.path + "\\" + data.name) + "\" " +
		    "-o \"" + (newpath + "\\" + data.name) + "\" " +
		    "-n " + aiConfig.noise + " " +
		    "-s " + aiConfig.scale;
	} else if (aiConfig.model == "waifu2x-anime") {
		command =
		    (string)"models\\waifu2x.exe" + " " +
		    "-i \"" + (data.path + "\\" + data.name) + "\" " +
		    "-o \"" + (newpath + "\\" + data.name) + "\" " +
		    "-n " + aiConfig.noise + " " +
		    "-s " + aiConfig.scale + " " +
		    "-m models-upconv_7_anime_style_art_rgb";
	} else if (aiConfig.model == "waifu2x-photo") {
		command =
		    (string)"models\\waifu2x.exe" + " " +
		    "-i \"" + (data.path + "\\" + data.name) + "\" " +
		    "-o \"" + (newpath + "\\" + data.name) + "\" " +
		    "-n " + aiConfig.noise + " " +
		    "-s " + aiConfig.scale + " " +
		    "-m models-upconv_7_photo";
	}
	system(command.c_str());
}
void refreshMenu() {
	RealesrTools.mainMenu.keyState = aiConfig.model + "Menu";
	RealesrTools.mainMenu.loadfromConfig(RealesrTools.menustyle.data);
	RealesrTools.mainMenu.linedata[6].text += aiConfig.model;
	RealesrTools.mainMenu.linedata[7].text += "x" + aiConfig.scale;
	RealesrTools.mainMenu.linedata[8].text += aiConfig.inputpath;
	RealesrTools.mainMenu.linedata[9].text += aiConfig.outputpath;
	RealesrTools.mainMenu.linedata[10].text += aiConfig.selector;
	if (aiConfig.model == "realcugan" or aiConfig.model == "waifu2x-anime" or aiConfig.model == "waifu2x-photo") {
		RealesrTools.mainMenu.linedata[11].text += aiConfig.noise;
	}
}
bool isalert = false;
size_t exitMenu() {
	Menu exitMenu("exitMenu");
	exitMenu.loadfromConfig(RealesrTools.menustyle.data);
	size_t choose = 0;
	exitMenu.MenuStart();
	do {
		choose = exitMenu.choose();
	} while (choose == (size_t) -1);
	return choose;
}
void showTips(string keyState) {
	vector<string> tips;
	loadfromConfig(tips, RealesrTools.menustyle.data, keyState);
	for (size_t i = 0; i < tips.size(); ++i) {
		cout << tips[i] << endl;
	}
}
void deleteExtraneousSymbols(string &data) {
	if (data[0] == '\"') data.erase(data.begin());
	if (data[data.length() - 1] == '\"') data.erase(data.end() - 1);
	for (size_t i = data.length() - 1; data[i] == '\\' or data[i] == '/'; --i) {
		data.erase(i);
	}
}
void modelReset() {
	vector<string> defaultConfig = {""};
	loadfromConfig(defaultConfig, RealesrTools.menustyle.data, aiConfig.model + "Default");
	if (defaultConfig[1] == aiConfig.scale and defaultConfig[2] == aiConfig.inputpath and defaultConfig[3] == aiConfig.outputpath and defaultConfig[4] == aiConfig.selector and defaultConfig[5] == aiConfig.noise) return;
	record.data.resize(6);
	record.data[1] = defaultConfig[1];
	record.data[5] = defaultConfig[5];
	aiConfig.loadfromConfig(record.data);
}
void changeModel() {
	Menu modelMenu("modelMenu");
	modelMenu.loadfromConfig(RealesrTools.menustyle.data);
	size_t choose = 0;
	modelMenu.MenuStart();
	do {
		choose = modelMenu.choose();
	} while (choose == (size_t) -1 or choose == 1);
	modelMenu.MenuStop();
	modelMenu.clearScreen();
	string newModel = modelMenu.linedata[choose].text;
	if (newModel == aiConfig.model) return;
	isalert = true;
	record.data[0] = newModel;
	aiConfig.loadfromConfig(record.data);
	modelReset();
	record.putRecord_utf8();
}
void changeScale() {
	Menu scaleMenu(aiConfig.model + "-scaleMenu");
	scaleMenu.loadfromConfig(RealesrTools.menustyle.data);
	size_t choose = 0;
	scaleMenu.MenuStart();
	do {
		choose = scaleMenu.choose();
	} while (choose == (size_t) -1 or choose == 1);
	scaleMenu.MenuStop();
	scaleMenu.clearScreen();
	string newScale = "4";
	if (scaleMenu.linedata.size() > 1) newScale = scaleMenu.linedata[choose].text.substr(1);
	if (newScale == aiConfig.scale) return;
	isalert = true;
	record.data[1] = newScale;
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
}
void changeInputPath() {
	showTips("inputpathTips");
	string newInputPath;
	getline(cin, newInputPath);
	if (newInputPath == "") return;
	deleteExtraneousSymbols(newInputPath);
	size_t pos = newInputPath.find_last_of(".");
	if (pos != string::npos) {
		size_t i = newInputPath.find_last_of('\\');
		if (i == string::npos) {
			i = 0;
		} else {
			++i;
		}
		if (i <= pos) {
			string newSelector = newInputPath.substr(i);
			record.data[4] = newSelector;
			aiConfig.selector = newSelector;
			path.selector = getSelector(aiConfig.selector);
			newInputPath.erase(i);
			deleteExtraneousSymbols(newInputPath);
			if (i == 0) {
				newInputPath = ".";
			}
		}
	}
	if (newInputPath == aiConfig.inputpath) return;
	isalert = true;
	record.data[2] = newInputPath;
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
	path.path = aiConfig.inputpath;
}
void changeOutputPath() {
	showTips("outputpathTips");
	string newOutputPath;
	getline(cin, newOutputPath);
	if (newOutputPath == "") return;
	deleteExtraneousSymbols(newOutputPath);
	if (newOutputPath == aiConfig.outputpath) return;
	isalert = true;
	record.data[3] = newOutputPath;
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
}
void changeSelector() {
	showTips("selectorTips");
	string newSelector;
	getline(cin, newSelector);
	if (newSelector == "") return;
	deleteExtraneousSymbols(newSelector);
	if (newSelector == aiConfig.selector) return;
	isalert = true;
	record.data[4] = newSelector;
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
	path.selector = getSelector(aiConfig.selector);
}
void changeNoise() {
	Menu noiseMenu(aiConfig.model + "-noiseMenu");
	noiseMenu.loadfromConfig(RealesrTools.menustyle.data);
	size_t choose = 0;
	noiseMenu.MenuStart();
	do {
		choose = noiseMenu.choose();
	} while (choose == (size_t) -1 or choose == 1);
	noiseMenu.MenuStop();
	noiseMenu.clearScreen();
	string newNoise = noiseMenu.linedata[choose].text;
	if (choose == 2) newNoise = newNoise.substr(2, 2);
	else newNoise = newNoise.substr(3, 1);
	if (newNoise == aiConfig.noise) return;
	isalert = true;
	record.data[5] = newNoise;
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
}
void Save() {
	isalert = false;
	lastrecord = record;
}
void Reset() {
	vector<string> defaultConfig = {""};
	loadfromConfig(defaultConfig, RealesrTools.menustyle.data, aiConfig.model + "Default");
	if (defaultConfig[1] == aiConfig.scale and defaultConfig[2] == aiConfig.inputpath and defaultConfig[3] == aiConfig.outputpath and defaultConfig[4] == aiConfig.selector and defaultConfig[5] == aiConfig.noise) return;
	isalert = true;
	record.data.resize(6);
	for (size_t i = 1; i <= 5; ++i) {
		record.data[i] = defaultConfig[i];
	}
	record.putRecord_utf8();
	aiConfig.loadfromConfig(record.data);
}
void startingCheck(){
	Record config_ini;
	config_ini.filename = ".\\config.ini";
	config_ini.data_length = 3;
	if(config_ini.getRecord_utf8()){
		config_ini.data.clear();
		config_ini.data.push_back("RealESR Tools by Tptr.lateworker");
		config_ini.data.push_back(".\\record.txt");
		config_ini.data.push_back(".\\languages\\zh-CN");
		config_ini.putRecord_utf8();
	}
	Record record_txt;
	record_txt.filename = ".\\record.txt";
	record_txt.data_length = 5;
	if (record_txt.getRecord_utf8()) {
		record_txt.data.clear();
		record_txt.data.push_back("realesrgan-anime");
		record_txt.data.push_back("4");
		record_txt.data.push_back(".");
		record_txt.data.push_back(".");
		record_txt.data.push_back("*");
		record_txt.putRecord_utf8();
	}
	record.getRecord_utf8();
}
int main() {
	startingCheck();
	RealesrTools.loadfromConfig(Record(".\\config.ini"));
	Save();
	aiConfig.loadfromConfig(record.data);
	refreshMenu();
	path.setAll(aiConfig.inputpath, getSelector(aiConfig.selector), processFile, processDir);

	size_t choose = 0;
	RealesrTools.mainMenu.MenuStart();
	do {
		choose = RealesrTools.mainMenu.choose();
		switch (choose) {
			case 1: {
				MessageBox(nullptr, "For all OIer. For Tt.", "Tptr-lateworker", MB_OK);
				break;
			}
			case 2: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				Dir outputdir(aiConfig.outputpath);
				if (outputdir.check() and outputdir.create()) {
					exit(1);//可以加提示
				}
				path.get();
				system("cls");
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 3: {
				RealesrTools.mainMenu.MenuStop();
				choose = 0;
				if (isalert) {
					puts("");
					switch (exitMenu()) {
						case 3: {
							lastrecord.putRecord_utf8();
						}
					}
				}
				break;
			}
			case 6: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				changeModel();
				refreshMenu();
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 7: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				changeScale();
				refreshMenu();
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 8: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				changeInputPath();
				refreshMenu();
				system("cls");
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 9: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				changeOutputPath();
				refreshMenu();
				system("cls");
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 10: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				changeSelector();
				refreshMenu();
				system("cls");
				RealesrTools.mainMenu.MenuStart();
				break;
			}
			case 11: {
				if (aiConfig.model == "realcugan" or aiConfig.model == "waifu2x-anime" or aiConfig.model == "waifu2x-photo") {
					RealesrTools.mainMenu.MenuStop();
					RealesrTools.mainMenu.clearScreen();
					changeNoise();
					refreshMenu();
					RealesrTools.mainMenu.MenuStart();
				} else {
					Save();
				}
				break;
			}
			case 12: {
				if (aiConfig.model == "realcugan" or aiConfig.model == "waifu2x-anime" or aiConfig.model == "waifu2x-photo") {
					Save();
				} else {
					RealesrTools.mainMenu.MenuStop();
					RealesrTools.mainMenu.clearScreen();
					Reset();
					refreshMenu();
					RealesrTools.mainMenu.MenuStart();
				}
				break;
			}
			case 13: {
				RealesrTools.mainMenu.MenuStop();
				RealesrTools.mainMenu.clearScreen();
				Reset();
				refreshMenu();
				RealesrTools.mainMenu.MenuStart();
				break;
			}
		}
	} while (choose);
	RealesrTools.mainMenu.MenuStop();
	return 0;
}
