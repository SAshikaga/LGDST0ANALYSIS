#include <iostream>
#include <fstream>
#include "TMath.h"
#include "LGGeometry.h"

using namespace std;

const uint16_t block_list[38] = {0,1,2,3,4,5  ,10,11,12,13,14,15  ,20,21,22,23,24,25,26  ,30,31,32,33,34,35,36  ,40,41,42,43,44,45  ,50,51,52,53,54,55};
const uint16_t module_list[6] = {102, 103, 104, 106, 107, 108};
const double pi = 3.14;

void LGGeometry::MakeLGLocal(){
	for(int i=0; i<38; i++){
	double lx = 0;
	double ly = 0;
	double lz = 0;
	double ltheta = 0;
	double lphi = 0;

	switch(block_list[i]/10){
		case 0:
			lx = -1436;
			ly = -315;
			lz = (block_list[i]%10*2 -5)*882./14.;
			ltheta = (block_list[i]%10 - 2.5)*30./6.;
			lphi = -12.;
			break;
		case 1:
			lx = -1601;	
			ly = -209;
			lz = (block_list[i]%10*2 -5)*882./14.;
			ltheta = (block_list[i]%10 - 2.5)*30./6.;
			lphi = -7.;
			break;
		case 2:
			lx = -1757;
			ly = -83;
			lz = (block_list[i]%10*2 -6)*882./14.;
			ltheta = (block_list[i]%10 - 3)*30./7.;
			lphi = -3.;
			break;	
		case 3:
			lx = -1757;
                        ly = 83;
			lz = (block_list[i]%10*2 -6)*882./14.;
			ltheta = (block_list[i]%10 - 3)*30./7.;
			lphi = 3.;
			break;
		case 4: 
			lx = -1601;
			ly = 209;
			lz = (block_list[i]%10*2 -5)*882./14.;
			ltheta = (block_list[i]%10 - 2.5)*30./6.;
			lphi = 7.;
			break;
		case 5:
			lx = -1436;
			ly = 315;
			lz = (block_list[i]%10*2 -5)*882./14.;
			ltheta = (block_list[i]%10 - 2.5)*30./6.;
			lphi = 12.;
			break;
		default:
			break;
	}	


	localposition* localpos = new localposition;
	localpos->Y = ly;
	localpos->X = lx;
	localpos->Z = lz;
	localpos->R = abs(lx);
	localpos->Theta = ltheta;
	localpos->Phi = lphi;

	uint16_t channel = block_list[i];
	(* this->LGLocalGeometry)[channel] =  localpos;
	}

}


void LGGeometry::MakeLGGlobal(){
	MakeLGLocal();
	for(int i=0; i<6; i++){
		double theta_module = (module_list[i]-102)*30.;
		double sin = TMath::Sin(theta_module*TMath::Pi()/180.);
		double cos = TMath::Cos(theta_module*TMath::Pi()/180.);
		for(int j=0; j<38; j++){
			int block = block_list[j];
			localposition* localpos = (* this->LGLocalGeometry)[block];		
			double lx = localpos->X;
			double ly = localpos->Y;
			double lz = localpos->Z;
			double lr = localpos->R;
			double ltheta = localpos->Theta;
			double lphi= localpos->Phi;

			double gx = lx*cos - lz*sin;
			double gy = ly;
			double gz = lx*sin + lz*cos;

			LGGeometry::globalposition* globalpos = new LGGeometry::globalposition;
	        	globalpos->X = gx;
       		 	globalpos->Y = gy;
        		globalpos->Z = gz;
        		globalpos->R = lr;
        		globalpos->Theta = ltheta + theta_module;
        		globalpos->Phi = lphi;

		        string key = to_string(module_list[i])+to_string(block_list[j]);
       			(* this->LGGlobalGeometry)[key] =  globalpos;


			}
		}



	}


