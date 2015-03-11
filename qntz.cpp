// ========================================================
// qntz.cpp - Histogram program.
//
// Written by: Christyn Vasquez
// ========================================================

#include "IP.h"

using namespace std;

void qntz(imageP, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine for quantization of image.
//
int
main(int argc, char** argv)
{
	int 	n;
	imageP 	inImage, outImage;
	
	// Checks image version
	if (argc != 4) {
        cerr << "Usage: qntz infile n outfile\n";
        exit(1);
    }
	
	// Read input image and reserve space for output
	inImage = IP_readImage(argv[1]);
	outImage = NEWIMAGE;

	// Read quantization number
	n  = atoi(argv[2]);

	// Quantization image and save result in file
	qntz(inImage, n, outImage);
	IP_saveImage(outImage, argv[3]);

	// Free up image structures/memory
	IP_freeImage(inImage);
	IP_freeImage(outImage);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// qntz:
//
// Quantization of input image (inImage) into n levels.
// The output image (outImage) is stored. Subdivide the
// range intensities into n uniform intervals. Assign
// midpoints of the intervals as the output intensities.
//
void
qntz(imageP inImage, int n, imageP outImage)
{
	int	 	i, total;
	int 	scale = MXGRAY / n;
	uchar	*in, *out, lut[256];

	// Total number of pixels in image
	total = inImage->width * inImage->height;

	// Init out image dimensions and buffer
	outImage->width  = inImage->width;
	outImage->height = inImage->height;
	outImage->image  = (uchar *) malloc(total);
	if(outImage->image == NULL) {
		cerr << "qntz: Insufficient memory\n";
		exit(1);
	}

	// Init lookup table
	for(j=0; j<n; j++){
		for(i=0; i<scale; i++) 
			lut[j*scale+i] = scale*j + scale/2;
	}

	// Visit all input pixels and apply lut to threshold
	in  = inImage->image;	// input  image buffer
	out = outImage->image;	// output image buffer
	for(i=0; i<total; i++) out[i] = lut[in[i]];
}