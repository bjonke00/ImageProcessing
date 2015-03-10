// ========================================================
// unordered_dither.cpp - dithering, halftoning, and neighborhood operations program.
//
// Written by: Christyn Vasquez
// ========================================================

#include "IP.h"

using namespace std;

void unordered_dither(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine for quantization of image.
//
int
main(int argc, char** argv)
{
	int 	n, gamma;
	imageP 	inImage, outImage;
	
	// Checks image version
	if (argc != 5) {
        cerr << "Usage: unordered_dither imageP int int imageP\n";
        exit(1);
    }
	
	// Read input image and reserve space for output
	inImage = IP_readImage(argv[1]);
	outImage = NEWIMAGE;

	// Read quantization levels
	n = atoi(argv[2]);

	// Read gamma number
	gamma = atoi(argv[3]);
	
	// Quantization image and save result in file
	unordered_dither(inImage, n, gamma, outImage);
	IP_saveImage(outImage, argv[4]);

	// Free up image structures/memory
	IP_freeImage(inImage);
	IP_freeImage(outImage);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// unordered_dither:
//
// 
//
void
unordered_dither(imageP inImage, int n, int gamma, imageP outImage){

	int		i, j, total, R;
	int 	jitter, bias, scale;
	double	base, sum, Hsum, bin[MXGRAY];
	int 	left[MXGRAY], width[MXGRAY];
	uchar	*in, *out, lut[MXGRAY], H[MXGRAY];

	scale = MXGRAY / n;
	bias = scale/2;

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
	
	in  = inImage->image;	// input  image buffer
	out = outImage->image;	// output image buffer
	
	/*
	for(i=0; i<total; i++){
		in[i] = pow(in[i],1/gamma);
	}
	*/
	for(i=0; i<total; i++){
		jitter = ((rand()&0x7fff)*bias)>>15;
		if(i%2 == 0)	in[i] += jitter;
		else			in[i] -= jitter;
	}

	// Init lookup table
	for(j=0; j<n; j++){
		for(i=0; i<scale; i++) 
			lut[j*scale+i] = scale*j + scale/2;
	}

	// Visit all input pixels and apply lut to threshold
	for(i=0; i<total; i++) out[i] = lut[in[i]];	
}

