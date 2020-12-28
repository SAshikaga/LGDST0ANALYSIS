#include <TCanvas.h>
#include <TArray.h>

using namespace std;

class LGVIEWER{

public:
	TCanvas* Draw1D(TArrayD* array[],int n_array, string title);
        TCanvas* Draw1DProfX(TArrayD* array[],int n_array, string title);
        TCanvas* Draw1DProfY(TArrayD* array[],int n_array, string title);
        TCanvas* Draw2D(TArrayD* array[],int n_array, string title);



};
