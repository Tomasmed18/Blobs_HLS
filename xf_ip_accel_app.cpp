#include "xf_ip_accel_config.h"

#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_duplicateimage.hpp"
#include "imgproc/xf_threshold.hpp"
#include "core/xf_arithm.hpp"
#include "imgproc/xf_erosion.hpp"
#include "imgproc/xf_dilation.hpp"


#define ERODE_ITERATIONS 10
#define ERODE_FILTER_SIZE 7
unsigned char erode_kernel[ERODE_FILTER_SIZE*ERODE_FILTER_SIZE] ={0,0,0,1,0,0,0,
												0,0,0,1,0,0,0,
												0,0,0,1,0,0,0,
												1,1,1,1,1,1,1,
												0,0,0,1,0,0,0,
												0,0,0,1,0,0,0,
												0,0,0,1,0,0,0};


#define DILATE_ITERATIONS 10
#define DILATE_FILTER_SIZE 11
unsigned char dilation_kernel[DILATE_FILTER_SIZE*DILATE_FILTER_SIZE] =   {0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0,
																			1,1,1,1,1,1,1,1,1,1,1,
																			0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0};

struct blobs{
	unsigned int area;
	//unsigned int perimeter;
	bool valid = false;

	unsigned int minRow, minCol, maxRow, maxCol;

	unsigned int newBlobReference;
};

struct run{
	unsigned short start, end;
	unsigned short blobReference;
};

blobs blobs[1024];
unsigned short blobsCounter = 0;

run runArray1[64];
run runArray2[64];


void verify_overlap_1(unsigned short i, run & run, unsigned int row){
	bool overlapDetected = false; //to control if its the first overlap detected or not
	for(unsigned short c = 0; c < i; c++){
		if (
			((runArray1[c].start <= run.start) && (runArray1[c].end >= run.start)) ||
			((runArray1[c].start <= run.end) && (runArray1[c].end >= run.end)) ||
			((runArray1[c].start >= run.start) && (runArray1[c].end <= run.end)) )
			{

			if (!overlapDetected){ //if it is its first overlap...

				if (blobs[runArray1[c].blobReference].valid){
					run.blobReference = runArray1[c].blobReference; // associate with possible blob from previous line

					// TODO update possible blob features
					blobs[run.blobReference].area += run.end - run.start + 1;

					// update blob position values
					blobs[runArray1[c].blobReference].maxRow = row;
					if (run.start < blobs[runArray1[c].blobReference].minCol)
						blobs[runArray1[c].blobReference].minCol = run.start;
					if (run.end > blobs[runArray1[c].blobReference].maxCol){
						blobs[runArray1[c].blobReference].maxCol = run.end;
					}


				}
				else{
					run.blobReference = blobs[runArray1[c].blobReference].newBlobReference;
					//  update possible blob features
					blobs[run.blobReference].area += run.end - run.start + 1;

					// TODO verify position updates
					blobs[run.blobReference].maxRow = row;
					if (run.start < blobs[run.blobReference].minCol)
						blobs[run.blobReference].minCol = run.start;
					if (run.end > blobs[run.blobReference].maxCol)
						blobs[run.blobReference].maxCol = run.end;
				}

				overlapDetected = true;

			}
			else { // if it is not the first overlapping segment

				if ((run.blobReference != runArray1[c].blobReference) // and this segment is not already associated with the same blob
						&& (blobs[runArray1[c].blobReference].valid)){
					// merge blob features in one
					blobs[run.blobReference].area += blobs[runArray1[c].blobReference].area;
					blobs[runArray1[c].blobReference].newBlobReference = run.blobReference;
					blobs[runArray1[c].blobReference].valid = false;

					// update blob position values
					if (run.end < blobs[runArray1[c].blobReference].maxCol)
						blobs[run.blobReference].maxCol = blobs[runArray1[c].blobReference].maxCol;
					if (blobs[runArray1[c].blobReference].minRow < blobs[run.blobReference].minRow)
						blobs[run.blobReference].minRow = blobs[runArray1[c].blobReference].minRow;
					if (blobs[runArray1[c].blobReference].minCol < blobs[run.blobReference].minCol) //TODO see if necessary
						blobs[run.blobReference].minCol = blobs[runArray1[c].blobReference].minCol;
					if (blobs[runArray1[c].blobReference].maxCol > blobs[run.blobReference].maxCol) //TODO see if necessary
						blobs[run.blobReference].maxCol = blobs[runArray1[c].blobReference].maxCol;

					runArray1[c].blobReference = run.blobReference;

				}
			}


		}

	}

	if (!overlapDetected){ // if there was not another segment that overlaps with this one...

		// associate run with a new blob entry
		run.blobReference = blobsCounter;

		// set it as valid
		blobs[blobsCounter].valid = true;
		// set feature values
		blobs[blobsCounter].area = run.end - run.start + 1;

		// set position values
		blobs[blobsCounter].minRow = row;
		blobs[blobsCounter].maxRow = row;
		blobs[blobsCounter].minCol = run.start;
		blobs[blobsCounter].maxCol = run.end;

		blobsCounter++;
	}

}

void verify_overlap_2(unsigned short i, run & run, unsigned int row){
	bool overlapDetected = false; //to control if its the first overlap detected or not
	for(unsigned short c = 0; c < i; c++){
		if (
			((runArray2[c].start <= run.start) && (runArray2[c].end >= run.start)) ||
			((runArray2[c].start <= run.end) && (runArray2[c].end >= run.end)) ||
			((runArray2[c].start >= run.start) && (runArray2[c].end <= run.end)) )
			{

			if (!overlapDetected){ //if it is its first overlap...

				if (blobs[runArray2[c].blobReference].valid){
					run.blobReference = runArray2[c].blobReference;

					// update blob features
					blobs[run.blobReference].area += run.end - run.start + 1;

					// update blob position values
					blobs[runArray2[c].blobReference].maxRow = row;
					if (run.start < blobs[runArray2[c].blobReference].minCol)
						blobs[runArray2[c].blobReference].minCol = run.start;
					if (run.end > blobs[runArray2[c].blobReference].maxCol)
						blobs[runArray2[c].blobReference].maxCol = run.end;

				}
				else{
					run.blobReference = blobs[runArray2[c].blobReference].newBlobReference;
					// update blob features
					blobs[run.blobReference].area += run.end - run.start + 1;

					//TODO verify position updates
					blobs[run.blobReference].maxRow = row;
					if (run.start < blobs[run.blobReference].minCol)
						blobs[run.blobReference].minCol = run.start;
					if (run.end > blobs[run.blobReference].maxCol)
						blobs[run.blobReference].maxCol = run.end;
				}

				overlapDetected = true;
			}
			else { // if it is not the first overlapping segment

				if ((run.blobReference != runArray2[c].blobReference) // and this segment is not already associated with the same blob
						&& (blobs[runArray2[c].blobReference].valid)){
					// merge blob features in one
					blobs[run.blobReference].area += blobs[runArray2[c].blobReference].area;

					blobs[runArray2[c].blobReference].newBlobReference = run.blobReference;
					blobs[runArray2[c].blobReference].valid = false;

					// update blob position values
					if (blobs[run.blobReference].maxCol < blobs[runArray2[c].blobReference].maxCol)
						blobs[run.blobReference].maxCol = blobs[runArray2[c].blobReference].maxCol;
					if (blobs[runArray2[c].blobReference].minRow < blobs[run.blobReference].minRow)
						blobs[run.blobReference].minRow = blobs[runArray2[c].blobReference].minRow;
					if (blobs[runArray2[c].blobReference].minCol < blobs[run.blobReference].minCol) //TODO see if necessary
						blobs[run.blobReference].minCol = blobs[runArray2[c].blobReference].minCol;

					runArray2[c].blobReference = run.blobReference;
				}

			}

		}

	}

	if (!overlapDetected){ // if there was not another segment that overlaps with this one...

		// associate run with a new blob entry
		run.blobReference = blobsCounter;

		// set it as valid
		blobs[blobsCounter].valid = true;
		// set feature values
		blobs[blobsCounter].area = run.end - run.start + 1;
		// set position values
		blobs[blobsCounter].minRow = row;
		blobs[blobsCounter].maxRow = row;
		blobs[blobsCounter].minCol = run.start;
		blobs[blobsCounter].maxCol = run.end;


		blobsCounter++;
	}

}


void blob_detection(xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> & src,
		xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> & dst){

	//initialize counters
	unsigned short i1 = 0;
	unsigned short i2 = 0;

	bool readingRun = false;
	int whitePixelsRead = 0;

	for(short int j = 0; j < (HEIGHT); j++ ){
		i1 = 0;
		for(short int i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
			//get the pixel brightness value
			unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];
			if (pix == 255)
				whitePixelsRead++;

			if (!readingRun && pix == 255){ //start of run detected

				readingRun = true;
				runArray1[i1].start = i;

			}else if(readingRun && pix == 0){ //end of run detected

				readingRun = false;
				runArray1[i1].end = i - 1;
				verify_overlap_2(i2, runArray1[i1], j);
				i1++;
			}

		}
		if (readingRun){ // if it was reading a run and it finished at the end of the row
			runArray1[i1].end = WIDTH -1;
			verify_overlap_2(i2, runArray1[i1], j);
			readingRun = false;
			i1++;
		}

		j++;

		readingRun = false;
		i2 = 0;
		for(short int i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
					//get the pixel brightness value
					unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];
					if (pix == 255)
						whitePixelsRead++;

					if (!readingRun && pix == 255){ //start of run detected

						readingRun = true;
						runArray2[i2].start = i;

					}else if(readingRun && pix == 0){ //end of run detected

						readingRun = false;
						runArray2[i2].end = i - 1;
						verify_overlap_1(i1, runArray2[i2], j);
						i2++;
					}

				}
		if (readingRun){ // if it was reading a run and it finished at the end of the row
			runArray2[i2].end = WIDTH -1;
			readingRun = false;
			verify_overlap_1(i1, runArray2[i2], j);
			i2++;
		}

	}

}


void blobs_accel(hls::stream< ap_axiu<24,1,1,1> >& _src,hls::stream< ap_axiu<24,1,1,1> >& _dst)
{
#pragma HLS INTERFACE axis register both  port=_src
#pragma HLS INTERFACE axis register both  port=_dst

	 xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> imgInput(HEIGHT, WIDTH); //RGB
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img0(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img1(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img2(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img3(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img4(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> imgOutput(HEIGHT, WIDTH); //RGB


#pragma HLS stream variable=imgInput.data dim=1 depth=1
#pragma HLS stream variable=img0.data dim=1 depth=1
#pragma HLS stream variable=img1.data dim=1 depth=1
#pragma HLS stream variable=img2.data dim=1 depth=1
#pragma HLS stream variable=img3.data dim=1 depth=1
#pragma HLS stream variable=img4.data dim=1 depth=1
#pragma HLS stream variable=imgOutputput.data dim=1 depth=1
	#pragma HLS dataflow


	xf::AXIvideo2xfMat(_src, imgInput);
	//RGB to GRAY conversion, to obtain the grayscale image
	xf::rgb2gray<XF_8UC3, XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput, img0);


//	xf::Threshold<XF_THRESHOLD_TYPE_BINARY, XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS>(img0, img1, 118, 255);
//	xf::bitwise_not<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS>(img1, img2);
//	xf::erode<XF_BORDER_CONSTANT, XF_8UC1 , HEIGHT, WIDTH, XF_SHAPE_ELLIPSE,
//		ERODE_FILTER_SIZE, ERODE_FILTER_SIZE, ERODE_ITERATIONS, NPIX_BLOBS>(img2, img3, erode_kernel);
//	xf::dilate<XF_BORDER_CONSTANT, XF_8UC1 , HEIGHT, WIDTH, XF_SHAPE_ELLIPSE,
//			DILATE_FILTER_SIZE, DILATE_FILTER_SIZE, DILATE_ITERATIONS, NPIX_BLOBS>(img3, img4, dilation_kernel);



	blob_detection(img0, imgOutput);



	for (int i = 0; i < blobsCounter; i++){
		if (blobs[i].valid){
			int areaCount = 0;
			for (int row = blobs[i].minRow; row <= blobs[i].maxRow; row++)
				for (int col = blobs[i].minCol; col <= blobs[i].maxCol; col++)
					if (img0.data[row*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+col] > 200)
						areaCount++;
			std::cout << i << " --> minRow: " << blobs[i].minRow <<
					"  maxRow: " << blobs[i].maxRow <<
					"  minCol: " << blobs[i].minCol <<
					"  maxCol: " << blobs[i].maxCol << std::endl;


			std::cout << "areaCount --> " << areaCount << std::endl;
			std::cout << "area      --> " << blobs[i].area << std::endl << std::endl;


			for (int j = blobs[i].minRow; j <= blobs[i].maxRow; j++){
				imgInput.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+(blobs[i].minCol)] = 0x00FF00;
				imgInput.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+(blobs[i].maxCol)] = 0x00FF00;
			}
			for (int j = blobs[i].minCol; j <= blobs[i].maxCol; j++){
				imgInput.data[(blobs[i].minRow)*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+j] = 0x00FF00;
				imgInput.data[(blobs[i].maxRow)*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+j] = 0x00FF00;
			}

		}
	}



	//xf::gray2rgb<XF_8UC1, XF_8UC3, HEIGHT, WIDTH, XF_NPPC1>(img0, imgOutput);
	xf::xfMat2AXIvideo(imgInput, _dst);


}
