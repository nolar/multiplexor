//???#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "tslog.h"

void tslog (int level, const char * id, const char * format, ...)
{
	va_list ap;
	va_start(ap, format);

	time_t time_unix;
	struct tm time_parsed;
	time(&time_unix);
	localtime_r(&time_unix, &time_parsed);
	time_parsed.tm_year += 1900;
	time_parsed.tm_mon  += 1;

	char *string1, *string2;
	int length1 = vasprintf(&string1, format, ap);
	int length2 =  asprintf(&string2, "%04d-%02d-%02d %02d:%02d:%02d L%X [%s] %s\n",
		time_parsed.tm_year, time_parsed.tm_mon, time_parsed.tm_mday,
		time_parsed.tm_hour, time_parsed.tm_min, time_parsed.tm_sec,
		level & 0xf, id, string1);
	//todo: check if strings are NULL && lengths == -1

	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	write(2, string2, length2);
	pthread_mutex_unlock(&mutex);

	free(string1);
	free(string2);
	va_end(ap);
}
