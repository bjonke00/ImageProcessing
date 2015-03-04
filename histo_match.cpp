// ========================================================
// histo_match.cpp - Histogram program.
//
// Written by: Christyn Vasquez
// ========================================================

#include "IP.h"

using namespace std;

void histo_match(imageP, int, imageP);

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
        cerr << "Usage: histo_stretch infile t1 t2 outfile\n";
        exit(1);
    }
	
	// Read input image and reserve space for output
	inImage = IP_readImage(argv[1]);
	outImage = NEWIMAGE;

	// Read n number
	n  = atoi(argv[2]);

	// Quantization image and save result in file
	histo_match(inImage, n, outImage);
	IP_saveImage(outImage, argv[3]);

	// Free up image structures/memory
	IP_freeImage(inImage);
	IP_freeImage(outImage);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_match:
//
// Histogram used is determined by number
// 		0 for flat histo
// 		+ for exponential histo
// 
//
void 
histo_match(imageP inImage, int n, imageP outImage)
{
	int i, R;
	int left[MXGRAY], width[MXGRAY];
	uchar *in, *out;
	long total, Hsum, Havg, H[MXGRAY];
	double bin[MXGRAY];

	// Total number of pixels in image
	total = (long)inImage->height * inImage->width;
	
	// Interval value for equal histogram
	Havg = total / MXGRAY;

//
	if(n >= 0){
		// Get Value of each bin.
		for (int i = 0 ; i < MXGRAY; i++){
		    double base = (double) (i/(double) MXGRAY);
		    bin[i] = pow(base,n);
		}
		// Find the total_sum of histogram entries.
		double total_sum = 0;
		for (int i = 0; i < MXGRAY; i++){
			total_sum += bin[i];
		}
		//divide each bin by total_sum.
		for (int i = 0; i < MXGRAY; i++){
			bin[i] = round(total * (bin[i]/total_sum));
		}
	}
	else{
		for (int i = 0 ; i < MXGRAY; i++){
		    double base = (double) (i/(double) MXGRAY);
		    bin[i] =  pow(base,-n);
		}
		// Find the total_sum of histogram entries.
		double total_sum = 0;
		for (int i = 0; i < MXGRAY; i++){
			total_sum += bin[i];
		}
		//divide each bin by total_sum.
		for (int i = 0; i < MXGRAY; i++){
			bin[i] = round(total * ((1 - bin[i])/total_sum));
		}

	}	


	// Init out image dimensions and buffer
	outImage->width  = inImage->width;
	outImage->height = inImage->height;
	outImage->image  = (uchar *) malloc(total);

	in  = inImage->image;		// input image  buffer
	out = outImage->image;		// output image buffer

	for(i = 0; i < MXGRAY; i++) H[i] = 0;	// clear histogram
	for(i = 0; i < total; i++)  H[in[i]]++;	// eval  histogram
		
	R    = 0;		// right end of interval
	Hsum = 0;		// cumulative value for interval

	// evaluate remapping of all input gray levels;
	// Each input gray value maps to an interval of valid
	// output values. The endpoints of the intervals are
	// left[] and left[]+width[].


if (n == 0){
	for(i=0; i<MXGRAY; i++) {
		left[i] = R;		// left end of interval
		Hsum += H[i];	// cum. interval value

		// make interval wider, if necessary
		while(Hsum>Havg && R<MXGRAY-1){
			Hsum -= Havg;	// adjust Hsum
			R++;		// update right end
		}

		// width of interval
		width[i] = R - left[i] + 1;
	}
}else{
	for(i=0; i<MXGRAY; i++) {
		left[i] = R;		// left end of interval
		Hsum += H[i];	// cum. interval value

		// make interval wider, if necessary
		while(Hsum>bin[R] && R<MXGRAY-1){
			Hsum -= bin[R];	// adjust Hsum
			R++;		// update right end
		}

		// width of interval
		width[i] = R - left[i] + 1;
	}
}
	// visit all input pixels and remap intensities
	for(i=0; i<total; i++) {
		if(width[in[i]] == 1) out[i] = left[in[i]];
		else {	// in[i] spills over into width[] possible values
			// randomly pick R from 0 to width[in[i]]
			R = ((rand()&0x7fff)*width[in[i]])>>15;

			// add R to left end of interval
			out[i] = left[in[i]] + R;
		}
	}
}
