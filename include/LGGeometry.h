#include <map>
#include <string>
#include <vector>
#include "TArrayD.h"
#include <TCanvas.h>
#include <unordered_map>

using namespace std;

class LGGeometry
{
 private:


 public:
 void MakeLGLocal();
 void MakeLGGlobal();

 struct globalposition{
	double X;
	double Y;
	double Z;
        double R;
        double Theta;
        double Phi;
        };
 struct localposition{
	double X;
	double Y;
	double Z;
        double R;
        double Theta;
        double Phi;
        };


 unordered_map<uint16_t, localposition*> *LGLocalGeometry = new unordered_map<uint16_t, localposition*>;
 unordered_map<string, globalposition*> *LGGlobalGeometry = new unordered_map<string, globalposition*>;
 
localposition* GetLocalPosition(uint16_t channel){
	auto posptr = ((* this->LGLocalGeometry)[channel]);
	return posptr;	
	}


globalposition* GetGlobalPosition(uint16_t module, uint16_t channel){
	string key = to_string(module) + to_string(channel);
	auto posptr = ((* this->LGGlobalGeometry)[key]);
	return posptr;	
	}


};
