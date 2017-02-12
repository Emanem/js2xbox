#include "log.h"
#include <unistd.h>
#include <sys/time.h>

std::string log::header(const char* type) {
	char			tm_str[64],
				p_fmt[260];
	struct timeval  	cur_tv;
	struct tm       	cur_tm;
	static const pid_t	cur_pid = getpid();

	gettimeofday(&cur_tv, 0);
	localtime_r(&cur_tv.tv_sec, &cur_tm);
	strftime(tm_str, 63, "[%%d]\t%Y-%m-%d %H:%M:%S.%%03d\t(%%s)\t", &cur_tm);
	snprintf(p_fmt, 259, tm_str, cur_pid, cur_tv.tv_usec/1000, type);

	return p_fmt;
}

