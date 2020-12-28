#include <iostream>
#include <fstream>
#include <iterator>
#include <TFile.h>
#include <LGDST0ANA.h>
#include <TROOT.h>
//#include <ROOT/TSeq.hxx>
#include <vector>
#include <string>
#include <unordered_map>
#include "E16DST/E16DST_DST0.hh"
#include "E16DST/E16DST_Constant.hh"

using namespace std;

void LGDST0ANA::SetRate(E16DST_DST0PhysicsEvent *event){
	 int n_scaler= event->ScalerLG().NumberOfHits();		
	 //scaler_rate.clear();
	 for(int i=0; i<n_scaler; i++){
		E16DST_DST0ScalerHit &hit = event->ScalerLG().Hit(i);
            	uint16_t scalermodule = hit.ModuleID();
            	uint16_t scalerblock = hit.ChannelID();
            	if((scalermodule==101)||(scalermodule==109)){
                	continue;
                }
		std::string key = std::to_string(scalermodule)+std::to_string(scalerblock);
		uint32_t scaler = hit.ShortScaler();
            	uint32_t scalertime = hit.ShortScalerTime();
		double hitrate = double(scaler)*1000000000/double(scalertime);
            	(*scaler_rate)[key]=hitrate;
		}
		
	}

double LGDST0ANA::GetRate(uint16_t module, uint16_t block){
	string key = to_string(module) + to_string(block);
	double shortscaler = (*(this->scaler_rate))[key];
	return shortscaler;
	}


void LGDST0ANA::SetTrigHit(E16DST_DST0PhysicsEvent *event){
	E16DST_DST0UT3 ut3 = event->UT3();
	trackmap->clear();
	trig_bl->clear();
	lghitmap->clear();
	int n_track = ut3.NumberOfTracks();
	uint64_t tagtime = event->TimeStamp();//ut3.TriggerTime();
	uint64_t trigtime = ut3.TriggerTime();



	for(int i=0; i<n_track; i++){
	    auto &trackdata = event->UT3().Track(i);
            uint16_t module = trackdata.ModuleID();
            uint16_t block = trackdata.ChannelID();
            uint32_t tracktime = trackdata.Time();
	    string key =  to_string(module) + to_string(block);
	    int64_t trackxpoint = (tagtime * 8) % 0x40000 - tracktime;
                if(trackxpoint<0){
                        trackxpoint = (tagtime * 8) % 0x40000 - tracktime + 0x40000;
                }
		if(trigtime==tracktime){
			(* this->trig_bl)[key]=true;
		}

	    (* this->trackmap)[key] = trackxpoint;
	}

	 auto& lgtrighit = event->TriggerLG();
	 int n_hit = lgtrighit.NumberOfHits();
	 for(int i=0; i<n_hit; i++){
	
		if(i>8){
			continue;	
			//break;
		}	

		auto& hit = lgtrighit.Hit(i);
		uint16_t module = hit.ModuleID();
		uint16_t block = hit.ChannelID();
		string key = to_string(module) + to_string(block);
	
		auto lghittime = hit.Time();
		int64_t xpoint = (tagtime * 8) % 0x40000 - lghittime;
		if(xpoint<0){
                        xpoint =  (tagtime * 8) % 0x40000  - lghittime  + 0x40000;
                }
		(* this->lghitmap)[key] =  xpoint;
		}


	}

bool LGDST0ANA::GetTrack(uint16_t module, uint16_t block, int64_t* xpoint){
	string key =  to_string(module) + to_string(block);
	bool trackhit = false;
	//cout<<(*(this->trackmap)->count(key)<<" "<<key<<endl;
	if((this->trackmap)->count(key) > 0 ){
		trackhit = true;
		*xpoint = (* this->trackmap)[key];
		//cout<<xpoint<<" "<<key<<endl;
	}else{
		trackhit = false;
		*xpoint = -99999;	
	}
	return trackhit;
	}

bool LGDST0ANA::GetLGHit(uint16_t module, uint16_t block, int64_t* xpoint){ 
	string key =  to_string(module) + to_string(block);
	bool lghit = false;
	if((this->lghitmap)->count(key) > 0 ){
		lghit = true;
		*xpoint = (* this->lghitmap)[key];
                //cout<<*xpoint<<" "<<key<<endl;
	}else{
		lghit =false;
		*xpoint =-999999;
	}
	return lghit;
	}


bool LGDST0ANA::GetTrig(uint16_t module, uint16_t block){
	string key =  to_string(module) + to_string(block);
	bool trighit = false;
	if((this->trig_bl)->count(key) > 0 ){
		trighit = true;
	}else{
		trighit = false;	
	}
	return trighit;
	}


void LGDST0ANA::SetSpillInTime(E16DST_DST0SpillStartEvent *event){
	uint32_t spill = event->SpillID();
        uint64_t time = event->TimeStamp();
	(*spillstarttime)[spill] = time;
	}

//uint64_t LGDST0ANA::GetSpillInTime(uint32_t spill, uint64_t time){
double LGDST0ANA::GetSpillInTime(uint32_t spill, uint64_t time){
	//uint64_t spillintime = (time - (*spillstarttime)[spill])*8;
	double spillintime = double(time - (*spillstarttime)[spill])*8;
	return spillintime;
	}



