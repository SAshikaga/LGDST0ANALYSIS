// 2015-11-02, uploaded by yokkaich
// 2015-11-02, uploaded by komatsu
// 2015-10-05, uploaded by yokkaich
// 2015-08-20, uploaded by yokkaich
// 2015-07-15, uploaded by nakai
// 2015-05-29, uploaded by yokkaich
// 2015-04-08, uploaded by nakai
#ifndef E16ANA_GTRStripAnalyzer_hh
#define E16ANA_GTRStripAnalyzer_hh 1

#include <iostream>

#include "E16ANA_GTRAnalyzer2.h"
//#include "E16ANA_WaveformFitter.h"
//#include "E16ANA_Waveform2dFitter.h"

class E16ANA_GTRAnalyzedStripHit {
public:
   enum {
      n_sampling = 24,
      is_x = 0,
      is_y = 1,
      is_yb = 2,
   };

private:
   int gem_max_strip;    // strip id
   double gem_max_value; // peak ADC value
   int gem_num_hit;
   double gem_cluster_charge;    // sum of ADC values in a cluster
   double gem_center_of_gravity; // mm
   double gem_tdc_hit;           // mm
   double gem_hit_pos;           // mm
   double tan_incident_angle;    // radian
   double timing;                // ns
   int layerID;                  // 0-2
   int moduleID;                 // 0-32

   int id;   // array intex in Analyzer
   int type; // x,y,yb=0,1,2

   double summed_waveform[n_sampling];
   std::vector<double> strip_charge;
   std::vector<double> strip_timing;
   std::vector<int> strip_id;
   std::vector<double> strip_pos;
   std::vector<double> strip_tot;

public:
   E16ANA_GTRAnalyzedStripHit() {
      SetInvalid();
   };
   ~E16ANA_GTRAnalyzedStripHit() {};

   int ID() {
      return id;
   };
   void SetID(int i) {
      id = i;
   };

   void SetMaxStrip(int _gem_max_strip) {
      gem_max_strip = _gem_max_strip;
   };
   void SetMaxValue(double _gem_max_value) {
      gem_max_value = _gem_max_value;
   };
   void SetNumHit(int _gem_num_hit) {
      gem_num_hit = _gem_num_hit;
   };
   void SetClusterCharge(double _gem_cluster_charge) {
      gem_cluster_charge = _gem_cluster_charge;
   };
   void SetCogHit(double _gem_center_of_gravity) {
      gem_center_of_gravity = _gem_center_of_gravity;
   };
   void SetTdcHit(double _gem_tdc_hit) {
      gem_tdc_hit = _gem_tdc_hit;
   };
   void SetTanTheta(double _tan_incident_angle) {
      tan_incident_angle = _tan_incident_angle;
   };
   void SetInvalid() {
      gem_max_strip = kInvalidValue;
      gem_max_value = kInvalidValue;
      gem_num_hit = kInvalidValue;
      gem_cluster_charge = kInvalidValue;
      gem_center_of_gravity = kInvalidValue;
      gem_tdc_hit = kInvalidValue;
      gem_hit_pos = kInvalidValue;
      tan_incident_angle = kInvalidValue;
      // x_or_y = kInvalidValue;
      timing = kInvalidValue;
      layerID = kInvalidValue;
      moduleID = kInvalidValue;
      id = kInvalidValue;
      type = kInvalidValue;

      strip_id.clear();
      strip_pos.clear();
      strip_tot.clear();
      strip_charge.clear();
      strip_timing.clear();
   };
   void AddWaveForm(double *wf) {
      for (int i = 0; i < n_sampling; i++) {
         summed_waveform[i] += wf[i];
      }
   };
   void ClearWaveForm() {
      for (int i = 0; i < n_sampling; i++) {
         summed_waveform[i] = 0.0;
      }
   }
   void SetTiming(double t) { timing = t; }
   void SetLayerAndModuleIDandType(int id1, int id2, int itype) {
      layerID = id1;
      moduleID = id2;
      type = itype;
   }

   bool IsX() { return type == is_x; }
   bool IsY() { return type == is_y; }
   bool IsYb() { return type == is_yb; }
   int Type() { return type; }

   void PushBackStrip(int id, double pos, double charge, double t, double tot = 0.0) {
      strip_id.push_back(id);
      strip_pos.push_back(pos);
      strip_charge.push_back(charge);
      strip_timing.push_back(t);
      strip_tot.push_back(tot);
   };

   int MaxStripId() {
      return gem_max_strip;
   };
   double MaxValue() {
      return gem_max_value;
   };
   int NumHit() {
      return gem_num_hit;
   };
   double ClusterCharge() {
      return gem_cluster_charge;
   };
   double CogHit() {
      return gem_center_of_gravity;
   };
   double TdcHit() {
      return gem_tdc_hit;
   };
   double TanTheta() {
      return tan_incident_angle;
   };
   double Timing() { return timing; }
   int ModuleID() { return moduleID; }
   int LayerID() { return layerID; }
   int StripID(int i) {
      return strip_id[i];
   };
   double StripPos(int i) {
      return strip_pos[i];
   };
   double StripCharge(int i) {
      return strip_charge[i];
   };
   double StripTiming(int i) {
      return strip_timing[i];
   };
   double StripTimeOverThreshold(int i) {
      return strip_tot[i];
   };

   enum {
      kInvalidValue = -1000000,
   };

   void Print() {
      std::cout << "E16ANA_GTRStripAnalyzer : "
                << "Num hit strips = " << gem_num_hit << ", Cluster charge = " << gem_cluster_charge
                << ", Cog hit pos = " << gem_center_of_gravity << " [mm], Tdc hit pos = " << gem_tdc_hit << " [mm]"
                << std::endl;
   };
};

class E16ANA_GTRStripAnalyzer {
   // friend void E16ANA_GTRAnalyzer2::SetParameters(std::string filename);
   // friend void E16ANA_GTR100Analyzer::SetParameters(std::string filename);
   friend class E16ANA_GTRAnalyzer2;
   friend class E16ANA_GTR100Analyzer;

public:
   E16ANA_GTRStripAnalyzer(int _n_strips, int _n_sampling);
   virtual ~E16ANA_GTRStripAnalyzer();
   virtual void Clear();
   virtual void SetFadc(int strip_id, int16_t *waveform);
   virtual void SetPedestal(double _fadc_ped[]){memcpy(fadc_ped, _fadc_ped, sizeof(double)*n_strips);};
   virtual void SetPedestal(int strip_id, double _fadc_ped){fadc_ped[strip_id] = _fadc_ped;};
   virtual void Analyze();

   virtual void Analyze2();
   virtual void Analyze2(int hitid); // only one hit

   virtual void AnalyzeV1();
   virtual void AnalyzeV1(int hitid) {
      Analyze2(hitid);
   };
   /*
      virtual void AnalyzeV2();
      virtual void AnalyzeV2(int hitid);//only one hit

      virtual void AnalyzeV3();
      virtual void AnalyzeV3(int hitid);//only one hit
   */
   void SetInverse(bool flag) {
      if (flag) {
         inverted = -1.0;
      } else {
         inverted = +1.0;
      }
   };

   std::vector<E16ANA_GTRAnalyzedStripHit> &GetAnalyzedHits() {
      return gem_analyzed_hits;
   };

protected:
   // Analysis parameters
   double drift_velocity;
   double drift_gap_center;
   double strip_pitch;
   double fadc_clock_period;
   double fadc_t0_correction;
   double gem_tdc_min;
   double gem_tdc_max;
   double gem_tr;
   double gem_threshold;
   double gem_tot_threshold;
   double threshold_fraction;
   double position_start;

   double inverted; // If strip order is the opposite direction to the local coordinate, ...

   double **fadc;
   double *fadc_ped;
   double *fadc_peak;
   double *fadc_peak_time;
   double *fadc_tdc;
   double *fadc_tot;

   int n_strips;
   int n_sampling;

   std::vector<int> fadc_valid_count;
   std::vector<std::vector<int>> clustered_strip_id;
   std::vector<E16ANA_GTRAnalyzedStripHit> gem_analyzed_hits;

   // E16ANA_WaveformFitter *wf1d_fitter;
   // E16ANA_Waveform2dFitter *wf2d_fitter;

   struct fit_params_t {
      int strip_id;
      std::vector<double> times;
      std::vector<double> peaks;
      fit_params_t(int _id, int size) {
         strip_id = _id;
         times.reserve(size);
         peaks.reserve(size);
      };
   };

   struct cluster_param_t {
      double pos;
      double time;
      double peak;
      cluster_param_t(double _pos, double _time, double _peak) {
         pos = _pos;
         time = _time;
         peak = _peak;
      };
   };

   double GetPosition(int strip_id) {
      return (strip_id * strip_pitch + position_start) * inverted;
   };

   void CalcWaveParamsPeak();
   void CalcWaveParamsPeak(int ch, double t_cutoff);
   void CalcWaveParamsFit(std::vector<fit_params_t> &fit_pars_array);
   void CalcWaveParamsFit(int ch, fit_params_t &fit_pars);
   // void CalcClusterParams(
   //      std::vector<double> &v1_pos, // 1D-array
   //      std::vector<double> &v1_time,
   //      std::vector<double> &v1_peak
   //      );
   void CalcClusterParams(std::vector<cluster_param_t> &v1);
   virtual int HitClusteringV0();
   virtual int HitClusteringV0(const int min_gap, const double cluster_threshold);
   virtual int HitClusteringV1(const int min_gap = 2, const double delta_tdc_threshold = 150.0);
   /*
      virtual int HitClusteringV2(const int min_gap = 2, const double delta_tdc_threshold = 150.0);
      virtual int HitClusteringV3(
            std::vector<std::vector<double> > &v2_x, // 2D-array
            std::vector<std::vector<double> > &v2_z,
            std::vector<std::vector<double> > &v2_peak
            );
   */
   std::vector<std::vector<double>> clustered_x;
   std::vector<std::vector<double>> clustered_z;
   std::vector<std::vector<double>> clustered_peak;

   void CalcCenterOfGravity(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit);
   void CalcTdcHit1(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit, int hitID);
   void
   CalcTdcHit2(const std::vector<int> &strip_ids, double tan_theta, E16ANA_GTRAnalyzedStripHit &hit); // fixed method
   void SetArraysForTdcMethods(const std::vector<int> &strip_ids, std::vector<double> &x_array,
                               std::vector<double> &time_array, std::vector<double> &peak_array);
   // void CalcTdcHit3(bool fix_flag, const std::vector<double> &x, const std::vector<double> &z, const
   // std::vector<double> &peak, E16ANA_GTRAnalyzedStripHit &hit, int hitID);
};

#endif // E16ANA_GTRStripAnalyzer_hh
