#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TTree.h>

using namespace std;

//int main(int argv, char* argc[]){
void maketimingoffset(string infile){
	//cout<< infile<<endl;
	TFile* f = new TFile(infile.c_str());
	TTree* tree = (TTree*) f->Get("tree");
	uint16_t module;
	uint16_t block;
	Long64_t xpoint;
	int peakx;
	int ip;

	if(!(tree)){
	//cout<<"no file"<<endl;
	exit(1);	
	}
	tree->SetBranchAddress("Module",&module);
	tree->SetBranchAddress("DRS4IP",&ip);
	tree->SetBranchAddress("Block",&block);
	tree->SetBranchAddress("PeakX",&peakx);
	tree->SetBranchAddress("xpoint",&xpoint);
	//cout<<"check"<<endl;
	int nevent = tree->GetEntries();
	TH1D* h[45];
	for(int i=0; i<44; i++){
		h[i] = new TH1D(Form("peak_%d",i),Form("peak_%d",i),200,0,200);
	}

	for(int j=0; j<nevent; j++){
		//cout<<"check"<<j<<endl;
		tree->GetEntry(j);

		//cout<<"check"<<j<<endl;
		if(xpoint>825&&xpoint<833){
//			cout<<"ip"<<ip<<endl;
			h[ip]->Fill(peakx);	
		}
	}
	
	TCanvas* c1 = new TCanvas("c1","c1");
	TCanvas* c2 = new TCanvas("c2","c2");
	c1->Divide(4,2);
	int count = 1;
	int nref = 10000;//h[16]->Integral();
	for(int i =16; i<43; i++){
		int nfill = h[i]->Integral();
		if(nfill==0){
			continue;
		}
		double mean = h[i]->GetMean(1);
		cout<<i<<" "<<mean<<endl;		
		c1->cd(count);
		count++;
		h[i]->Draw("hist");
		h[i]->Scale(double(nref)/double(nfill));
		c2->cd();
		h[i]->SetLineColor(i%10+1);
		h[i]->Draw("sames hist");	
	}
	c1->Print("time1.pdf","pdf");
	c2->Print("time2.pdf","pdf");
	
}

int main(int argc, char* argv[]){	
	string infile = argv[1];
	maketimingoffset(infile);
	return 0;
	}
