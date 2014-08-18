#pragma once
#include "utf.h"

namespace mxcomp {
  namespace utf {    
    static inline utf8_char_t leading_data_mask(uint8_t length) {
      if(length > 1) 
	return (0xFF >> (length + 1));
      return 0x7F;
    }
        
    static inline utf8_char_t leading_utf8(uint8_t codelength) {
      if(codelength == 1) return 0;
      return 0xFF << (8 - codelength);
    }


     template <> inline uint8_t code_length(utf8_char_t leading) {
      if((leading & 0x80) == 0) {
	return 1; 
      } 

#pragma warning (disable : 4309)
      utf8_char_t mask = 0xE0; // 0b11100000
#pragma warning (default : 4309)

      for (uint8_t i = 2; i < 7; i++) {
	if( (leading & mask) == (mask << 1) )
	  return i;
	mask = (mask >> 1) | 0x80; // 0b111xxxxx -> 0b1111xxxx, etc	
      }
      assert(false);
      return 1;  
    }

     template <> inline utf32_char_t extract_code_point(const utf8_char_t*& buffer, const utf8_char_t* end) {
       if(buffer > end) {
	 std::cerr << buffer << std::endl;
	 assert(false);
       }
       assert(buffer <= end);

       uint32_t rtn = 0;
       const utf8_char_t* b = buffer; 
       auto len = code_length(buffer[0]);

       buffer += len;
       
       if(len == 1) {
            return (utf32_char_t)b[0];
       }
       
       rtn = leading_data_mask(len) & (utf32_char_t)b[0];
       
       for(uint8_t i = 1;i < len;i++) {
	 rtn <<= 6;
	 rtn += (b[i] & 0x3F); 
       }
       return rtn;
    }

      template <> inline uint8_t code_length<utf8_char_t>(uint32_t point) {
	if(point < 0x80) return 1;
	uint8_t mask = 0x20;
	for(uint8_t i = 2;i < 7;i++) {
	  point >>= 6; 
	  if(point < mask)
	    return i;
	  mask >>= 1;
	}

      auto blength = active_bits(point); 
      // TODO: there is a less stupid thign for this i'm sure      
      if(blength <= 11) {
	assert(blength > 7);
	return 2;
      } else if (blength <= 16) {
	return 3;
      } else if (blength <= 21) {
	return 4; 
      } else if (blength <= 26) {
	return 5;
      } 

      assert(blength < 31);
      return 6; 
    }

      template <> inline uint8_t push_code_points<utf8_char_t>(utf32_char_t point, utf8_t& str) {
      if( (point & 0xFFFFFF80) == 0) {
	str.push_back( (utf8_char_t) point );      
	return 1;
      }

      auto clength = code_length<utf8_char_t>(point); 
      assert(clength > 1);
      auto leading = leading_utf8(clength) | (point >> (6 * (clength - 1)));
      str.push_back((utf8_char_t) leading);
      for(auto i = clength - 1;i > 0;i--) {
	auto shift = (i - 1) * 6; 
        str.push_back((utf8_char_t)(0x80 | ((point >> shift) & 0x3F)));
      }
      return clength;
    }

  }
}
