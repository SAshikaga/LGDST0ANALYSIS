#include <vector>
#include "E16DST/E16DST_DST0.hh"
#include "LGBasic.h"
#include "LGDST0ANA.h"
#include "unordered_map"
#include <iostream>

using namespace std;

LGDST0ANA::lgqdc* LGDST0ANA::LGGet1stAna(E16DST_DST0LGHit* hit){
	

  	lgqdc* lgqdc = new struct lgqdc;	

	LGDST0ANA lgdata = *this;

		
	//vector<double> dat = {0};
	//dat.resize(Nsample);
	uint16_t module, block;
	double* waveform = new double[Nsample];
        module = hit->ModuleID();
        block = hit->BlockID();

	int64_t xpoint=0;
	bool blockhit = lgdata.GetLGHit(module,block,&xpoint); 
	if(blockhit==false||xpoint<600||xpoint>960){
		return 0;
		exit(1);
	}

	*(lgdata.eventtime) = xpoint;
	
	string key = to_string(module) + to_string(block);
//	cout<< "OK" << endl;	
	auto specmap = (this->GetSpec(module, block));	

	//cout<< specmap->IP << endl;
	int ip = specmap->IP;

	int timeoffset = specmap->TIME;	
	double wftype = specmap->WF_TYPE;
	for(int j=0; j<Nsample; j++){
		int ph = hit->Waveform()[j];
		waveform[j] = ph*wftype;	
		//dat[j] = hit->Waveform()[j];
		//(lgqdc->Waveform)[j] = ph*wftype;
	}	
	//cout<<"wf"<<waveform[130]<<endl;

	double peak_can = 0;
	int peakx_can = 0;
	double baseline_can = 0;
	double integral_can = 0;
	double baselinerms = 0;
	uint64_t reftime = *(this->eventtime);
	//cout<<reftime<<double(reftime)<<endl;
	int starttime = - (960./1000.)*double(reftime) + timeoffset - 10;
	//int starttime = 80;
	//int endtime =120;
	int endtime = -  (960./1000.)*double(reftime) + timeoffset + 10;
	//cout<<this->peakstart<<" "<<this->peakend<<endl;
	lgdata.LGWFPeak(waveform, &peak_can, &peakx_can, &starttime, &endtime);		
	lgdata.LGWFBaseline(waveform, peakx_can, &baseline_can, &baselinerms);	
	lgdata.LGWFIntegral(waveform, peakx_can, baseline_can, &integral_can);
	if(!(peakx_can==-1)&&!(baseline_can<-10000)){
	lgqdc->Waveform = waveform;
	//cout<<(lgqdc->Waveform)[100]<<endl;
	lgqdc->IP = ip;	
	lgqdc->WF_TYPE = wftype;	
	lgqdc->Module = module;
	lgqdc->Block = block;
	lgqdc->Peak = peak_can - baseline_can;
	lgqdc->PeakX = peakx_can;
	lgqdc->Baseline = baseline_can;
	lgqdc->BaselineRMS = baselinerms;
	lgqdc->Integral = integral_can;	
	lgqdc->Starttime = starttime;	
	lgqdc->Endtime = endtime;	
	//cout<<" module:"<<module<<" block:"<<block<<" Integral:"<<integral_can<<endl;
	return lgqdc;	
	}else{
	return 0;
	}	

}


