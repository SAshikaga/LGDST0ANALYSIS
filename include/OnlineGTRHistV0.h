#ifndef OnlineGTRHistV0_h
#define OnlineGTRHistV0_h

#include "OnlineGTRUtility.h"
#include "E16ANA_GTRAnalyzer2.h"
#include "E16ANA_GTRPedestal.h"
#include "E16DST/E16DST_DST0.hh"

#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>
#include <TCanvas.h>

class OnlineGTRAnalyzerV0 {
public:
   OnlineGTRAnalyzerV0();
   ~OnlineGTRAnalyzerV0();
   bool Exists(uint16_t module_id, uint16_t layer_id);
   void Analyze(E16DST_DST0PhysicsEvent *event);
   E16ANA_GTRAnalyzer2* Chamber(uint16_t module_id, uint16_t layer_id);
   void SetPedestal(E16ANA_GTRPedestal *pedestal);
   void Clear();
private:
   OnlineGTR::HashMap<E16ANA_GTRAnalyzer2*> analyzer_map;
};

class OnlineGTRHistV0_OneModule {
public:
   OnlineGTRHistV0_OneModule(TString base_name);
   ~OnlineGTRHistV0_OneModule();
   void Fill(E16ANA_GTRAnalyzer2 *analyzer100, E16ANA_GTRAnalyzer2 *analyzer200, E16ANA_GTRAnalyzer2 *analyzer300);

   enum {
      n_layers = 3
   };
   TH1D *h_max_strip_x[n_layers];
   TH1D *h_max_strip_y[n_layers];
   TH1D *h_max_strip_yb[n_layers];
   TH1D *h_cluster_timing_x[n_layers];
   TH1D *h_cluster_timing_y[n_layers];
   TH1D *h_cluster_timing_yb[n_layers];
   TH1D *h_cluster_charge_x[n_layers];
   TH1D *h_cluster_charge_y[n_layers];
   TH1D *h_cluster_charge_yb[n_layers];
   TH1D *h_timing_x[n_layers];
   TH1D *h_timing_y[n_layers];
   TH1D *h_timing_yb[n_layers];
   TH2D *h_hit_map[n_layers];
   TH1D *h_nhit_x[n_layers];
   TH1D *h_nhit_y[n_layers];
   TH1D *h_nhit_yb[n_layers];
   TH1D *h_peak_x[n_layers];
   TH1D *h_peak_y[n_layers];
   TH1D *h_peak_yb[n_layers];
   TH1D *h_part_peak_y[n_layers];
   TH1D *h_part_peakL_y[n_layers];
   TH1D *h_part_peakR_y[n_layers];
   TH1D *h_part_peak_yb[n_layers];
   TH1D *h_part_peakR_yb[n_layers];
   TH1D *h_part_peakL_yb[n_layers];
   TH2D *h_strip_peak[n_layers];
   TH2D *h_strip_timing[n_layers];
   TH1D *h_tot_x[n_layers];
   TH1D *h_tot_y[n_layers];
   TH1D *h_tot_yb[n_layers];
   TH2D *h_peak_tot_x[n_layers];
   TH2D *h_peak_tot_y[n_layers];
   TH2D *h_peak_tot_yb[n_layers];

   int sum1 = 0;
   int sum2 = 0;

   const double t_window = 50.0;
   const double threshold = 300.0;

};

class OnlineGTRHistV0 {
public:
   OnlineGTRHistV0();
   ~OnlineGTRHistV0();
   void Fill(OnlineGTRAnalyzerV0 *analyzer);
   bool Exists(uint16_t module_id);
   OnlineGTRHistV0_OneModule* Module(uint16_t module_id);

private:
   OnlineGTR::HashMap<OnlineGTRHistV0_OneModule*> hist_map;

};

class OnlineGTRSimpleCanvas : public TCanvas {
public:
   OnlineGTRSimpleCanvas(TString canvas_name, int _n_columns, int _n_rows, int width = 1024, int height = 768);
   ~OnlineGTRSimpleCanvas();
   void Add(int column, int row, TObject *obj, TString option);
   void Draw();

private:
   int n_columns;
   int n_rows;
   std::vector<TObject*> draw_objects;
   std::vector<TString> draw_options;

};

#endif // GTROnlineHist_h
