#ifndef I_G_HPP
#define I_G_HPP

#include <vector>
#include "E16DST/E16DST_DST0.hh"
//#include "LGDST0ANA.h"
#include <unordered_map>
#include <iostream>
#include <set>
using namespace std;



class LGBasic
{

public:

LGBasic(){};
~LGBasic(){};

struct ch_pp{
        char LorR;
        int DEG;
        int BLID;
        int MODULE;
        int BLOCK;
        int PP;
        int DRS4CH;
        int TRIGID;
        int HVCH;
        int IP;
        int TIME;
        int VTH_TYPE;
        double WF_TYPE;
        };


//static LGBasic* lgbasic = new LGBasic();
//void LGSetUp();
void MakeMap();
void SetMap();

unordered_map<string, ch_pp*>*  lgdatamap = new unordered_map<string, ch_pp*>;

ch_pp* GetSpec(uint16_t module, uint16_t block);

string file_channelmap();
string file_drs4assign();
string file_timeoffset();
string file_binary();

protected:

struct ip_pp{
        int IP;
        int PP;
        int TIME;
        int VTH_TYPE;
        int WF_TYPE;
        };

private:


};



#endif //I_G_HPP
