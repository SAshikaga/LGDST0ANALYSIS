#include <LGBasic.h>
#include <LGDST0ANA.h>
#include <TROOT.h>
#include <ROOT/TSeq.hxx>
#include <stdio.h>
#include <iostream>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>
#include "E16DST/E16DST_DST0.hh"

#include <LGGeometry.h>

using namespace std;

int LG1stAna(string inputfile, string outputfile){
	ROOT::EnableImplicitMT(10);
	LGDST0ANA lgana;
	lgana.MakeMap();
	lgana.SetMap();
	LGGeometry lggeo;
	lggeo.MakeLGGlobal();
	cout<<"ana start"<<endl;

	E16DST_DST0 *dst0 = new E16DST_DST0();
	if(!dst0->Open(inputfile, E16DST_DST0::ReadMode)){
         	std::cout << "failed to open file : " << "test.dst0" << std::endl;
         	return 1;
        }

	int n_event = 0;	

	TFile* fout = new TFile(outputfile.c_str(),"recreate");
	fout->SetCompressionLevel(4);
	TTree* tree = new TTree("tree","tree");

	uint16_t module, block;
	uint32_t spill, eventinspill, eventid, spillstartid=0;
	double waveform[Nsample];
	//double* waveform;//= new double[Nsample];
	int peakx, ip;
	double integral, baseline, baselinerms, peak;
	double rate, wftype;
	double globalpos[6];	
	bool trighit, trackhit, blockhit;
	int64_t  xpoint =0 ;
	int64_t  trackxpoint = 0;
	uint64_t time, spillstarttime=0;
	double timeinspill;
	double starttime, endtime;
//	uint64_t time, timeinspill, spillstarttime=0;
	int ncell = Nsample;

	tree->Branch("Module",&module,"Module/s");
	tree->Branch("Block",&block,"Block/s");
        tree->Branch("Time",&time,"Time/l");
        tree->Branch("TimeInSpill",&timeinspill,"TimeInSpill/D");
        tree->Branch("Rate",&rate,"Rate/D");
        tree->Branch("Trig",&trighit,"Trig/O");
        tree->Branch("Track",&trackhit,"Track/O");
        tree->Branch("Spill",&spill,"Spill/i");
        tree->Branch("EventInSpill",&eventinspill,"EventInSpill/i");
        tree->Branch("Event",&eventid,"Event/i");
        tree->Branch("WFtype",&wftype,"Wftype/D");
        tree->Branch("DRS4IP",&ip,"ip/I");
	tree->Branch("ncell",&ncell,"ncell/I");
        //tree->Branch("Waveform",&waveform[0],"Waveform[ncell]/D");
        tree->Branch("Waveform",waveform,"Waveform[ncell]/D");
        tree->Branch("GlobalPos",globalpos,"GlobalPos[6]/D");

	tree->Branch("Peak",&peak,"Peak/D");	
	tree->Branch("PeakX",&peakx,"PeakX/I");	
	tree->Branch("xpoint",&xpoint,"xpoint/L");	
	tree->Branch("Baseline",&baseline,"Baseline/D");	
	tree->Branch("BaselineRMS",&baselinerms,"BaselineRMS/D");	
	tree->Branch("Integral",&integral,"Integral/D");	
	tree->Branch("Starttime",&starttime,"Starttime/D");	
	tree->Branch("Endtime",&endtime,"Endtime/D");	

	while (dst0->ReadAnEvent() > 0) {

		if (n_event % 1000 == 0){
			std::cout << "NReadEvents = " << n_event << ", EventID = " << dst0->Event()->EventID() << std::endl;
			}
		
	
		if(dst0->EventType() == E16DST_DST0EventType::SpillStart){
		      E16DST_DST0SpillStartEvent *event = dynamic_cast<E16DST_DST0SpillStartEvent *>(dst0->Event());
      		      if (event == nullptr) {
      	 		continue;
     			 }
		      lgana.SetSpillInTime(event);
      		      continue;
        		}


		if (dst0->EventType() != E16DST_DST0EventType::Physics) {
         		continue;
     			 }

      		E16DST_DST0PhysicsEvent *event = dynamic_cast<E16DST_DST0PhysicsEvent*>(dst0->Event());
	
		//int n_scaler= event->ScalerLG().NumberOfHits();
			
		eventid = event->EventID();
		lgana.SetRate(event);
		lgana.SetTrigHit(event);
		
		spill = event->SpillID();
        	time = event->TimeStamp();
		timeinspill = lgana.GetSpillInTime(spill,time); 	
		
		auto& lghit = event->LG();
		int n_lghit = lghit.NumberOfHits();
		

		for(int i=0;i<n_lghit;++i){
		
			auto& hit = lghit.Hit(i);
			module = hit.ModuleID();
        		block = hit.BlockID();
	
			auto lg_analysed = lgana.LGGet1stAna(&hit);	
			if(lg_analysed == 0){
				continue;
			}
			trackhit = lgana.GetTrack(module,block,&trackxpoint);
			trighit = lgana.GetTrig(module,block);		
		
			double* dat = ( lg_analysed->Waveform); 
			for(int j=0; j<Nsample; j++){
				waveform[j] = dat[j];
			}
			//cout<<(waveform)[100]<<endl;
			ip = lg_analysed->IP;
			wftype = lg_analysed->WF_TYPE;
			peak = lg_analysed->Peak;
			peakx = lg_analysed->PeakX;
			baseline = lg_analysed->Baseline;
			baselinerms = lg_analysed->BaselineRMS;
			integral = lg_analysed->Integral;
			starttime = lg_analysed->Starttime;
			endtime = lg_analysed->Endtime;
			//lggeo.GetGlobalPosition(module, block, globalpos);
			auto globalposition = lggeo.GetGlobalPosition(module, block);
			//cout<<module<<" "<<block<<" "<<pos->R<<endl;		
			globalpos[0] = 	globalposition->X;
			globalpos[1] = 	globalposition->Y;
			globalpos[2] = 	globalposition->Z;
			globalpos[3] = 	globalposition->R;
			globalpos[4] = 	globalposition->Theta;
			globalpos[5] = 	globalposition->Phi;
	
			tree->Fill();
		
			}
			n_event++;
		}


	fout->Write();
	return 0;
	}

int main(int argc, char* argv[]){
	//string inputfile =  argv[1];
//	string inputfile =  "/ccj/w/data03a/E16/Run0a/prod-1.2/dst0/all/all-run003422.dst0";
	string inputfile = "/e16/u/e16daq/Run0a/offline/data200828/merge/003422_srs.gtr.trg.lg.dst0"; 
	//string inputfile = "/e16/u/ashikaga/work/drs4test/data/dst0/merge/00503.dst0"; 
	string outputfile = "test5.root";//argv[2];	
	LG1stAna(inputfile, outputfile);
	return 0;
	}
