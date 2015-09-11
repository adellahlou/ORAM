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

	// Just display the message
	fprintf(stderr, "%s: %s\n", prefix, msg);

	// Fatal errors stop the program
	if (type == FATAL) {
		abort();
	}
}
