#include "xf_ip_accel_config.h"
#include "xf_ip_accel_types.h"
#include "xf_ip_accel_structs.h"


const ap_fixed<22, 2> circleAreaValues[ MAX_BLOB_RADIUS ] = { 0, //Added 0 to avoid looking for r-1 in the table
      0.3183098861837907 , 0.07957747154594767 , 0.0353677651315323 , 0.019894367886486918 , 0.012732395447351627 , 0.008841941282883075 , 0.00649612012619981 , 0.0049735919716217296 , 0.003929751681281367 , 0.0031830988618379067 ,0.0026306602163949644 , 0.002210485320720769 , 0.0018834904507916608 , 0.0016240300315499526 , 0.001414710605261292 , 0.0012433979929054324 , 0.0011014182912933933 , 0.0009824379203203417 , 0.0008817448370742125 , 0.0007957747154594767 ,      0.0007217911251333121 , 0.0006576650540987411 , 0.0006017200116895854 , 0.0005526213301801922 , 0.0005092958178940651 , 0.0004708726126979152 , 0.00043663907569792965 , 0.00040600750788748815 , 0.0003784897576501673 , 0.000353677651315323 , 0.00033122776918188417 , 0.0003108494982263581 , 0.00029229557959944047 , 0.00027535457282334833 , 0.00025984480504799237 , 0.0002456094800800854 , 0.0002325126999151137 , 0.00022043620926855313 , 0.000209276716754629 , 0.00019894367886486917 ,      0.0001893574575751283 , 0.00018044778128332802 ,
	  0.0001721524533173557 , 0.00016441626352468527 , 0.00015719006725125465 , 0.00015043000292239636 , 0.0001440968248908061 , 0.00013815533254504805 , 0.00013257388012652673 , 0.00012732395447351627 ,      0.00012237981014371038 , 0.0001177181531744788 , 0.00011331786620996464 , 0.00010915976892448241 , 0.00010522640865579856 , 0.00010150187697187204 , 9.797164856380138e-05 , 9.462243941254182e-05 , 9.144208163854946e-05 , 8.841941282883075e-05 ,      8.554417795855702e-05 , 8.280694229547104e-05 , 8.019901390370136e-05 , 7.771237455658952e-05 , 7.533961803166644e-05 , 7.307389489986012e-05 , 7.090886303938309e-05 , 6.883864320583708e-05 , 6.68577790766206e-05 , 6.496120126199809e-05 ,
      6.31441948390777e-05 , 6.140237002002136e-05 , 5.973163561339665e-05 , 5.8128174978778425e-05 , 5.658842421045167e-05 , 5.510905231713828e-05 , 5.3686943191733965e-05 , 5.231917918865725e-05 , 5.100302614705827e-05 , 4.973591971621729e-05 ,      4.851545285532552e-05 , 4.733936439378208e-05 , 4.620552855041235e-05 , 4.5111945320832005e-05 , 4.405673165173574e-05 , 4.3038113329338925e-05 , 4.2054417516685254e-05 , 4.110406588117132e-05 , 4.018556825953675e-05 , 3.929751681281366e-05 ,      3.843858062840124e-05 , 3.760750073059909e-05 , 3.68030854646538e-05 , 3.6024206222701526e-05 , 3.5269793482968496e-05 , 3.453883313626201e-05 , 3.383036307618139e-05 , 3.314347003163168e-05 , 3.247728662216005e-05 , 3.183098861837907e-05 ,      3.120379239131366e-05 , 3.0594952535927595e-05 , 3.0003759655367206e-05 , 2.94295382936197e-05 , 2.887164500533249e-05 , 2.832946655249116e-05 , 2.7802418218516085e-05 , 2.7289942231120603e-05 , 2.6791506285985244e-05 , 2.630660216394964e-05 ,
      2.5834744435012637e-05 , 2.537546924296801e-05 , 2.4928333164992613e-05 , 2.4492912140950345e-05 , 2.4068800467583413e-05 , 2.3655609853135456e-05 , 2.325296852829211e-05 , 2.2860520409637365e-05 , 2.247792431211007e-05 , 2.210485320720769e-05 ,      2.1740993523925326e-05 , 2.1386044489639256e-05 , 2.103971750834759e-05 , 2.070173557386776e-05 , 2.0371832715762605e-05 , 2.004975347592534e-05 , 1.9735252413899847e-05 , 1.942809363914738e-05 , 1.9128050368595078e-05 , 1.883490450791661e-05 ,      1.854844625510114e-05 , 1.826847372496503e-05 , 1.7994792593351273e-05 , 1.7727215759845772e-05 , 1.7465563027917186e-05 , 1.720966080145927e-05 , 1.695934179678143e-05 , 1.671444476915515e-05 , 1.6474814253081656e-05 , 1.6240300315499523e-05 ,      1.601075832120068e-05 , 1.5786048709769424e-05 , 1.5566036783402157e-05 , 1.535059250500534e-05 , 1.513959030600669e-05 , 1.4932908903349162e-05 , 1.4730431125169637e-05 , 1.4532043744694606e-05 , 1.4337637321913009e-05 , 1.4147106052612918e-05 ,
      1.396034762439326e-05 , 1.377726307928457e-05 , 1.3597756682634486e-05 , 1.3421735797933491e-05 , 1.3249110767275367e-05 , 1.3079794797164313e-05 , 1.2913703849397165e-05 , 1.2750756536764568e-05 , 1.2590874023329406e-05 , 1.2433979929054323e-05 ,      1.2280000238562966e-05 , 1.212886321383138e-05 , 1.1980499310617287e-05 , 1.183484109844552e-05 , 1.169182318397762e-05 , 1.1551382137603087e-05 , 1.1413456423098378e-05 , 1.1277986330208001e-05 , 1.1144913910009828e-05 , 1.1014182912933934e-05 ,      1.0885738729311265e-05 , 1.0759528332334731e-05 , 1.063550022332155e-05 , 1.0513604379171314e-05 , 1.0393792201919697e-05 , 1.027601647029283e-05 , 1.0160231293172162e-05 , 1.0046392064884188e-05 , 9.934455422233722e-06 , 9.824379203203415e-06 ,      9.716122407246136e-06 , 9.60964515710031e-06 , 9.504908662061892e-06 , 9.401875182649772e-06 , 9.300507996604549e-06 , 9.20077136616345e-06 , 9.102630506556968e-06 , 9.006051555675381e-06 , 8.911001544855706e-06 , 8.817448370742124e-06 };



unsigned char erode_kernel[ERODE_FILTER_SIZE*ERODE_FILTER_SIZE] ={0,0,0,1,0,0,0,
												0,0,0,1,0,0,0,
												0,0,1,1,1,0,0,//	0,0,0,1,0,0,0,
												1,1,1,1,1,1,1,
												0,0,1,1,1,0,0, //0,0,0,1,0,0,0,
												0,0,0,1,0,0,0,
												0,0,0,1,0,0,0};


unsigned char dilation_kernel[DILATE_FILTER_SIZE*DILATE_FILTER_SIZE] =   {0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,1,1,1,0,0,0,0, //0,0,0,0,0,1,0,0,0,0,0,
																			1,1,1,1,1,1,1,1,1,1,1,
																			0,0,0,0,1,1,1,0,0,0,0, //0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0,
																			0,0,0,0,0,1,0,0,0,0,0};





void verify_overlap(BLOB_REFERENCE_TYPE i, Run (&runArray)[MAX_RUNS], Run & run, COORDINATE_TYPE row, Blob (&blobs)[MAX_BLOBS], BLOB_REFERENCE_TYPE & blobsCounter){

	bool overlapDetected = false; //to control if its the first overlap detected of this run or not

	for(BLOB_REFERENCE_TYPE c = 0; c < i; c++){
		if (
			((runArray[c].start <= run.start) && (runArray[c].end >= run.start)) || //conditions to verify overlapping between 2 segments
			((runArray[c].start <= run.end) && (runArray[c].end >= run.end)) ||
			((runArray[c].start >= run.start) && (runArray[c].end <= run.end)) )
			{

			if (!overlapDetected){ //if it is its first overlap...

				if (blobs[runArray[c].blobReference].valid){
					run.blobReference = runArray[c].blobReference; // associate with possible blob from previous line

					// TODO update possible blob features
					blobs[run.blobReference].area += run.end - run.start + 1;

					// update blob position values
					blobs[runArray[c].blobReference].maxRow = row;
					if (run.start < blobs[runArray[c].blobReference].minCol)
						blobs[runArray[c].blobReference].minCol = run.start;
					if (run.end > blobs[runArray[c].blobReference].maxCol){
						blobs[runArray[c].blobReference].maxCol = run.end;
					}


				}
				else{ // special case, where the segment that overlaps with this run has been re-associated with a different blob
					run.blobReference = blobs[runArray[c].blobReference].newBlobReference;
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

				if ((run.blobReference != runArray[c].blobReference) // and this segment is not already associated with the same blob
						&& (blobs[runArray[c].blobReference].valid)){
					// merge blob features in one
					blobs[run.blobReference].area += blobs[runArray[c].blobReference].area;
					blobs[runArray[c].blobReference].newBlobReference = run.blobReference;
					blobs[runArray[c].blobReference].valid = false;

					// update blob position values
					if (run.end < blobs[runArray[c].blobReference].maxCol)
						blobs[run.blobReference].maxCol = blobs[runArray[c].blobReference].maxCol;
					if (blobs[runArray[c].blobReference].minRow < blobs[run.blobReference].minRow)
						blobs[run.blobReference].minRow = blobs[runArray[c].blobReference].minRow;
					if (blobs[runArray[c].blobReference].minCol < blobs[run.blobReference].minCol) //TODO see if necessary
						blobs[run.blobReference].minCol = blobs[runArray[c].blobReference].minCol;
					if (blobs[runArray[c].blobReference].maxCol > blobs[run.blobReference].maxCol) //TODO see if necessary
						blobs[run.blobReference].maxCol = blobs[runArray[c].blobReference].maxCol;

					runArray[c].blobReference = run.blobReference;

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


void blob_detection(xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> & src, Blob (&blobs)[MAX_BLOBS], BLOB_REFERENCE_TYPE & blobsCounter){

	//initialize counters for Run arrays
	RUN_REFERENCE_TYPE i1 = 0;
	RUN_REFERENCE_TYPE i2 = 0;
	static Run runArray1[MAX_RUNS];
	static Run runArray2[MAX_RUNS];

	bool readingRun = false; // state bit

	for(COORDINATE_TYPE j = 0; j < (HEIGHT); j++ ){
		i1 = 0;
		for(COORDINATE_TYPE i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
			//get the pixel brightness value
			unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];

			if (!readingRun && pix == 255){ //start of run detected

				readingRun = true;
				runArray1[i1].start = i;

			}else if(readingRun && pix == 0){ //end of run detected

				readingRun = false;
				runArray1[i1].end = i - 1;
				verify_overlap(i2, runArray2, runArray1[i1], j, blobs, blobsCounter);
				i1++;
			}

		}
		if (readingRun){ // if it was reading a run and it finished at the end of the row
			runArray1[i1].end = WIDTH -1;
			verify_overlap(i2, runArray2, runArray1[i1], j, blobs, blobsCounter);
			readingRun = false;
			i1++;
		}

		j++;
		// repeated code...
		readingRun = false;
		i2 = 0;
		for(COORDINATE_TYPE i = 0; i < ((WIDTH>>XF_BITSHIFT(XF_NPPC1))); i++ ){
					//get the pixel brightness value
					unsigned char pix = src.data[j*(WIDTH>>XF_BITSHIFT(XF_NPPC1))+i];

					if (!readingRun && pix == 255){ //start of run detected

						readingRun = true;
						runArray2[i2].start = i;

					}else if(readingRun && pix == 0){ //end of run detected

						readingRun = false;
						runArray2[i2].end = i - 1;
						verify_overlap(i1, runArray1, runArray2[i2], j, blobs, blobsCounter);
						i2++;
					}

				}
		if (readingRun){ // if it was reading a run and it finished at the end of the row
			runArray2[i2].end = WIDTH -1;
			readingRun = false;
			verify_overlap(i1, runArray1, runArray2[i2], j, blobs, blobsCounter);
			i2++;
		}

	}

}


float getCircleAreaValue(unsigned int r){
	if (r < MAX_BLOB_RADIUS)
		return circleAreaValues[r]; // 1 / (PI * r^2)
	else
		return 0;
}

void blob_classification(Blob (&blobs)[MAX_BLOBS], BLOB_REFERENCE_TYPE blobsCounter, bool & error){
	// TODO: add Loop-Unrolling
	for (BLOB_REFERENCE_TYPE i = 0; i < blobsCounter; i++){
		Blob b = blobs[i];
		if (b.valid){
			if (b.area > MIN_BLOB_AREA){

				COORDINATE_TYPE blobHeight = (b.maxCol - b.minCol);
				COORDINATE_TYPE blobWidth = (b.maxRow - b.minRow);

				COORDINATE_TYPE blobRadius = blobHeight > blobWidth ? blobHeight/2 : blobWidth/2; //set radius to the highest length divided by 2

				ap_fixed<20, 2> blobInverseRoundness = b.area * getCircleAreaValue(blobRadius);

				//std::cout << i << " = " << blobInverseRoundness << std::endl;

				if (blobInverseRoundness < MIN_BLOB_ROUNDNESS){
					error = true;
					//blobs[i].valid = false; DELETED TO AVOID WRITING IN BLOBS[i].VALID
				}
			} //else
				//blobs[i].valid = false; DELETED TO AVOID WRITING IN BLOBS[i].VALID
		}
	}
}

void blob_preprocessing(hls::stream< ap_axiu<24,1,1,1> >& _src, xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> & imgOut, Blob (&blobs)[MAX_BLOBS], BLOB_REFERENCE_TYPE & blobsCounter){
	xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> imgInput(HEIGHT, WIDTH); //RGB
	xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img0(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img1(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img2(HEIGHT, WIDTH); //GRAY
	 xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> img3(HEIGHT, WIDTH); //GRAY

#pragma HLS stream variable=imgInput.data dim=1 depth=1
#pragma HLS stream variable=img0.data dim=1 depth=1
#pragma HLS stream variable=img1.data dim=1 depth=1
#pragma HLS stream variable=img2.data dim=1 depth=1
#pragma HLS stream variable=img3.data dim=1 depth=1
	#pragma HLS DATAFLOW

		xf::AXIvideo2xfMat(_src, imgInput);
	 	xf::rgb2gray<XF_8UC3, XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput, img0);

	 	xf::Threshold<XF_THRESHOLD_TYPE_BINARY, XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS>(img0, img1, 118, 255);
	 	xf::bitwise_not<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS>(img1, img2);
	 	xf::erode<XF_BORDER_CONSTANT, XF_8UC1 , HEIGHT, WIDTH, XF_SHAPE_ELLIPSE,
	 		ERODE_FILTER_SIZE, ERODE_FILTER_SIZE, ERODE_ITERATIONS, NPIX_BLOBS>(img2, img3, erode_kernel);
	 	xf::dilate<XF_BORDER_CONSTANT, XF_8UC1 , HEIGHT, WIDTH, XF_SHAPE_ELLIPSE,
	 			DILATE_FILTER_SIZE, DILATE_FILTER_SIZE, DILATE_ITERATIONS, NPIX_BLOBS>(img3, imgOut, dilation_kernel);

}


void blobs_accel(hls::stream< ap_axiu<24,1,1,1> >& _src,hls::stream< ap_axiu<24,1,1,1> >& _dst, bool & errorDetected)
{
#pragma HLS INTERFACE axis register both  port=_src
#pragma HLS INTERFACE axis register both  port=_dst

	xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> imgPreprocessed(HEIGHT, WIDTH); //GRAY
	xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> imgPreprocessed_a(HEIGHT, WIDTH); //GRAY
	xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS> imgPreprocessed_b(HEIGHT, WIDTH); //GRAY
	xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPIX_BLOBS> imgOutput(HEIGHT, WIDTH); //RGB


	 Blob blobs[MAX_BLOBS];
	 BLOB_REFERENCE_TYPE blobsCounter = 0;
	 bool error = false;

#pragma HLS stream variable=imgPreprocessed.data dim=1 depth=1
#pragma HLS stream variable=imgPreprocessed_a.data dim=1 depth=1
#pragma HLS stream variable=imgPreprocessed_b.data dim=1 depth=1
#pragma HLS stream variable=imgOutputput.data dim=1 depth=1
#pragma HLS stream variable=blobs dim=1 depth=1

#pragma HLS DATAFLOW

	/* IMAGE PREPROCESSING */
	blob_preprocessing(_src, imgPreprocessed, blobs, blobsCounter);
 	xf::duplicateMat<XF_8UC1, HEIGHT, WIDTH, NPIX_BLOBS>(imgPreprocessed, imgPreprocessed_a, imgPreprocessed_b);

	/* IMAGE PROCESSING */
	blob_detection(imgPreprocessed_a, blobs, blobsCounter);

	/* BLOB CLASSIFICATION */
	blob_classification(blobs, blobsCounter, error);

	/* GENERATE OUTPUT IMAGE */
	xf::gray2rgb<XF_8UC1, XF_8UC3, HEIGHT, WIDTH, XF_NPPC1>(imgPreprocessed_b, imgOutput);
	xf::xfMat2AXIvideo(imgOutput, _dst);

	errorDetected = error;
}
