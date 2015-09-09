#pragma once

class Log {
public:
	enum LogType {
		WARNING,
		FATAL
	};

	static void Write(LogType type, const char *str);
};
