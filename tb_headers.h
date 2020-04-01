#ifndef _TB_HEADERS_H_
#define _TB_HEADERS_H_

#if __SDSCC__
#undef __ARM_NEON__
#undef __ARM_NEON
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define __ARM_NEON__
#define __ARM_NEON
#else
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#endif



#if __SDSCC__
#include "sds_lib.h"
#define TIME_STAMP_INIT  unsigned int clock_start, clock_end;  clock_start = sds_clock_counter();
#define TIME_STAMP  { clock_end = sds_clock_counter(); printf("elapsed time %lu \n", clock_end-clock_start); clock_start = sds_clock_counter();  }
#endif


#include "common/xf_axi.h"
#include "common/xf_common.h"
#include "common/xf_utility.h"
#include "hls_stream.h"

#endif //_TB_HEADERS_H_
