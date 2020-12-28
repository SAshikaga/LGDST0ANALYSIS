#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

//#ifndef I_G_HPP
//#define I_G_HPP
#include "LGBasic.h"
#include "LGDST0ANA.h"
//#endif //  I_G_HPP

#include <algorithm>
#include <unordered_map>   

//std::vector<LG::ch_pp> ip_ch_map;
//const int modid[6] = {102,103,104,106,107,108};
//const int block_list[38] = {0,1,2,3,4,5  ,10,11,12,13,14,15  ,20,21,22,23,24,25,26  ,30,31,32,33,34,35,36  ,40,41,42,43,44,45  ,50,51,52,53,54,55};


void LGBasic::MakeMap(){
	//LGBasic lgbasic;	
	unordered_map<string, ch_pp>  ch_map;
	//std::ifstream ifs_ip_pp("/e16/u/ashikaga/work/run0b/operation/map/drs4assign/pp_map_20200522_run0.txt");
	std::ifstream ifs_ip_pp(file_drs4assign());
	if (!ifs_ip_pp) {
		std::cout<<"[Error] pp map file is not found !"<<std::endl;
		exit(1);
	}
	std::vector<LGBasic::ip_pp> ip_pp_map;
        	
	while(!ifs_ip_pp.eof()){
	LGBasic::ip_pp ip_pp;
	ifs_ip_pp>>ip_pp.IP>>ip_pp.PP>>ip_pp.VTH_TYPE>>ip_pp.WF_TYPE;
        ip_pp_map.push_back(ip_pp);
	std::cout<<"making..."<<ip_pp.IP<<std::endl;
	}


	//std::ifstream ifs_ch_pp("/e16/u/ashikaga/work/run0b/operation/map/channelmap/ch_pp_20200522_run0.txt");
	std::ifstream ifs_ch_pp(file_channelmap());
	if (!ifs_ch_pp) {
		std::cout<<"[Error] ch map file is not found !"<<std::endl;
		exit(1);
	}

	//std::ifstream ifs_ip_time("/e16/u/ashikaga/work/run0b/operation/map/timeoffset/timeoffset_20201219_tagtime829.txt");
	std::ifstream ifs_ip_time(file_timeoffset());
        if (!ifs_ip_time) {
                std::cout<<"[Error] timing map file is not found !"<<std::endl;
                exit(1);
        }

	int ip;
	double time;
	double wfmean;
	while(!ifs_ip_time.eof()){
		ifs_ip_time>>ip>>wfmean>>time;
		cout<<ip <<" "<< wfmean<<" "<<time<<endl;
		auto ip_time_vector = std::find_if(std::begin(ip_pp_map),std::end(ip_pp_map),[ip](const LGBasic::ip_pp& val){
		return (val.IP==ip);
		});
	 if(!(ip_time_vector==ip_pp_map.end())){
		(ip_time_vector->TIME)	=  time;	
		}
	}


	fstream fout;
	//fout.open("/e16/u/ashikaga/work/run0b/operation/map/binary/map.dat", ios::binary | ios::out);
	fout.open(file_binary(), ios::binary | ios::out);

	while(!ifs_ch_pp.eof()){
	ch_pp ch_pp;
	ifs_ch_pp>>ch_pp.LorR>>ch_pp.DEG>>ch_pp.BLID>>ch_pp.MODULE>>ch_pp.BLOCK>>ch_pp.PP>>ch_pp.DRS4CH>>ch_pp.TRIGID>>ch_pp.HVCH;
        int pp = ch_pp.PP;	
	auto ip_pp_vector = std::find_if(std::begin(ip_pp_map),std::end(ip_pp_map),[pp](const LGBasic::ip_pp& val){
		return (val.PP==pp);
			});

	if(!(ip_pp_vector==ip_pp_map.end())){
	ch_pp.VTH_TYPE = ip_pp_vector->VTH_TYPE;
	ch_pp.WF_TYPE = ip_pp_vector->WF_TYPE;
	ch_pp.IP = ip_pp_vector->IP;
	ch_pp.TIME = ip_pp_vector->TIME;
	std::string key = std::to_string(ch_pp.MODULE)+std::to_string(ch_pp.BLOCK);
	ch_map[key] = ch_pp;
	fout.write((char*)&ch_pp, sizeof(ch_pp));
	}
	}
	fout.close();
}

void LGBasic::SetMap(){
	//unordered_map<string, ch_pp>* specmap;
	unordered_map<string, ch_pp*> mapdata;
	//unordered_map<string, int>  ipmap;
	//unordered_map<string, int>  calibmap;

	fstream fmap;
	fmap.open(file_binary(), ios::binary | ios::in);

	while(!fmap.eof()){
	ch_pp spec;
	fmap.read((char*)&spec, sizeof(spec));
	std::string key = std::to_string(spec.MODULE)+std::to_string(spec.BLOCK);	
	//ipmap[key] = spec.IP;
	
	double typeflag = spec.WF_TYPE;
	if(typeflag == 0){
		spec.WF_TYPE =0.44;
		}
	if(typeflag == 1){
                spec.WF_TYPE = 0.27;
                }
	ch_pp* specpoint = new ch_pp;
	*specpoint=spec;
	(* this->lgdatamap)[key] =  specpoint;
	}
	}

LGBasic::ch_pp* LGBasic::GetSpec(uint16_t module, uint16_t block){
	std::string key = std::to_string(module)+std::to_string(block);
	auto spec = (* this->lgdatamap)[key];
	return spec;  
		}


