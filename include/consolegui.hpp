#ifndef MENU_HPP
#define MENU_HPP
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sys/time.h>
using namespace std;
// 一些其他的函数
long getTimeDifference(const struct timeval &start,  const struct timeval &end) {
	return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}

// 与控制台有关的设置
// 隐藏鼠标光标
void hidecursor() {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
// 显示鼠标光标
void showcursor() {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
// 移除控制台属性
void removeAttributes() {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;	//移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE;		//移除插入模式
	mode |= ENABLE_MOUSE_INPUT;			//添加鼠标输入
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
// 添加控制台属性
void addAttributes() {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode |= ENABLE_QUICK_EDIT_MODE;		//添加快速编辑模式
	mode |= ENABLE_INSERT_MODE;     	//添加插入模式
	mode |= ENABLE_MOUSE_INPUT;			//添加鼠标输入
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

// 与鼠标有关的函数
// 获取鼠标位置
COORD getcursor() {
	CONSOLE_SCREEN_BUFFER_INFO temp;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
	return temp.dwCursorPosition;
}
// 设置鼠标位置
void setcursor(const COORD &temp = {0, 0}) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
// 等待鼠标事件，事件包括鼠标移动、点击、滚轮滚动等一切由鼠标出发的事件
MOUSE_EVENT_RECORD waitMouseEvent(bool move = true) {
	INPUT_RECORD record; //输入事件
	DWORD reg;           //临时寄存
	while (true) {
		Sleep(10);
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //将输入事件存入record
		if (record.EventType == MOUSE_EVENT and (move | (record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED))) //是鼠标事件 && 移动事件与模式对应
			return record.Event.MouseEvent;
	}
}
// 调试代码，用于输出COORD变量，打印鼠标位置
ostream &operator<<(ostream &pout, const COORD &temp) {
	pout.setf(ios_base::fixed);
	pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
	return pout;
}

// 与控制台文本颜色有关的类
// 一些常用颜色组合的定义
#define BlackWhite 0x07
#define BlackCyan 0x0b
#define YellowBlue 0xe9
struct Color {
	short Highlight;	// 文本高亮颜色组合
	short Default;		// 默认颜色组合
	short lastcolor;	// 用于实现切换颜色组合，属于辅助变量

	// 初始化
	Color() : Highlight(BlackCyan), Default(BlackWhite), lastcolor(BlackWhite) {}
	Color(short Highlight = BlackCyan, short Default = BlackWhite) : Highlight(Highlight), Default(Default), lastcolor(BlackWhite) {}

	// 将文本颜色设置为Default组合
	void setDefault() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Default);
		lastcolor = Default;
	}
	// 将文本颜色设置为Highlight组合
	void setHighlight() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Highlight);
		lastcolor = Highlight;
	}
};

// Menu类的前置声明，Parameter类中使用
class Menu;
// 定义Parameter成员变量状态
// buttonState变量值的定义
#define mouseLeftButton FROM_LEFT_1ST_BUTTON_PRESSED	// 左键
#define mouseMiddleButton FROM_LEFT_2ND_BUTTON_PRESSED	// 中键
#define mouseRightButton RIGHTMOST_BUTTON_PRESSED		// 右键
// eventFlag变量值的定义
#define mouseClick 0x0					// 鼠标按键按下或松开时触发
#define mouseMove MOUSE_MOVED			// 鼠标移动时触发
#define mouseDoubleClick DOUBLE_CLICK	// 鼠标第二次按下时触发，触发此事件前一定会触发 mouseClick 事件
#define mouseWheel MOUSE_WHEELED		// 鼠标滚轮滚动时触发
// *****Parameter类必须作为回调函数参数传入*****
struct Parameter {
	DWORD buttonState;		// 鼠标按键记录
	DWORD controlkeyState;	// 控制按键状态
	DWORD eventFlag;		// 鼠标事件记录

	// 初始化
	Parameter() : buttonState(mouseLeftButton), controlkeyState(0), eventFlag(0) {}
	Parameter(MOUSE_EVENT_RECORD mouseEvent) : buttonState(mouseEvent.dwButtonState), controlkeyState(mouseEvent.dwControlKeyState), eventFlag(mouseEvent.dwEventFlags) {}
};

// 菜单的文本类，每个Text型变量都代表菜单中的一行
struct Text {
	string text;		// 文本内容
	Color color;		// 颜色属性
	COORD position;		// 文本位置

	// 初始化
	Text() : text(""), color(Color(0, 0)), position({0, 0}) {}
	Text(string text, Color color) : text(text), color(color), position({0, 0}) {}

	// 判断鼠标坐标是否与文本位置重合
	bool operator== (const COORD &mousePosition) const {
		if (position.Y == mousePosition.Y and position.X <= mousePosition.X and mousePosition.X <= position.X + (short)text.length() - 1) {
			return true;
		}
		return false;
	}
	// 判断鼠标坐标是否不与文本位置重合
	bool operator!= (const COORD &mousePosition) const {
		return !operator==(mousePosition);
	}
};
COORD operator+ (const COORD x, const int y) {
	return {x.X, (short)(x.Y + y)};
}

// *****菜单类*****
class Menu {
	private:
		struct timeval timeStart, timeNow;
	protected:
		// 输出
		void write(string text, bool isEndl = false) {
			printf("%s", text.c_str());
			if (isEndl) {
				printf("\n");
			}
		}
		// 整行覆盖输出
		void rewrite(Text data) {
			COORD lastposition = getcursor();
			setcursor({0, data.position.Y});
			// 清空旧数据
			for (short j = 0; j < data.position.X; ++j) {
				write(" ");
			}
			setcursor({0, data.position.Y});
			// 输出新数据
			write(data.text);
			setcursor(lastposition);
		}
		// 输出菜单，并对linedata内position成员变量初始化
		void initializePosition() {
			for (size_t i = 1; i < linedata.size(); ++i) {
				linedata[i].position = getcursor();
				linedata[i].color.setDefault();
				write(linedata[i].text, true);
			}
		}
		// 刷新菜单
		void refresh(COORD hangPosition) {
			for (size_t i = 1; i < linedata.size(); ++i) {
				// 如果鼠标位于%data%按钮上 并且 %data%按钮不处于高亮
				if (linedata[i] == hangPosition and linedata[i].color.lastcolor != linedata[i].color.Highlight) {
					linedata[i].color.setHighlight();	// 设置为高亮颜色
					rewrite(linedata[i]);
				}
				// 如果鼠标不位于%data%按钮上 并且 %data%按钮处于高亮
				if (linedata[i] != hangPosition and linedata[i].color.lastcolor != linedata[i].color.Default) {
					linedata[i].color.setDefault();	// 设置为默认颜色
					rewrite(linedata[i]);
				}
			}
		}
	public:
		long timeLimit;					// 菜单显示时间限制, 单位ms
		vector<Text> linedata = {Text()};	// 存放菜单每行的信息
		string keyState;

		// 构造函数
		Menu() : timeLimit(-1), keyState("") {}
		Menu(long timeLimit) : timeLimit(timeLimit), keyState("") {}
		Menu(string keyState) : timeLimit(-1), keyState(keyState) {}
		~Menu() {}

		bool loadfromConfig(const vector<string> &config) {
			linedata = {Text()};
			for (size_t i = 0; i < config.size(); ++i) {
				if (config[i] == keyState + "{") {
					string text, color;
					size_t pos;
					while (config[i + 1] != "}") {
						++i;
						pos = config[i].find("\":\"");
						if (pos == string::npos) continue;
						if (config[i][0] != '\"' or config[i][config[i].length() - 1] != '\"') continue;
						text = config[i].substr(1, pos - 1);
						color = config[i].substr(pos + 3, config[i].size() - pos - 4);
						linedata.push_back(Text(text, stoi(color)));
					}
				}
			}
			return false;
		}
		// 向菜单尾部添加一行
		Menu &push_back(string text = "", short colorHighlight = BlackCyan, short colorDefault = BlackWhite) {
			linedata.push_back(Text(text, Color(colorHighlight, colorDefault)));
			return *this;
		}
		// 从菜单尾部弹出一行
		Menu &pop_back() {
			linedata.pop_back();
			return *this;
		}
		// 清空菜单
		Menu &clear() {
			linedata.clear();
			return *this;
		}
		// 设置菜单显示时间
		void set_timeLimit(long _timeLimit) {
			timeLimit = _timeLimit;
		}
		void clearScreen() {
			if (linedata.size() == 1) return;
			for (size_t i = 1; i < linedata.size(); ++i) {
				setcursor(linedata[i].position);
				for (size_t j = 0; j < linedata[i].text.length(); ++j) {
					write(" ");
				}
			}
			setcursor(linedata[1].position);
		}
		void MenuStart() {
			if (linedata.size() == 1) return;
			gettimeofday(&timeStart, nullptr);
			removeAttributes();
			hidecursor();
			initializePosition();
		}
		long MenuStop() {
			if (linedata.size() == 1) return 0;
			setcursor(linedata[linedata.size() - 1].position + 1);
			for (size_t i = 1; i < linedata.size(); ++i) {
				if (linedata[i].color.lastcolor == linedata[i].color.Highlight) {
					linedata[i].color.setDefault();
					rewrite(linedata[i]);
				}
			}
			addAttributes();
			showcursor();
			return getTimeDifference(timeStart, timeNow);
			Sleep(100);
		}
		// 显示菜单, 返回值：0结束，1~n行数，inf无效点击
		size_t choose() {
			if (linedata.size() == 1) return 0;
			removeAttributes();
			hidecursor();
			MOUSE_EVENT_RECORD mouseEvent;
			while (timeLimit == -1 or getTimeDifference(timeStart, timeNow) < timeLimit) {
				Sleep(50);
				mouseEvent = waitMouseEvent();
				switch (mouseEvent.dwEventFlags) {
					case mouseMove: {
						refresh(mouseEvent.dwMousePosition);
						break;
					}
					case mouseClick: {
						if (mouseEvent.dwButtonState and mouseEvent.dwButtonState != mouseWheel) {
							setcursor(linedata[linedata.size() - 1].position + 1);
							for (size_t i = 1; i < linedata.size(); ++i) {
								if (linedata[i] == mouseEvent.dwMousePosition) {
									linedata[i].color.setDefault();
									rewrite(linedata[i]);
									addAttributes();
									showcursor();
									return i;
								}
							}
							addAttributes();
							showcursor();
							return -1;
						}
						break;
					}
				}
				gettimeofday(&timeNow, nullptr);
			}
			return 0;
		}
};
void loadfromConfig(vector<string> &data, const vector<string> &config, string keyState) {
	for (size_t i = 0; i < config.size(); ++i) {
		if (config[i] == keyState + "{") {
			string text;
			while (config[i + 1] != "}") {
				++i;
				if (config[i][0] != '\"' or config[i][config[i].length() - 1] != '\"') continue;
				text = config[i].substr(1, config[i].length() - 2);
				data.push_back(text);
			}
		}
	}
}
#endif
