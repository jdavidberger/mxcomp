#pragma once
#include "utf.h"

namespace mxcomp {
  namespace utf {    
    template <> inline utf32_char_t extract_code_point(const utf32_char_t*& buffer, const utf32_char_t* end) {
      assert(buffer <= end);
      buffer += 1; 
      return buffer[-1];
    }

    template <> inline uint8_t push_code_points(utf32_char_t point, utf32_t& str) {
      str.push_back(point);      
      return 1;
    }

    template <> inline uint8_t code_length<utf32_char_t>(utf32_char_t point) {
      return 1;
    }

  }
}
