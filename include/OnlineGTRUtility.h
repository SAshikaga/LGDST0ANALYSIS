#ifndef OnlineGTRUtility_h
#define OnlineGTRUtility_h

#include <cstdint>
#include <map>
#include <unordered_map>

namespace OnlineGTR {
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
   IDs(uint16_t _module_id, uint16_t _layer_id) : 
      strip_id(0xffffffff), layer_id(_layer_id), module_id(_module_id)
   {};
   IDs(uint16_t _module_id) : 
      strip_id(0xffffffff), layer_id(0xffff), module_id(_module_id)
   {};
   ~IDs(){};
#if 0
   inline bool operator < (const IDs &rhs) const {
      return this->value64 < rhs.value64;
   };
   inline bool operator == (const IDs &rhs) const {
      return this->value64 == rhs.value64;
   };
   inline bool operator != (const IDs &rhs) const {
      return this->value64 != rhs.value64;
   };
#endif
};

template<class T>
using Map = std::map<uint64_t, T>;

template<class T>
using HashMap = std::unordered_map<uint64_t, T>;
};

#endif // OnlineGTRUtility_h
