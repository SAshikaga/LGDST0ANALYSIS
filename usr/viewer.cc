#include <TMultiGraph.h>
#include <TF1.h>
#include <TString.h>
#include <TLatex.h>
#include <TTree.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <vector>
#include <TChain.h>
#include <TCanvas.h>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include <string>
#include <TEventList.h>
#include <TBranch.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "LGVIEWER.h"

const int module_list[6] = {102,103,104,106,107,108};
const int block_list[38] = {0,1,2,3,4,5  ,10,11,12,13,14,15  ,20,21,22,23,24,25,26  ,30,31,32,33,34,35,36  ,40,41,42,43,44,45  ,50,51,52,53,54,55};


void LGMonitor(int size, std::string inputfile[]){
	TChain chain("tree");
	//std::cout<<"  size   "<<size<<std::endl;
	for(int i=0; i<size-1; i++){
		//std::cout<<"  file   "<< i << inputfile[i] <<std::endl;
		std::ifstream ifs(inputfile[i].c_str());
		if(!ifs.is_open()){
		std::cout<<"file "<<inputfile[i]<<" is not exist"<<std::endl;
		continue;
		}
		chain.Add(inputfile[i].c_str());
		}
	
	std::string outputpdf = inputfile[size-1];	
	std::string outputpdfall = outputpdf+".pdf";	
	std::string outputtext = outputpdf+".text";	
	int n = chain.GetEntries();

	chain.SetBranchStatus("*",0);
        chain.SetBranchStatus("Event",1);	
        chain.SetBranchStatus("Module",1);	
        chain.SetBranchStatus("Block",1);	
        chain.SetBranchStatus("Peak",1);	
        chain.SetBranchStatus("PeakX",1);	
        chain.SetBranchStatus("Baseline",1);	
        chain.SetBranchStatus("BaselineRMS",1);	
        chain.SetBranchStatus("Integral",1);
        chain.SetBranchStatus("Trig",1);
        chain.SetBranchStatus("Track",1);
	
	double peak = 0;
	int peakx = 0;
	double integral = 0;
	double baseline = 0;
	double baselinerms = 0;
	bool trig = false;
	bool track = false;
	uint16_t module = 0;
        uint16_t block = 0;	
        uint32_t event = 0;
	double peakarr[6][38] = {{0}};
	double peakxarr[6][38] = {{0}};
	double baselinearr[6][38] = {{0}};
	double baselinermsarr[6][38] = {{0}};
	double baselinermslocalarr[6][38] = {{0}};
	double baselinermsglobalarr[6][38] = {{0}};
	double integralarr[6][38] = {{0}};
	int N[6][38] = {{0}};
	TH1D* hpeak[6][38];	
	TH1D* hpeakx[6][38];	
	TH1D* hbaseline[6][38];	
	TH1D* hintegral[6][38];	
	TH1D* hpeakall[6][38];	
	TH1D* hpeakxall[6][38];	
	TH1D* hbaselineall[6][38];	
	TH1D* hintegralall[6][38];	
	TH2D* col[6][38];	

	TF1* f[6][38];
	for(int i=0; i<6; i++){
	for(int j=0; j<38; j++){
	hpeak[i][j] = new TH1D(Form("peacutk%d_%d",module_list[i],block_list[j]),Form("peakcut%d_%d",module_list[i],block_list[j]), 850, -25, 250);
	hpeakx[i][j] = new TH1D(Form("peakxcut%d_%d",module_list[i],block_list[j]),Form("peakxcut%d_%d",module_list[i],block_list[j]), 100, 80, 180);
	hbaseline[i][j] = new TH1D(Form("baselibecut%d_%d",module_list[i],block_list[j]),Form("baselinecut%d_%d",module_list[i],block_list[j]), 60, -30, 30);
	hintegral[i][j] = new TH1D(Form("integralcut%d_%d",module_list[i],block_list[j]),Form("integralcut%d_%d",module_list[i],block_list[j]), 350, -50, 300);
	hpeakall[i][j] = new TH1D(Form("peak%d_%d",module_list[i],block_list[j]),Form("peak%d_%d",module_list[i],block_list[j]), 400, -50, 800);
	hpeakxall[i][j] = new TH1D(Form("peakx%d_%d",module_list[i],block_list[j]),Form("peakx%d_%d",module_list[i],block_list[j]), 100, 80, 180);
	hbaselineall[i][j] = new TH1D(Form("baseline%d_%d",module_list[i],block_list[j]),Form("baseline%d_%d",module_list[i],block_list[j]), 60, -30, 30);
	hintegralall[i][j] = new TH1D(Form("integral%d_%d",module_list[i],block_list[j]),Form("integral%d_%d",module_list[i],block_list[j]), 350, -50, 300);
	col[i][j] = new TH2D(Form("peak:integral%d_%d",module_list[i],block_list[j]),Form("peak:integral%d_%d",module_list[i],block_list[j]),350, -50, 800, 400, -50, 1400);
	f[i][j] = new TF1(Form("peakf_%d_%d",module_list[i],block_list[j]),"gaus",0,10000);
	}
	}


	chain.SetBranchAddress("Event",&event);
	chain.SetBranchAddress("Module",&module);
	chain.SetBranchAddress("Block",&block);
	chain.SetBranchAddress("Peak",&peak);
	chain.SetBranchAddress("PeakX",&peakx);
	chain.SetBranchAddress("Baseline",&baseline);
	chain.SetBranchAddress("BaselineRMS",&baselinerms);
	chain.SetBranchAddress("Integral",&integral);
	chain.SetBranchAddress("Trig",&trig);
	chain.SetBranchAddress("Track",&track);
	//chain.SetBranchAddress("Scaler",&scaler);


	for(int i=0; i<n; i++){
		chain.GetEntry(i);
		int i_mod = -1;
		int i_block = -1;
		if(i%10000==0){	
		std::cout<<i<<"/"<<n<<std::endl;	
		}
		for(int k=0; k<6; k++){
			if(module == module_list[k]){
				i_mod=k;
				}
			}	
		for(int k=0; k<38; k++){
                        if(block == block_list[k]){
                                i_block=k;
                                }
                        }
		
		hpeakall[i_mod][i_block]->Fill(peak);
		hpeakxall[i_mod][i_block]->Fill(peakx);
		hbaselineall[i_mod][i_block]->Fill(baseline);
		hintegralall[i_mod][i_block]->Fill(integral);
		col[i_mod][i_block]->Fill(integral,peak);
		//if(track==1){
			peakarr[i_mod][i_block]+=peak;
			peakxarr[i_mod][i_block]+=peakx;
			baselinearr[i_mod][i_block]+=baseline;
			baselinermslocalarr[i_mod][i_block]+=baselinerms;
			cout<<module<<" "<<block<<" "<<integral<<endl;
			integralarr[i_mod][i_block]+=integral;
			N[i_mod][i_block]+=1;
			hpeak[i_mod][i_block]->Fill(peak);
			hpeakx[i_mod][i_block]->Fill(peakx);
			hbaseline[i_mod][i_block]->Fill(baseline);
			hintegral[i_mod][i_block]->Fill(integral);
		//	}

	}
	

	std::ofstream fout(outputtext.c_str());
    

        TArrayD* peak_arr[6] = {0};
        TArrayD* peakx_arr[6] = {0};
        TArrayD* baseline_arr[6] = {0};
        TArrayD* baselinermslocal_arr[6] = {0};
        TArrayD* baselinermsglobal_arr[6] = {0};
        TArrayD* integral_arr[6] = {0};
        TArrayD* N_arr[6] = {0};
	TCanvas* c_hpeak[6][38];
	TCanvas* c_hpeakx[6][38];
	TCanvas* c_hbaseline[6][38];
	TCanvas* c_hintegral[6][38];
	TCanvas* c_col[6][38];



	for(int i=0; i<6; i++){
		peak_arr[i] = new TArrayD(38);
		peakx_arr[i] = new TArrayD(38);
		baseline_arr[i] = new TArrayD(38);
		baselinermslocal_arr[i] = new TArrayD(38);
		baselinermsglobal_arr[i] = new TArrayD(38);
		integral_arr[i] = new TArrayD(38);
		N_arr[i] = new TArrayD(38);
	
	for(int j=0; j<38; j++){
		double peaksum = peakarr[i][j];
		int peakxsum = peakxarr[i][j];
		double integralsum = integralarr[i][j];
		double baselinesum = baselinearr[i][j];
		double baselinermssum = baselinermslocalarr[i][j];
		int Nsum = N[i][j];

		if(Nsum==0){
		//	std::cout<<"Nsum 0"<<std::endl;
			continue;
		}
		double peakrms = hpeak[i][j]->GetRMS();	
		double peakxrms = hpeakx[i][j]->GetRMS();	
		double baselinerms = hbaseline[i][j]->GetRMS();	
		double integralrms = hintegral[i][j]->GetRMS();	
		

		N_arr[i]->AddAt(Nsum,j);
		baselinermsglobalarr[i][j] = baselinerms;
		baselinermslocalarr[i][j] = baselinermssum/double(Nsum);
		baselinearr[i][j] = baselinesum/double(Nsum);
		integralarr[i][j] = integralsum/double(Nsum);
		peak_arr[i]->AddAt(double(peakarr[i][j]),j);
		peakx_arr[i]->AddAt(double(peakxarr[i][j]),j);
		baselinermslocal_arr[i]->AddAt(double(baselinermslocalarr[i][j]),j);
		baselinermsglobal_arr[i]->AddAt(double(baselinermsglobalarr[i][j]),j);
		baseline_arr[i]->AddAt(double(baselinearr[i][j]),j);
		integral_arr[i]->AddAt(double(integralarr[i][j]),j);
	
		//std::cout<<" Nsum "<<Nsum<<std::endl;
	
		fout<<module_list[i]<<" "<<block_list[j]<<" "<<peakarr[i][j]<<" "<<peakrms<<" "<<integralarr[i][j]<<" "<<integralrms<<" "<<peakxarr[i][j]<<" "<<peakxrms<<" "<<baselinearr[i][j]<<" "<<baselinerms<<"\n";				
		}
		}

	
 	LGVIEWER lgdraw;	
	TCanvas* c1_peak = lgdraw.LGVIEWER::Draw1D(peak_arr,6,"peak_ana2nd1D");
	TCanvas* c1_peakx = lgdraw.LGVIEWER::Draw1D(peakx_arr,6,"peakX_ana2nd1D");
	TCanvas* c1_baseline = lgdraw.LGVIEWER::Draw1D(baseline_arr,6,"baseline_ana2nd1D");
	TCanvas* c1_baselinermsglobal = lgdraw.LGVIEWER::Draw1D(baselinermsglobal_arr,6,"baselinermsglolbal_ana2nd1D");
	TCanvas* c1_baselinermslocal = lgdraw.LGVIEWER::Draw1D(baselinermslocal_arr,6,"baselinermslocal_ana2nd1D");
	TCanvas* c1_integral = lgdraw.LGVIEWER::Draw1D(integral_arr,6,"integral_ana2nd1D");
	TCanvas* c1_n = lgdraw.LGVIEWER::Draw1D(N_arr,6,"N1D");
	TCanvas* c2_peak = lgdraw.LGVIEWER::Draw2D(peak_arr,6,"peak_ana2nd2D");
	TCanvas* c2_peakx = lgdraw.LGVIEWER::Draw2D(peakx_arr,6,"peakX_ana2nd2D");
	TCanvas* c2_baseline = lgdraw.LGVIEWER::Draw2D(baseline_arr,6,"baseline_ana2nd2D");
	TCanvas* c2_baselinermsglobal = lgdraw.LGVIEWER::Draw2D(baselinermsglobal_arr,6,"baselinermsglobal_ana2nd2D");
	TCanvas* c2_baselinermslocal = lgdraw.LGVIEWER::Draw2D(baselinermslocal_arr,6,"baselinermslocal_ana2nd2D");
	TCanvas* c2_integral = lgdraw.LGVIEWER::Draw2D(integral_arr,6,"integral_ana2nd2D");
	TCanvas* c2_n = lgdraw.LGVIEWER::Draw2D(N_arr,6,"N2D");
	c1_peak->Print((outputpdfall+"[").c_str());
	c1_peak->Print(outputpdfall.c_str());
        c1_peakx->Print(outputpdfall.c_str());
        c1_baseline->Print(outputpdfall.c_str());
        c1_baselinermslocal->Print(outputpdfall.c_str());
        c1_baselinermsglobal->Print(outputpdfall.c_str());
        c1_integral->Print(outputpdfall.c_str());
        c1_n->Print(outputpdfall.c_str());
        c2_n->Print(outputpdfall.c_str());
        c2_peak->Print(outputpdfall.c_str());
        c2_peakx->Print(outputpdfall.c_str());
        c2_baseline->Print(outputpdfall.c_str());
        c2_baselinermsglobal->Print(outputpdfall.c_str());
        c2_baselinermslocal->Print(outputpdfall.c_str());
        c2_integral->Print((outputpdfall+"]").c_str());
	
//	fout_root->Close();
//	fout.close();

}





int main(int argc, char* argv[]){
	if(!(argc>2)){
		cout<<"input file1 .. input file2 outputpdf"<<endl;
		exit(1);
	}
	if(argc>2){
	std::string file[argc];
	for(int i=1; i<argc; i++){
		file[i] = argv[i];
	}
	
	std::cout<<"file: "<<argc<<"  first: "<<argv[1]<<" last: "<<argv[argc-2]<<std::endl;
	LGMonitor(argc,file);
	}

        return 0;
}


