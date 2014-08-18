#pragma once
#include <stdint.h>
#include <iostream>
#include <mxcomp/log.h>

namespace mxcomp {
  namespace utf {
    template <typename T> uint8_t code_length(T leading);
    template <typename T> uint8_t code_length(uint32_t);
    template <typename T> utf32_char_t extract_code_point(const T*&, const T*);
    template <typename T> uint8_t push_code_points(utf32_char_t, std::basic_string<T>&);

    static inline uint8_t active_bits(uint32_t point) {
      LOG(UTF, Debug) << std::hex << point << " has ";

      for(uint8_t rtn = 8; rtn < 32;rtn++){
	if( (1U << (rtn) > point ) ) {
	  LOG(UTF, Debug) << std::dec << (int)rtn << " active bits " << std::endl;
	  return rtn;
	}
      }
      assert(false);
      return 32;      
    }
  }
}
