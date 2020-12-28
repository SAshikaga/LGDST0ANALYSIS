#include "LGDST0ANA.h"
#include <vector>
#include <iostream>

//const int start_x = 110;
//const int end_x = 155;

void LGDST0ANA::LGWFPeak(double *dat, double* peak, int* peakx, int *start_x, int *end_x){
	if(*start_x<0||*start_x>Nsample){
		 *peak = 0;
        	 *peakx = -1;
	}
	double peak_can=-100000;
	int peakx_can=0;
	
	for(int i=*start_x; i<*end_x; i++){

	if(*end_x>Nsample){
		break;
	}
	if(dat[i]>peak_can){
            	peak_can=dat[i];
            	peakx_can=i;
        	}
    	}
	//std::cout<<"peak   "<<peak_can<<" x  "<<peakx_can<<std::endl;
	*peak = peak_can;
	*peakx = peakx_can;

}
