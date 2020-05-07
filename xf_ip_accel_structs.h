#include "xf_ip_accel_types.h"

struct Blob{
	AREA_TYPE area;
	//unsigned int perimeter;
	bool valid = false;

	COORDINATE_TYPE minRow, minCol, maxRow, maxCol;

	BLOB_REFERENCE_TYPE newBlobReference;
};

struct Run{
	COORDINATE_TYPE start, end;
	BLOB_REFERENCE_TYPE blobReference;
};
