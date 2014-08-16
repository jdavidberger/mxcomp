#include <string>
#include <assert.h>
#include <iostream>
#include <mxcomp/utf.h>
#include "utf8.h"
#include "utf16.h"
#include "utf32.h"
#include "utfwide.h"

namespace mxcomp {
  namespace utf {
    
    template <typename inT, typename outString >
    void convert_(const inT* curr, const inT* end, outString& out) {
      while(curr < end) {
	push_code_points( extract_code_point(curr, end), out);
      }
    }

    template <typename inT, typename outT>
    void convert_(const std::basic_string<inT>& in, std::basic_string<outT>& out){
      convert_(&in[0], &in[in.size()], out);
    }


    template <typename T> const T* strend(const T* str) {
      while( (str++) != 0);
      return str;
    }

#define CONVERT(x, y)						\
    void convert(const x& in, y& out) { convert_(in, out); }	\
    void convert(x::value_type const* in, x::value_type const* end, y& out) { convert_(in, end, out); }	\
    void convert(x::value_type const* in, y& out) { convert_(in, strend(in), out); }	
    
    CONVERT(utf8_t, utf32_t);
    CONVERT(utf8_t, utf16_t);
    CONVERT(utf8_t, std::wstring);
    
    CONVERT(utf16_t, utf8_t);
    CONVERT(utf16_t, utf32_t);
    CONVERT(utf16_t, std::wstring);

    CONVERT(utf32_t, utf8_t);
    CONVERT(utf32_t, utf16_t);
    CONVERT(utf32_t, std::wstring);

    CONVERT(std::wstring, utf8_t);
    CONVERT(std::wstring, utf16_t);
    CONVERT(std::wstring, utf32_t);
  }
}
