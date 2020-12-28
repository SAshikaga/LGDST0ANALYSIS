#include <iostream>
#include <complex>
#include <TTree.h>
#include <TFile.h>

using namespace std;
const int n_ch = 16;
void checkdata(string infile,string outfile){
	TFile* f = new TFile(infile.c_str());
	TTree* tree = (TTree*)f->Get("tree");
 	ULong64_t timestamp = 0;
	ULong64_t localtime = 0;
	double timediff = 0;
	double Ltimediff = 0;
	double trigdiff = 0;
	uint32_t eventid = 0;
	tree->SetBranchAddress("timestamp",&timestamp);	
	tree->SetBranchAddress("localTime",&localtime);	
	tree->SetBranchAddress("eventID",&eventid);	
	int n = tree->GetEntries();
	TFile* f_new = new TFile(outfile.c_str(),"recreate");
	TTree* result = new TTree("result","result");
	result->Branch("timediff",&timediff,"timediff/l");
	result->Branch("Ltimediff",&Ltimediff,"Ltimediff/l");
	result->Branch("eventID",&eventid,"eventID/i");
	result->Branch("localTime",&timestamp,"timestamp/D");
	result->Branch("timestamp",&localtime,"localTime/l");
	result->Branch("diff",&trigdiff,"diff/D");
	ULong64_t timestamp_old = 0;
        ULong64_t localtime_old = 0;
	for(int i=0; i<n; i++){
		if(i%1000==0){
		cout<<i<<"/"<<n<<endl;
		}
		tree->GetEntry(i);
		timediff = double(timestamp - timestamp_old)*8.;
		Ltimediff = double(localtime - localtime_old)*100./3.;
		trigdiff = timediff -Ltimediff;
		/*if(timediff>Ltimediff){
			trigdiff = timediff -Ltimediff;
		}else{
			trigdiff = -timediff +Ltimediff;
		}*/
		timestamp_old = timestamp;
		localtime_old = localtime;
		//cout<< timediff<<" "<<Ltimediff<<endl;
		result->Fill();
	}
	result->Write();
	f_new->Close();
}


int main(int argc, char* argv[]){
	string input = (argv[1]);
	string output = (argv[2]);
	checkdata(input, output);
	return 0;
}
