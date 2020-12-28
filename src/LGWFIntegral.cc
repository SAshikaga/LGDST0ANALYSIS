#include "LGDST0ANA.h"
#include<vector>

void LGDST0ANA::LGWFIntegral(double *dat, int peakx, double baseline, double* integral){

	int integ_startx = peakx - 50;
	int integ_endx = peakx + 100;
	double integ_val=0;
    	for(int i=integ_startx; i<integ_endx; i++){
		if(i>(Nsample-1)){
			//cout<<integ_val/50.<<": exit"<<endl;
			break;
		}
		if(i<0){
			//cout<<integ_val/50.<<endl;
			continue;
		}
        	integ_val+=dat[i]-baseline;
    	}
	*integral = integ_val/50.;
}
