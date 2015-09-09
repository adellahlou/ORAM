#include "Log.hpp"

#include <cstdlib>
#include <cstdio>

void Log::Write(LogType type, const char *msg)
{
	const char *prefix;

	switch (type) {
		case WARNING:
			prefix = "WARNING";
			break;

		default:
		case FATAL:
			prefix = "FATAL";
			break;

	}

	fprintf(stderr, "%s: %s\n", prefix, msg);

	// Quit program on fatal error
	if (type == FATAL) {
		exit(1);
	}
}
