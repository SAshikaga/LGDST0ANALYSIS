#include<iostream>
#include<fstream>
#include<string>
#include<string>
#include<TString.h>
#include<vector>
#include<algorithm>
#include<sstream>
#include <stdio.h>
#include "LGBasic.h"
#include "LGADJ.h"

const double gain_param = 1./7.638; 


void NewHVOut(std::string inputfileHV,std::string inputfileANA,std::string outputfileHV,double ref){
	LGADJ lgadj;
	//lgmap.MakeMap();
	lgadj.SetMap();
	
	ifstream ifshv(inputfileHV.c_str());
	ifstream ifsana(inputfileANA.c_str());
	if (!(ifshv)) {
		std::cout<<"No HV file"<<std::endl;
       		exit(1);
   	 	}
	if (!(ifsana)) {
		std::cout<<"No analysed data file"<<std::endl;
        	exit(1);
   	 	}
   
        auto hv_map = lgadj.LoadHV(inputfileHV);
	std::cout<<"loading hv table"<<std::endl;
        auto ana_map = lgadj.LoadAna(inputfileANA);
	std::cout<<"loading data table"<<std::endl;
	//LGADJ::anadata* test = (*ana_map)[to_string(10820)];
	std::cout<<"loading channel map"<<std::endl;
	//std::cout<<"OK"<<std::endl;
	std::ofstream outputfile(outputfileHV.c_str());
	
	//cout<<test->MODULE<<" "<<test->BLOCK<<endl;
	int i = 0;
	for (auto itr = ana_map->begin(); itr != ana_map->end(); itr++){	
		string key = itr->first;
		LGADJ::anadata* anaresult = itr->second;
		double peak = anaresult->PEAK;
		int module = anaresult->MODULE;
		int block = anaresult->BLOCK;
		if(module==0){
			cout<<key<<" is missing"<<endl;
			continue;
		}
		auto specmap = (lgadj.GetSpec(module, block));
		int hvch = specmap->HVCH;
		string hvkey = "u"+to_string(hvch);
		//cout<<hvkey<<endl;
		LGADJ::hvdata* hvval = (*hv_map)[hvkey];
		double oldval = hvval->HV;
		double ratito_peak = double(ref)/double(peak);
		double newval = oldval*pow(ratito_peak, gain_param);
		(hvval->HV) = newval;
		cout<<" "<<module<<" "<<block<<" "<<hvch<<": "<<oldval<<"=>"<<newval<<endl;
		outputfile<< hvval->HVCH <<" "<< hvval->LABEL <<" "<< hvval->HV <<" "<< hvval->CURRENT <<" "<< hvval->TAG <<"\n";		
		i++;
	}
	outputfile.close();

}



int main(int argc, char* argv[]){
    
    //std::string inputfileHV = argv[1];
    std::string inputfileHV = "/e16/u/ashikaga/work/run0b/operation/hv/10.txt";
    //std::string inputfileANA = argv[2];
    std::string inputfileANA = "/e16/u/ashikaga/work/run0b/operation/ana/wgtr_8ana2nd.text";
    //std::string outputfileHV = argv[3];
    std::string outputfileHV = "test.txt";

    //double ref= std::stod(argv[4]);
    double ref= 27;
    NewHVOut(inputfileHV,inputfileANA,outputfileHV,ref);
    string command = "sort -t ' ' -k 5 -n "+ outputfileHV+" -o "+outputfileHV;
    system(command.c_str()); 
    return 0;
           }
