#ifndef I_G_HP
#define I_G_HP

#include "LGBasic.h"
#include <string>
#include <vector>
#include "E16DST/E16DST_DST0.hh"
#include <unordered_map>
#include <iostream>
#include <TH1.h>

const int modid[6] = {102,103,104,106,107,108};
const int block_list[38] = {0,1,2,3,4,5  ,10,11,12,13,14,15  ,20,21,22,23,24,25,26  ,30,31,32,33,34,35,36  ,40,41,42,43,44,45  ,50,51,52,53,54,55};
const int Nsample = 200;
using namespace std;

class LGDST0ANA : public LGBasic
{


public:
//protected:
	int* eventtime = new int;
//	const int xofs = 962;
	unordered_map<string, int64_t>* trackmap = new unordered_map<string, int64_t>;
	unordered_map<string, int64_t>* lghitmap = new unordered_map<string, int64_t>;
	unordered_map<string, bool>* trig_bl = new unordered_map<string, bool>;
	unordered_map<string, uint32_t>* scaler_rate = new unordered_map<string, uint32_t>;
	unordered_map<uint32_t, uint64_t>* spillstarttime = new unordered_map<uint32_t, uint64_t>;
	TH1D* h_baseline;


//private:
	LGDST0ANA(){};
	~ LGDST0ANA(){}; 	

	struct lgqdc{
		double* Waveform = new double[Nsample];
		int IP;
		double WF_TYPE;
		uint16_t Module;
		uint16_t Block;
		double Peak;
		int PeakX;
		double Baseline;
		double BaselineRMS;
		double Integral;
		double Starttime;
		double Endtime;
	};

//	lgqdc LGQDC;
//	unordered_map<string, lgqdc> LGwfana;
	
//public:
	struct lgspec{
        char LorR;
        int DEG;
        int BLID;
        int MODULE;
        int BLOCK;
        int PP;
        int DRS4CH;
        int TRIGID;
        int HVCH;
        int IP;
        int VTH_TYPE;
        int WF_TYPE;
        };


	void LGWFPeak(double *dat, double* peakc, int* peakxc, int* start_x, int* end_x);
	void LGWFBaseline(double *dat, int peakxc, double* baselinec, double* rms);
	void LGWFIntegral(double *dat, int peakxc, double beselinec, double* integralc);

	/*void Set1stAna(unordered_map<string, lgqdc>* lgana){
		this -> LGwfana = (*lgana);
	};

	lgqdc Get1stAna(uint16_t module, uint16_t block){
		string key = to_string(module) + to_string(block);
		lgqdc qdcdata = (this->LGwfana)[key];
		return qdcdata;
        	};*/
	
	lgqdc* LGGet1stAna(E16DST_DST0LGHit* hit);	
	//void LGWFAnalysedValue(E16DST_DST0PhysicsEvent *event);
//	void LGWFAnalysedValue(unordered_map<string, LGBasic::ch_pp>* map,E16DST_DST0PhysicsEvent *event);

	unordered_map<string, LGBasic::ch_pp>  lgdatamap;	


	void SetRate(E16DST_DST0PhysicsEvent *event);
	void SetTrigHit(E16DST_DST0PhysicsEvent *event);
	void SetSpillInTime( E16DST_DST0SpillStartEvent *event);
	double GetRate(uint16_t module, uint16_t block);
	bool GetLGHit(uint16_t module, uint16_t block, int64_t* xpoint);
	bool GetTrack(uint16_t module, uint16_t block, int64_t* xpoint);
	bool GetTrig(uint16_t module, uint16_t block);
	//uint64_t GetSpillInTime(uint32_t spill, uint64_t time);
	double GetSpillInTime(uint32_t spill, uint64_t time);
};

	//unordered_map<string, LGBasic::ch_pp>  lgdatamap;
#endif // I_G_HP
