#include "LGDST0ANA.h"
#include <TH1.h>
#include <vector>
#include <iostream>
#include <algorithm>

void LGDST0ANA::LGWFBaseline(double *dat, int peakx, double* baseline, double* rms){

	double baseline_can;
	double base_sum=0;
	int count=0;
	double h_mean=0;
	double h_rms=0;	

	auto max = *max_element(dat,dat+Nsample);
	auto min = *min_element(dat,dat+Nsample);
	//cout<<"max:"<<max<<endl;
	int nbin = (max-min)/2;

	h_baseline = new TH1D("baseline","baseline",nbin,min,max);

	for(int i=(peakx-100);i<(peakx-50); i++){
		if(i<1){
			continue;
		}
		base_sum+=dat[i];
		h_baseline->Fill(dat[i]);
		count++;
	}

	if(count==0){
		baseline_can= -10000000;
		h_mean = -100000;
		h_rms = -100000;
	}else{
		baseline_can=double(base_sum)/double(count);
		h_mean = h_baseline->GetMean();
		h_rms = h_baseline->GetRMS();
	}

	h_baseline->Delete();

	//std::cout<<" baseline"<<baseline_can<<" "<<h_mean<<std::endl;
	*baseline = baseline_can;
	*rms = h_rms;
}
