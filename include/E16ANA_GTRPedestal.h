#ifndef E16ANA_GTRPedestal_h
#define E16ANA_GTRPedestal_h

#include <cmath>
#include <string>
#include <unordered_map>

class E16ANA_GTRPedestal {
public:
   union IDs {
      uint64_t value64;
      struct {
         uint32_t strip_id;
         uint16_t layer_id;
         uint16_t module_id;
      };
      IDs(uint16_t _module_id, uint16_t _layer_id, uint32_t _strip_id) : 
         strip_id(_strip_id), layer_id(_layer_id), module_id(_module_id)
      {};
      IDs(uint64_t id) : 
         value64(id)
      {};
      ~IDs(){};
#if 0
      bool operator < (const IDs &rhs) const {
         return this->value64 < rhs.value64;
      };
#endif
   };

   class Pedestal {
   public:
      Pedestal() : 
         value(0.0), sigma(1.0), sum(0.0), square_sum(0.0), n_values(0)
      {
      };
      ~Pedestal(){
      };
      void AddADCValue(double adc){ // calculate mode
         sum += adc;
         square_sum += (adc*adc);
         n_values++;
      };
      double Value(){
         if(n_values <= 0){
            return value;
         }
         value = sum/(double)n_values;
         return value;
      };
      double Sigma(){
         if(n_values <= 0){
            return sigma;
         }
         double n = n_values;
         sigma = sqrt(square_sum/n - sum*sum/n/n);
         return sigma;
      };
      void SetValues(double v, double s){ // file read mode
         value = v;
         sigma = s;
         n_values = 0;
      };
   private:
      double value;
      double sigma;
      double sum;
      double square_sum;
      int n_values;
   };

   E16ANA_GTRPedestal();
   ~E16ANA_GTRPedestal();
   void Read(std::string fname);
   void Write(std::string fname);
   void AddADCValue(IDs id, double adc){
      pedestal_map[id.value64].AddADCValue(adc);
   };
   Pedestal& GetPedestal(IDs id){
      return pedestal_map[id.value64];
   };
   Pedestal& GetPedestal(uint16_t module_id, uint16_t layer_id, uint32_t strip_id){
      return GetPedestal(IDs(module_id, layer_id, strip_id));
   };

private:
   std::unordered_map<uint64_t, Pedestal> pedestal_map;

};

#endif // E16ANA_GTRPedestal_h
