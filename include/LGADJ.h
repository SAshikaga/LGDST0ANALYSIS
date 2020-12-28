#include <unordered_map>
#include <LGBasic.h>
#include <string>

using namespace std;

class LGADJ: public LGBasic
{


private:
	
//	unordered_map<int, hvdata>* hvmap = new unordered_map<int, double>;






public: 
	struct hvdata{
		string HVCH;
		string LABEL;
		double HV;
		double CURRENT;
		int TAG;
	};

	struct anadata{
		int MODULE;
		int BLOCK;
		double PEAK;
		double PEAKX;
		double BASELINE;
		double INTEGRAL;
		double PEAKRMS;
		double PEAKXRMS;
		double BASELINERMS;
		double INTEGRALRMS;
	};


	unordered_map<string, hvdata*>* LoadHV(string inputfile);
	unordered_map<string, anadata*>* LoadAna(string inputfile);





};
