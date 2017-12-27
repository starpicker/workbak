#ifndef MLTIME_H__
#define MLTIME_H__

#include <sys/time.h>
#include <stdio.h>
//#include "Log.h"

struct MLTIME {
		MLTIME(const char* l = 0):label(l)
		{
			gettimeofday(&t_start, 0);
		}
		
		~MLTIME()
		{
			gettimeofday(&t_end, 0);
			long time_diff = (t_end.tv_sec - t_start.tv_sec)*1000 + (t_end.tv_usec - t_start.tv_usec)/1000;
			//log(Verb, "%s cost time is %ld ms", label, time_diff);
			printf("%s cost time is %ld ms\n", label, time_diff);
		}
private:
		MLTIME operator =(const MLTIME&);
		
private:
	struct timeval t_start;
	struct timeval t_end;
	const char* label;
};

#define T MLTIME t = __func__;
#define MLT MLTIME t
#define MLT1 MLTIME t1
#define MLT2 MLTIME t2
#define MLT3 MLTIME t3

#endif // MLTIME_H__