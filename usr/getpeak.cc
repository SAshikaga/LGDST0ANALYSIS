#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <LGDST0ANA.h>
#include <TH1.h>


const int sample = 205;
using namespace std;

int En_ch(int ip){
	int en_ch = 0;
	switch(ip){
		case 19:
			en_ch = 5;
			break;
		case 23:	
			en_ch = 7;
			break;
                case 30:
                        en_ch = 11;
			break;
                case 22:
                        en_ch = 1;
			break;
                case 41:
                        en_ch = 5;
			break;
                case 40:
                        en_ch = 11;      
                        break;
		default:
			break;
	}
		return en_ch;
	}


int main(int argc, char* argv[]){
	TFile* f = new TFile(argv[1]);
	int ipin = atoi(argv[3]);
	TTree* tree = (TTree*)f->Get("tree");
	if(!tree){
		cout<<"not found:"<<argv[1]<<endl;
		exit(1);
	}
	vector<double>* waveform[16] = {0};
        TBranch *brptr[16] = {0};
	LGDST0ANA lgdata;

	double* dat = new double[Nsample];

	TFile* fout = new TFile(argv[2],"recreate");
	TFile* foutall = new TFile("test.root","update");
	double peak[16];
	int peakx[16];
	TH1D* hpeak[16];
	TTree* result = new TTree(Form("tree_%d",ipin),Form("tree_%d",ipin));
	int ch = En_ch(ipin);
	int n = tree->GetEntries();
	for(int i=0; i<16; i++){
		tree->SetBranchAddress(Form("Waveform%d",i),&waveform[i],&brptr[i]);
		hpeak[i] = new TH1D(Form("peak%d_%d",ipin,i),Form("peak%d_%d",ipin,i), 500, -10, 490);
		result->Branch("Peak",peak,"Peak[16]/D");

	}
	
	for(int eve=0; eve<n; eve++){
		int starttime = 80;
	        int endtime = 120;
		if(eve%1000==0){
			cout<<eve<<"/"<<n<<endl;
			if(eve==2000){
					break;
				}
		}

		tree->GetEntry(eve);
		for(int j=0; j<16; j++){
			for(int k=0; k<Nsample; k++){
				(dat)[k] = (waveform[j]->at(k))*0.44;
			}
			lgdata.LGWFPeak(dat, &peak[j], &peakx[j], &starttime, &endtime);		
			hpeak[j]->Fill(peak[j]);
		}
	//	cout<<peak[5]<<endl;
		result->Fill();
	}

	fout->cd();	
	result->Write();
	foutall->cd();
	hpeak[ch]->Write();
	return 0;
}
