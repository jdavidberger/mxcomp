#pragma once
#include "utf.h"

namespace mxcomp {
  namespace utf {    
    template <> inline uint8_t code_length<utf16_char_t>(utf16_char_t leading) {
      if( leading < 0xD800 || leading > 0xDFFF) {
	return 1; 
      }       
      return 2;
    }

    template <> inline utf32_char_t extract_code_point(const utf16_char_t*& buffer, const utf16_char_t* end) {
         assert(buffer < end);
      auto len = code_length(buffer[0]);
      const utf16_char_t* b = buffer; 
      buffer += len;

      if(len == 1) 
	return b[0];

      return ((utf32_char_t)(b[0] - 0xD800) << 10) +
	((utf32_char_t)(b[1] - 0xDC00)) + 0x10000;	
    }

    template <> inline uint8_t push_code_points(utf32_char_t point, utf16_t& str) {
      assert(point < 0xD800 || point > 0xDFFF);
      if( point < 0x10000) {
	str.push_back( (utf16_char_t) point );      
	return 1;
      }
      
      point -= 0x10000;
      assert((point & 0xFFF00000) == 0);
      str.push_back( ((point >> 10) & 0x3FF) + 0xD800 );
      str.push_back( (point & 0x3FF) + 0xDC00 );

      return 2;
    }

    template <> inline uint8_t code_length<utf16_char_t>(uint32_t point) {
      assert(point < 0xD800 || point > 0xDFFF);
      if( point < 0x10000) {
	return 1; 
      }
      return 2;
    }

  }
}
