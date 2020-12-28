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
#include <TString.h>
#include "E16DST/E16DST_DST0.hh"

#include <LGGeometry.h>

using namespace std;

const int drs4list[16] = {16,17,18,20,25,26,34,21,19,23,30,29,22,41,40,42};

int main(int argc, char* argv[]){
	LGDST0ANA lgana;
	lgana.MakeMap();
	lgana.SetMap();
	string dir_path = "/e16/u/ashikaga/work/drs4test/data/root/";
	int nrun = atoi(argv[1]);
       
	vector<double>* Waveform[16] = {0};
	TBranch *brptr[16] = {0};

	TFile* fout = new TFile("test.root","recreate");

	TTree* outtree[16];// = new TTree("tree","tree");


/*	for(int d=0; d<16; d++){
	for(int ds=0; ds<16; ds++){
		outtree->Branch(Form("Baseline%d_%d",drs4list[d],ds),&baseline[d][ds],Form("Baseline%d_%d/D",drs4list[d],ds));
	}
	}*/

	for(int i=0; i<16; i++){//module
	
		int ip = drs4list[i];
		string file_path = dir_path + Form("%05d_%d.root",nrun,ip);
		TFile* f = new TFile(file_path.c_str());
		TTree* tree = (TTree*)f->Get("tree");
		if(!tree){
			cout<<"no file ip = "<<ip<<endl;
			continue;
			}
		fout->cd();	
		outtree[i] = new TTree(Form("tree%d",ip),Form("tree%d",ip));
		//outtree[i]->Branch("Event",&event,"Event/I");	
		double baseline[16];
		int event = 0;

		for(int ds=0; ds<16; ds++){
	                outtree[i]->Branch(Form("Baseline%d",ds),&baseline[ds],Form("Baseline%d/D",ds));
        	}
		
		TH1D* hbl[16];
		int nevent = tree->GetEntries();
		for(int s=0; s<16; s++){
			tree->SetBranchAddress(Form("Waveform%d",s),&Waveform[s],&brptr[s]);	
			hbl[s] = new TH1D(Form("baseline%d",s), Form("baseline%d",s), 500, -500, 500 );
		}

		for(int j=0; j<nevent; j++){//event
			tree->GetEntry(j);
			for(int k=0; k<2047; k++){//sampling cell
			//cout<<"get entry"<<j<<endl;
			for(int l=0; l<16; l++){//channel
				//cout<<"access wf "<<k<<endl;
				baseline[l]=Waveform[l]->at(k);		
				hbl[l]->Fill(baseline[l]);
			} 
			event = j;
			fout->cd();
			(outtree[i])->Fill();	
			}
		 }

		for(int p=0; p<16; p++){
			double mean = hbl[p]->GetMean();
			double sigma = hbl[p]->GetRMS();
			cout<<ip<<" "<<p<<" "<<mean<<" "<<sigma<<endl;
			hbl[p]->Delete();
		}
	//		cout<<baseline[i][0]<<endl;
		fout->cd();
		outtree[i]->Write();	
	}
		return 0;
}
