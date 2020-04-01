/***************************************************************************
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

#include "xf_headers.h"
#include "tb_headers.h"
#include "xf_ip_accel_config.h"

int main(int argc, char** argv)
{

	if(argc != 2)
	{
		fprintf(stderr,"Invalid Number of Arguments!\nUsage:\n");
		fprintf(stderr,"<Executable Name> <input image path> \n");
		return -1;
	}

	cv::Mat in_img;
	cv::Mat out_img;

	// reading in the color image
	in_img = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (in_img.data == NULL)
	{
		fprintf(stderr,"Cannot open image at %s\n", argv[1]);
		return 0;
	}
	out_img.create(HEIGHT, WIDTH,CV_8UC3);

	uint16_t height = in_img.rows;
	uint16_t width = in_img.cols;

	hls::stream< ap_axiu<24,1,1,1> > _src;
	hls::stream< ap_axiu<24,1,1,1> > _dst;

	#if __SDSCC__
	perf_counter hw_ctr;
	hw_ctr.start();
	#endif

	cvMat2AXIvideoxf<XF_NPPC1>(in_img, _src);

	blobs_accel(_src, _dst);

	AXIvideo2cvMatxf<XF_NPPC1>(_dst, out_img);

	#if __SDSCC__
	hw_ctr.stop();
	uint64_t hw_cycles = hw_ctr.avg_cpu_cycles();
	#endif

	cv::imwrite("hls_out.png", out_img);

	return 0;

}
