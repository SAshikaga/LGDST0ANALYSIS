#include <fstream>
#include <iostream>
#include <string>
#include <LGBasic.h>

using namespace std;


string channelmap_path = "/e16/u/ashikaga/work/run0b/operation/map/channelmap/ch_pp_20200522_run0.txt";
string drs4assign_path = "/e16/u/ashikaga/work/run0b/operation/map/drs4assign/pp_map_20200522_run0.txt";
string timeoffset_path = "/e16/u/ashikaga/work/run0b/operation/map/timeoffset/timeoffset_20201219_tagtime829.txt";
string binary_path = "/e16/u/ashikaga/work/run0b/operation/map/binary/map.dat";

string LGBasic::file_channelmap(){
	return channelmap_path;
}

string LGBasic::file_drs4assign(){
	return drs4assign_path;
}

string LGBasic::file_timeoffset(){
        return timeoffset_path;
}

string LGBasic::file_binary(){
	return binary_path;
}
