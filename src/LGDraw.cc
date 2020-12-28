#include <TF1.h>
#include <TString.h>
#include <TTree.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <vector>
#include <TGraph.h>
#include <TCanvas.h>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include <string>
#include <TEventList.h>
#include <TBranch.h>
#include "TArrayD.h"
#include "LGVIEWER.h"

const int modid[6] = {102,103,104,106,107,108};
const int modofs[6] = {5,4,3,2,1,0};
const int block_list[38] = {0,1,2,3,4,5  ,10,11,12,13,14,15  ,20,21,22,23,24,25,26  ,30,31,32,33,34,35,36  ,40,41,42,43,44,45  ,50,51,52,53,54,55};
const int x_block[38] = { 12,10,8,6,4,2,  12,10,8,6,4,2  ,13,11,9,7,5,3,1  ,13,11,9,7,5,3,1,  12,10,8,6,4,2,  12,10,8,6,4,2, };

TCanvas* LGVIEWER::Draw1D(TArrayD* array[],int n_array,std::string title){
	
   if( array == nullptr ) {
	std::cout<<"array error"<<std::endl;
	exit(1);
	}

   TH1I* h1[6];
   TGraph* g1[6];
   int MaxVal=0;
   int MinVal=0;
   std::cout<<"array size   "<<n_array<<std::endl;

   for(int i=0; i<n_array; ++i){
	
	int xbin_offset = i*40 +1;

	std::string histname = title + Form("_mod%d",modid[i]);
	h1[i]= new TH1I(histname.c_str(),histname.c_str(),38,xbin_offset,xbin_offset+38);

	for(int j=0; j<38; j++){
		int val = array[i]->GetAt(j);
		h1[i]->SetBinContent(j+1,val);

		if(MaxVal<val){
			  MaxVal = val;
		}
		if(MinVal>val){
        	  MinVal = val;
       		 }
	}
	g1[i] = new TGraph(h1[i]);
	g1[i]->SetLineColor(1+i);
        g1[i]->SetMarkerStyle(29);
        g1[i]->SetMarkerColor(1+i);
	}

   TCanvas* c = new TCanvas(title.c_str(),title.c_str(),700,500);
   TH1F* frame = (TH1F*) c->DrawFrame(0,int(MinVal*1.2),n_array*40+10,int(MaxVal*1.2)); 
   frame->SetTitle(title.c_str());    
   for(int k=0; k<n_array; k++){
	g1[k]->Draw("same PL");
	}
	return c;
	}


TCanvas* LGVIEWER::Draw2D(TArrayD* array[],int n_array,std::string title){

   if( array == nullptr ) {
        std::cout<<"array error"<<std::endl;
        exit(1);
        }



   TH2I* h2[6];
   int MaxVal=0;
   int MinVal=0;
   for(int i=0; i<n_array; i++){
	int xbin_offset = i*40 +1;

	std::string histname = title + Form("_mod%d",modid[i]);
	h2[i]= new TH2I(histname.c_str(),histname.c_str(),7,xbin_offset,xbin_offset+7,6,0,6);
  
	for(int j=0; j<38; j++){
		int xbin = 6-block_list[j]%10 +1;
	        int ybin = int(double(block_list[j])/10.)+1;	
		int val = array[i]->GetAt(j);
		h2[i]->SetBinContent(xbin,ybin,val);
		if(MaxVal<val){
			  MaxVal = val;
		}
		if(MinVal>val){
			  MinVal = val;
       		}
		}
	}

    const int NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t Red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t Green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t Blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, Red, Green, Blue, NCont);
    gStyle->SetNumberContours(NCont);
    gStyle->SetOptStat(0);

    Double_t levels[NCont];
    for(int s=0; s<NCont; s++){
       levels[s]= pow(double(MaxVal)*s/NCont,1);
       //levels[s]= std::log(double(MaxVal)*s/NCont);
                        }
    TCanvas* c = new TCanvas(title.c_str(),title.c_str(),1500,900);
    if(n_array>2){
     	c->Divide(3,2);
	}
    c->SetRightMargin(0.0);
    c->SetLeftMargin(0.0);
    for(int k=0; k<n_array; k++){
        c->cd(k+1);
        c->SetRightMargin(0.0);
        c->SetLeftMargin(0.0);
        h2[k]->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	h2[k]->SetMarkerSize(1.2);
	h2[k]->Draw("box coltz text");
	}
	return c;
}



TCanvas* LGVIEWER::Draw1DProfX(TArrayD* array[],int n_array,std::string title){
	
   if( array == nullptr ) {
	std::cout<<"array error"<<std::endl;
	exit(1);
	}

   TGraph* g1[6];
   int MaxVal=0;
   int MinVal=0;
   std::cout<<"array size   "<<n_array<<std::endl;
    
   for(int i=0; i<n_array; ++i){
	 int sum[14] = {0};
         int ave[14] = {0};
	 int x[14] = {0};
	 int xbin_offset = modofs[i]*14 +3;

	std::string histname = title + Form("_mod%d",modid[i]);
   for(int j=0; j<38; j++){
	int val = array[i]->GetAt(j);
//	std::cout<<i<<"   "<<j<<"    "<<val<<std::endl;
	sum[x_block[j]]+=val; 
        }

   for(int k=1; k<14; k++){
	if(k%2==0){
	   ave[k-1]= sum[k]/4;
	}	
	else if(k%2==1){
	   ave[k-1]= sum[k]/2;
	}
	if(MaxVal<ave[k-1]){
	   MaxVal = ave[k-1];
	}
	if(MinVal>ave[k-1]){
           MinVal = ave[k-1];
        }
	
	x[k-1] = xbin_offset + k;   	
	std::cout<<modid[i]<<"   "<<k<<"   "<<ave[k]<<std::endl;
	}
	x[13]=xbin_offset + 13;
	ave[13]=ave[12];
	g1[i] = new TGraph(14,&x[0],&ave[0]);
	g1[i]->SetLineColor(1+i);
	g1[i]->SetMarkerColor(1+i);
	if(i==4){
	g1[i]->SetLineColor(8);
	g1[i]->SetMarkerColor(8);
		}
        g1[i]->SetMarkerStyle(29);
	}

   TCanvas* c = new TCanvas(title.c_str(),title.c_str(),700,500);
   TH1F* frame = (TH1F*) c->DrawFrame(0,int(MinVal*1.2),6*16,int(MaxVal*1.2)); 
   frame->SetTitle(title.c_str());    
   for(int k=0; k<n_array; k++){
	g1[k]->Draw("same PL");
	}
	return c;
}

TCanvas* LGVIEWER::Draw1DProfY(TArrayD* array[],int n_array,std::string title){
	
   if( array == nullptr ) {
	std::cout<<"array error"<<std::endl;
	exit(1);
	}

   TGraph* g1[6];
   int MaxVal=0;
   int MinVal=0;
   std::cout<<"array size   "<<n_array<<std::endl;
    
   for(int i=0; i<n_array; ++i){
	 int sum[6] = {0};
         int ave[6] = {0};
	 int x[6] = {0};
	 int xbin_offset = 0;

	std::string histname = title + Form("_mod%d",modid[i]);
   	for(int j=0; j<38; j++){
		int val = array[i]->GetAt(j);
		int v_raw = block_list[j]/10;
		sum[v_raw]+=val; 
        }

	for(int k=0; k<6; k++){
		if(k==2||k==3){
	 		  ave[k]= sum[k]/7;
		}	
		else if(!(k==2)&&!(k==3)){
	  		  ave[k]= sum[k]/6;
		}
		if(MaxVal<ave[k]){
	   		  MaxVal = ave[k];
		}
		if(MinVal>ave[k]){
               	 	  MinVal = ave[k];
        	}
	
		x[k] = k;   	
		std::cout<<modid[i]<<"   "<<k<<"   "<<ave[k]<<std::endl;
	}
		g1[i] = new TGraph(6,&x[0],&ave[0]);
		g1[i]->SetLineColor(1+i);
		g1[i]->SetMarkerColor(1+i);
		if(i==4){
			g1[i]->SetLineColor(8);
			g1[i]->SetMarkerColor(8);
		}
       		g1[i]->SetMarkerStyle(29);
	}

   TCanvas* c1 = new TCanvas(title.c_str(),title.c_str(),700,500);
   c1->Divide(3,2);
   for(int k=0; k<n_array; k++){
	c1->cd(k+1);
   	g1[k]->SetTitle(title.c_str());    
	g1[k]->Draw("APL");
	}
   return c1;
}


