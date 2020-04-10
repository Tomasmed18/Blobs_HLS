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

struct possibleBlob{
	//unsigned int color;
	unsigned int area;
	unsigned int perimeter;
	bool valid;

	unsigned int minRow, minCol, maxRow, maxCol;
};

struct run{
	unsigned short start, end;
	unsigned short associatedPossibleBlob;
};

possibleBlob possibleBlobs[1000];
unsigned short possibleBlobsCounter = 0;

run runArray1[100];
run runArray2[100];



void verify_overlap_1(unsigned short i, run & run, unsigned int row, unsigned int lastAssociatedBlob = 255){
	bool overlapDetected = false; //to control if its the first overlap detected or not
	for(unsigned short c = 0; c < i; c++){
		if (
			((runArray1[c].start <= run.start) && (runArray1[c].end >= run.start)) ||
			((runArray1[c].start <= run.end) && (runArray1[c].end >= run.end)) ||
			((runArray1[c].start >= run.start) && (runArray1[c].end <= run.end)) )
			{

			if (!overlapDetected){ //if it is its first overlap...

				if (possibleBlobs[runArray1[c].associatedPossibleBlob].valid){
					run.associatedPossibleBlob = runArray1[c].associatedPossibleBlob; // associate with possible blob from previous line

					// TODO update possible blob features
					possibleBlobs[runArray1[c].associatedPossibleBlob].area++;

					// update blob position values
					possibleBlobs[runArray1[c].associatedPossibleBlob].maxRow = row;
					if (run.start < possibleBlobs[runArray1[c].associatedPossibleBlob].minCol)
						possibleBlobs[runArray1[c].associatedPossibleBlob].minCol = run.start;
					if (run.end > possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol){
						possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol = run.end;
					}


				}
				else{
					run.associatedPossibleBlob = lastAssociatedBlob;
					// TODO update possible blob features
					possibleBlobs[lastAssociatedBlob].area++;

					//TODO verify position updates
					possibleBlobs[lastAssociatedBlob].maxRow = row;
					if (run.start < possibleBlobs[lastAssociatedBlob].minCol)
						possibleBlobs[lastAssociatedBlob].minCol = run.start;
					if (run.end > possibleBlobs[lastAssociatedBlob].maxCol)
						possibleBlobs[lastAssociatedBlob].maxCol = run.end;
				}

				overlapDetected = true;
				std::cout << "OVERLAP!! with " << runArray1[c].start << "  " << runArray1[c].end << std::endl;

			}
			else { // if it is not the first overlapping segment

				if (run.associatedPossibleBlob != runArray1[c].associatedPossibleBlob){ // and this segment is not already associated with the same blob
					// TODO merge blob features in one
					possibleBlobs[run.associatedPossibleBlob].area += possibleBlobs[runArray1[c].associatedPossibleBlob].area;
					possibleBlobs[runArray1[c].associatedPossibleBlob].valid = false;

					// update blob position values
					if (run.end < possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol)
						possibleBlobs[run.associatedPossibleBlob].maxCol = possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol;
					if (possibleBlobs[runArray1[c].associatedPossibleBlob].minRow < possibleBlobs[run.associatedPossibleBlob].minRow)
						possibleBlobs[run.associatedPossibleBlob].minRow = possibleBlobs[runArray1[c].associatedPossibleBlob].minRow;
					if (possibleBlobs[runArray1[c].associatedPossibleBlob].minCol < possibleBlobs[run.associatedPossibleBlob].minCol) //TODO see if necessary
						possibleBlobs[run.associatedPossibleBlob].minCol = possibleBlobs[runArray1[c].associatedPossibleBlob].minCol;
					if (possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol > possibleBlobs[run.associatedPossibleBlob].maxCol) //TODO see if necessary
						possibleBlobs[run.associatedPossibleBlob].maxCol = possibleBlobs[runArray1[c].associatedPossibleBlob].maxCol;

					runArray1[c].associatedPossibleBlob = run.associatedPossibleBlob;

					std::cout << "AND with " << runArray1[c].start << "  " << runArray1[c].end << std::endl;
				}
			}


		}

	}

	if (!overlapDetected){ // if there was not another segment that overlaps with this one...

		// associate run with a new blob entry
		run.associatedPossibleBlob = possibleBlobsCounter;

		// set it as valid
		possibleBlobs[possibleBlobsCounter].valid = true;
		// set feature values
		possibleBlobs[possibleBlobsCounter].area = 1;
		possibleBlobs[possibleBlobsCounter].perimeter = 1;
		// set position values
		possibleBlobs[possibleBlobsCounter].minRow = row;
		possibleBlobs[possibleBlobsCounter].maxRow = row;
		possibleBlobs[possibleBlobsCounter].minCol = run.start;
		possibleBlobs[possibleBlobsCounter].maxCol = run.end;

		possibleBlobsCounter++;
	}

}

void verify_overlap_2(unsigned short i, run & run, unsigned int row, unsigned int lastAssociatedBlob = 255){
	bool overlapDetected = false; //to control if its the first overlap detected or not
	for(unsigned short c = 0; c < i; c++){
		if (
			((runArray2[c].start <= run.start) && (runArray2[c].end >= run.start)) ||
			((runArray2[c].start <= run.end) && (runArray2[c].end >= run.end)) ||
			((runArray2[c].start >= run.start) && (runArray2[c].end <= run.end)) )
			{

			if (!overlapDetected){ //if it is its first overlap...

				if (possibleBlobs[runArray2[c].associatedPossibleBlob].valid){
					run.associatedPossibleBlob = runArray2[c].associatedPossibleBlob; // associate with possible blob from previous line

					// TODO update possible blob features
					possibleBlobs[runArray2[c].associatedPossibleBlob].area++;

					// update blob position values
					possibleBlobs[runArray2[c].associatedPossibleBlob].maxRow = row;
					if (run.start < possibleBlobs[runArray2[c].associatedPossibleBlob].minCol)
						possibleBlobs[runArray2[c].associatedPossibleBlob].minCol = run.start;
					if (run.end > possibleBlobs[runArray2[c].associatedPossibleBlob].maxCol)
						possibleBlobs[runArray2[c].associatedPossibleBlob].maxCol = run.end;

				}
				else{
					run.associatedPossibleBlob = lastAssociatedBlob;
					// TODO update possible blob features
					possibleBlobs[lastAssociatedBlob].area++;

					//TODO verify position updates
					possibleBlobs[lastAssociatedBlob].maxRow = row;
					if (run.start < possibleBlobs[lastAssociatedBlob].minCol)
						possibleBlobs[lastAssociatedBlob].minCol = run.start;
					if (run.end > possibleBlobs[lastAssociatedBlob].maxCol)
						possibleBlobs[lastAssociatedBlob].maxCol = run.end;
				}


				overlapDetected = true;
				std::cout << "OVERLAP!! with " << runArray2[c].start << "  " << runArray2[c].end << std::endl;
			}
			else { // if it is not the first overlapping segment

				if (run.associatedPossibleBlob != runArray2[c].associatedPossibleBlob){ // and this segment is not already associated with the same blob
					// TODO merge blob features in one
					possibleBlobs[run.associatedPossibleBlob].area += possibleBlobs[runArray2[c].associatedPossibleBlob].area;

					possibleBlobs[runArray2[c].associatedPossibleBlob].valid = false;

					// update blob position values
					if (possibleBlobs[run.associatedPossibleBlob].maxCol < possibleBlobs[runArray2[c].associatedPossibleBlob].maxCol)
						possibleBlobs[run.associatedPossibleBlob].maxCol = possibleBlobs[runArray2[c].associatedPossibleBlob].maxCol;
					if (possibleBlobs[runArray2[c].associatedPossibleBlob].minRow < possibleBlobs[run.associatedPossibleBlob].minRow)
						possibleBlobs[run.associatedPossibleBlob].minRow = possibleBlobs[runArray2[c].associatedPossibleBlob].minRow;
					if (possibleBlobs[runArray2[c].associatedPossibleBlob].minCol < possibleBlobs[run.associatedPossibleBlob].minCol) //TODO see if necessary
						possibleBlobs[run.associatedPossibleBlob].minCol = possibleBlobs[runArray2[c].associatedPossibleBlob].minCol;

					runArray2[c].associatedPossibleBlob = run.associatedPossibleBlob;

					std::cout << "AND with " << runArray2[c].start << "  " << runArray2[c].end << std::endl;
				}

			}

		}

	}

	if (!overlapDetected){ // if there was not another segment that overlaps with this one...

		// associate run with a new blob entry
		run.associatedPossibleBlob = possibleBlobsCounter;

		// set it as valid
		possibleBlobs[possibleBlobsCounter].valid = true;
		// set feature values
		possibleBlobs[possibleBlobsCounter].area = 1;
		possibleBlobs[possibleBlobsCounter].perimeter = 1;
		// set position values
		possibleBlobs[possibleBlobsCounter].minRow = row;
		possibleBlobs[possibleBlobsCounter].maxRow = row;
		possibleBlobs[possibleBlobsCounter].minCol = run.start;
		possibleBlobs[possibleBlobsCounter].maxCol = run.end;


		possibleBlobsCounter++;
	}

}


void blob_detection(xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> & src,
		xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> & dst){

	//initialize counters
	unsigned short i1 = 0;
	unsigned short i2 = 0;

	bool readingRun = false;

	for(short int j = 0; j < (HEIGHT); j++ ){
		unsigned int lastAssociatedBlob = 255;
		i1 = 0;
		for(short int i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
			//get the pixel brightness value
			unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];

			if (!readingRun && pix == 255){ //start of run detected
				readingRun = true;
				runArray1[i1].start = i;
			}else if(readingRun && pix == 0){ //end of run detected
				readingRun = false;
				runArray1[i1].end = i - 1;

				std::cout << "start: " << runArray1[i1].start << "  end: " << runArray1[i1].end << std::endl;

				verify_overlap_2(i2, runArray1[i1], j, lastAssociatedBlob);

				lastAssociatedBlob = runArray1[i1].associatedPossibleBlob;
				i1++;
			}

		}
		if (readingRun){
			runArray1[i1].end = WIDTH -1;
			std::cout << "start: " << runArray1[i1].start << "  end: " << runArray1[i1].end << std::endl;
			verify_overlap_2(i2, runArray1[i1], j);
			readingRun = false;
			i1++;
		}

		std::cout << "i1: " << i1 << std::endl;

		j++;

		lastAssociatedBlob = 255;
		readingRun = false;
		i2 = 0;
		for(short int i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
					//get the pixel brightness value
					unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];

					if (!readingRun && pix == 255){ //start of run detected
						readingRun = true;
						runArray2[i2].start = i;
					}else if(readingRun && pix == 0){ //end of run detected
						readingRun = false;
						runArray2[i2].end = i - 1;

						std::cout << "start: " << runArray2[i2].start << "  end: " << runArray2[i2].end << std::endl;

						verify_overlap_1(i1, runArray2[i2], j, lastAssociatedBlob);

						lastAssociatedBlob = runArray2[i2].associatedPossibleBlob;
						i2++;
					}

				}
		if (readingRun){
			runArray2[i2].end = WIDTH -1;
			readingRun = false;
			std::cout << "start: " << runArray2[i2].start << "  end: " << runArray2[i2].end << std::endl;
			verify_overlap_1(i1, runArray2[i2], j);
			i2++;
		}
		std::cout << "i2: " << i2 << std::endl;

	}
	std::cout << "FINISHED!!" << std::endl;

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



	for (int i = 0; i < possibleBlobsCounter; i++){
		if (possibleBlobs[i].valid){
			std::cout << "minRow: " << possibleBlobs[i].minRow <<
					"  maxRow: " << possibleBlobs[i].maxRow <<
					"  minCol: " << possibleBlobs[i].minCol <<
					"  maxCol: " << possibleBlobs[i].maxCol << std::endl;
			for (int j = possibleBlobs[i].minRow; j <= possibleBlobs[i].maxRow; j++){
				imgInput.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+(possibleBlobs[i].minCol)] = 0xFF00FF;
				imgInput.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+(possibleBlobs[i].maxCol)] = 0xFF00FF;
			}
			for (int j = possibleBlobs[i].minCol; j <= possibleBlobs[i].maxCol; j++){
				imgInput.data[(possibleBlobs[i].minRow)*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+j] = 0xFF00FF;
				imgInput.data[(possibleBlobs[i].maxRow)*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+j] = 0xFF00FF;
			}

		}
	}



	//xf::gray2rgb<XF_8UC1, XF_8UC3, HEIGHT, WIDTH, XF_NPPC1>(img0, imgOutput);
	xf::xfMat2AXIvideo(imgInput, _dst);


}
