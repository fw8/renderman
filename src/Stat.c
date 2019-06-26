#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "Stat.h"

struct Stat RmStat;

get_cpu_time(utime, stime)
double *utime, *stime;
{
        struct rusage usage;

        getrusage(RUSAGE_SELF, &usage);

        *utime = (double)usage.ru_utime.tv_sec +
                        (double)usage.ru_utime.tv_usec / 1000000.;
        *stime = (double)usage.ru_stime.tv_sec +
                        (double)usage.ru_stime.tv_usec / 1000000.;
}

void
_begin_Stat()
{
  RmStat.BoundinRegionTests=0;
  RmStat.BoundinRegionTestsSucceeded=0;
}

void
_end_Stat()
{
  double utime, stime;

  if (RmStat.BoundinRegionTests) {
    fprintf(stderr,"\nperfomance of BoundinRegionTest : %f\n",
    	  (float)RmStat.BoundinRegionTestsSucceeded /
          (float)RmStat.BoundinRegionTests);
  }else{
    fprintf(stderr,"\nno BoundinRegionTests performed");
  }

  get_cpu_time(&utime, &stime);
  fprintf(stderr,"\nTotal CPU time (sec):\t\t");
  fprintf(stderr,"%2.2lf (%2.2lfu + %2.2lfs)\n",utime+stime, utime, stime);
}
