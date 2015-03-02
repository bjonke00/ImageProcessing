// ========================================================
// histo_stretch.cpp - Histogram program.
//
// Written by: Christyn Vasquez
// ========================================================

#include "IP.h"

using namespace std;

void histo_stretch(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine for quantization of image.
//
int
main(int argc, char** argv)
{
	int 	t1, t2;
	imageP 	inImage, outImage;
	
	// Checks image version
	if (argc != 5) {
        cerr << "Usage: histo_stretch infile t1 t2 outfile\n";
        exit(1);
    }
	
	// Read input image and reserve space for output
	inImage = IP_readImage(argv[1]);
	outImage = NEWIMAGE;

	// Read spans t1 and t2 number
	t1  = atoi(argv[2]);
	t2  = atoi(argv[3]);

	// Quantization image and save result in file
	histo_stretch(inImage, t1, t2, outImage);
	IP_saveImage(outImage, argv[4]);

	// Free up image structures/memory
	IP_freeImage(inImage);
	IP_freeImage(outImage);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_stretch:
//
// Stretches the dynamic range of the input image to fill 
// [0, 255] range. The range that is stretched spas from t1 
// to t2. 
// 		if ( t1 == neg)
//			hist[i] (nonzero) is used to delimit the start of the span.
// All intensity values below t1 or above t2 are pulled to
// 0 to 255, respectively. The output is stored in the output
// file. 
//
void 
histo_stretch(imageP inImage, int t1, int t2, imageP outImage)
{
	int i, j, total;
	int x, y, v;
	uchar *in, *out, H[MXGRAY];
	
	// Init number of pixels in image
	total = inImage->width * inImage->height;
	
	// Init out image dimensions
	outImage->width  = 256;
	outImage->height = 256;
	outImage->image  = (uchar *) malloc(total);
	if(outImage->image == NULL) {
		cerr << "histo_stretch: Insufficient memory\n";
		exit(1);
	}
	
	in = inImage->image;		// Input image buffer
	out = outImage->image;		// Output image buffer
	
	// Create an empty histogram array
	for(i = 0; i < MXGRAY; i++) H[i] = 0;
	
	// Fill array with values in image
	for(i = 0; i < total; i++) H[in[i]]++;
	
	// Search for first instance on non-zero value
	x = 0;
	if(t1 < 0 && t2 < 0){
		for(i = 0; i < MXGRAY; i++){
			if((int)H[i] != 0){
				for(j = x; j <= H[i]; j++){
					H[j]++;
				}
				H[i]--;
				x++;
			}
		}
	}
	
	x = 0;
	//Start stretching from t1 value
	if(t1 > 0 && t1 < 255){
		for(i = t1+1; i < MXGRAY; i++){
			H[i] = 0;
		}
		for(i = 0; i < MXGRAY; i++){
			if((int)H[i] != 0){
				for(j = x; j <= H[i]; j++){
					H[j]++;
				}
				H[i]--;
				x++;
			}
		}
	}
	
	x = 0;
	//Start stretching from t2 value
	if(t2 > 0 && t2 < 255){
		for(i = 0; i < t2; i++){
			H[i] = 0;
		}
		for(i = 0; i < MXGRAY; i++){
			if((int)H[i] != 0){
				for(j = x; j <= H[i]; j++){
					H[j]++;
				}
				H[i]--;
				x++;
			}
		}
	}
	
	// Plot histogram in out file
	for(x = 0; x < (256); x++) {
		for(y=1; y <= H[x]; y++) {
			v = (256)*(256-y)+x;
			out[v] = MaxGray;
		}
	}	
}