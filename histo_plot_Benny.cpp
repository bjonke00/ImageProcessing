// ========================================================
// histo_plot.cpp - Histogram program.
//
// Written by: Christyn Vasquez
// ========================================================

#include "IP.h"

using namespace std;

void histo_plot(imageP, imageP, int);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine for quantization of image.
//
int
main(int argc, char** argv)
{
	int 	flg;
	imageP 	inImage, outImage;
	
	// Checks image version
	if (argc != 4) {
        cerr << "Usage: histo_plot imageP imageP bool\n";
        exit(1);
    }
	
	// Read input image and reserve space for output
	inImage = IP_readImage(argv[1]);
	outImage = NEWIMAGE;

	//Read flag
	flg = atoi(argv[3]);
	
	// Quantization image and save result in file
	histo_plot(inImage, outImage, flg);
	IP_saveImage(outImage, argv[2]);

	// Free up image structures/memory
	IP_freeImage(inImage);
	IP_freeImage(outImage);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_plot:
//
// Evaluates the histogram of input image and plots it into an
// output image. Black background, white foreground.
// X - axis : intensity				Y- axis : frequency
//
void
histo_plot(imageP inImage, imageP outImage, int flg)
{
	int i, total;
	int x, y, v;
	uchar *in, *out, H[MXGRAY];
	
	// Init number of pixels in image
	total = inImage->width * inImage->height;
	
	// Init out image dimensions
	outImage->width  = 256;
	outImage->height = 256;
	outImage->image  = (uchar *) malloc(total);
	if(outImage->image == NULL) {
		cerr << "qntz: Insufficient memory\n";
		exit(1);
	}
	
	in = inImage->image;		// Input image buffer
	out = outImage->image;		// Output image buffer
	
	// Create an empty histogram array
	for(i = 0; i < MXGRAY; i++) H[i] = 0;
	
	// Fill array with values in image
	for(i = 0; i < total; i++) {
		H[in[i]]++;
	}
	
	if (flg == 0){
	int scale = 128 / (inImage->width * inImage->height / MXGRAY);
	for (i = 0; i <256; i++){
		H[i] = H[i]*scale;
		if(H[i] > 255) H[i] = 0;
	}}

	// Plot histogram in out file
	for(x = 0; x < (256); x++) {
		for(y=1; y <= H[x]; y++) {
			v = (256)*(256-y)+x;
			out[v] = MaxGray;
		}
	}
}
