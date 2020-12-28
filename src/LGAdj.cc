#include<iostream>
#include<fstream>
#include<string>
#include<string>
#include<TString.h>
#include<vector>
#include<algorithm>
#include<sstream>
#include <stdio.h>
#include "LGADJ.h"
#include <unordered_map>

using namespace std;

unordered_map<string, LGADJ::hvdata*>*  LGADJ::LoadHV(std::string inputfile){

	std::ifstream f_old(inputfile.c_str());
	if (!f_old) {
                std::cout<<"No HV file"<<std::endl;
                exit(1);
        }


	unordered_map<string, hvdata*>* hv_data_map = new unordered_map<string, hvdata*>;

     
	while(!(f_old.eof())){
		hvdata* hv_data = new hvdata;
		f_old >> hv_data->HVCH >> hv_data->LABEL >> hv_data->HV >> hv_data->CURRENT >> hv_data->TAG;
		string key = hv_data->HVCH;
		(*hv_data_map)[key] = hv_data;
		}

	return hv_data_map;
}



unordered_map<string, LGADJ::anadata*>* LGADJ::LoadAna(string inputfile){
        
	std::ifstream f_ana(inputfile.c_str());

	if(!f_ana){
        	std::cout<<"No analysed data file"<<std::endl;
        	exit(1);
         }


	unordered_map<string, anadata*>* ana_data_map = new unordered_map<string, anadata*>;

        while(!(f_ana.eof())){
                anadata* ana_data = new anadata;
        	f_ana >> ana_data->MODULE >> ana_data->BLOCK >> ana_data->PEAK >> ana_data->PEAKRMS >> ana_data->INTEGRAL >> ana_data->INTEGRALRMS >> ana_data->PEAKX >> ana_data->PEAKXRMS >> ana_data->BASELINE >> ana_data->BASELINERMS;
		string key = to_string(ana_data->MODULE) + to_string(ana_data->BLOCK);
		//cout<<key<<endl;
		if((ana_data->BLOCK)==0){
			cout<<key<<" "<<ana_data->MODULE<<" "<<ana_data->PEAK<<" "<<ana_data->PEAKRMS<<endl;

		}
		(*ana_data_map)[key] = ana_data; 
                }

        return ana_data_map;
                }
/*
double LG::GetHVVal(std::string hvch,std::vector<LG::hv_data> hv_data_map){
	auto selected_ch = std::find_if(std::begin(hv_data_map),std::end(hv_data_map),[hvch](const LG::hv_data& val){
                  return (val.HVCH==hvch);
                          });

	if(selected_ch==hv_data_map.end()){
	std::cout<<"NO HVCH MATCHED"<<std::endl;
	}

        double hv_val = selected_ch->HV;
        return hv_val;
	
}
*/


/*
int main(int argc, char* argv[]){
        int nrun = std::stoi(argv[1]);
   //     int ch = std::stoi(argv[2]);
	LG lg;
	std::vector<LG::hv_data> hv_data_map;	
        hv_data_map = lg.LG::LoadHV(nrun);
	double hv = lg.LG::GetHVVal("u104",hv_data_map);
	std::cout<<" (^^)/////  "<<hv<<std::endl;       	

	 return 0;
}*/
