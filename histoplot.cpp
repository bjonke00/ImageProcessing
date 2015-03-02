#include "IP.h"

using namespace std; 

void histo_plot(imageP,imageP,int);

int
main(int argc, char** argv)
{
	imageP	Iin, Ihist;
	
	Iin = IP_readImage(argv[1]);
	Ihist = NEWIMAGE;
	int flag = argv[3];
	
	histo_plot(Iin,imageP,flag);
	
	#clear memory
	IP_freeImage(Iin);
	IP_freeImage(Ihist);
}

void histo_plot(imageP im, imageP histogram, int flag){
	int width, height, total;

	width = height = 256;

	total = im->width * im->height;
	int frequency[256];

	for(int i = 0; i < 256; i++){
		frequency[i] = 0;
	}

	for(int i=0; i < total; i++){
		frequency[im->image[i]]++;
	}


	for(int i = 0; i < width; i++){
		int h = 255;
		while(255-h < frequency[i]){
			//yada
			h--;
		}		
	}
}
