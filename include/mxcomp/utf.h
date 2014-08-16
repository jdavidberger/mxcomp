#include <string>
#include <mxcomp/log.h>

namespace mxcomp {
  namespace utf {

    typedef char     utf8_char_t;
    typedef char16_t utf16_char_t;
    typedef char32_t utf32_char_t;

    typedef std::basic_string<utf8_char_t> utf8_t;
    typedef std::basic_string<utf16_char_t> utf16_t;
    typedef std::basic_string<utf32_char_t> utf32_t;

#define CONVERT(x, y)							\
    void convert(const x::value_type*, const x::value_type*, y&);	\
    void convert(const x&, y&);						\

    CONVERT(utf8_t, utf16_t);
    CONVERT(utf8_t, utf32_t);

    CONVERT(utf16_t, utf8_t);
    CONVERT(utf16_t, utf32_t);

    CONVERT(utf32_t, utf8_t);
    CONVERT(utf32_t, utf16_t);
    
#define WCONVERT(x)				\
    CONVERT(std::wstring, x);			\
    CONVERT(x, std::wstring);			

    WCONVERT(utf8_t);
    WCONVERT(utf16_t);
    WCONVERT(utf32_t);
    

    template<typename T, unsigned size, typename out>
      void convert(const T (& in)[size], out& o) {
      convert(&in[0], &in[size-1], o);
    }

#undef CONVERT
#undef WCONVERT

#define CONVERT(x, y)						\
    static inline y convert(const x& in) {			\
      y rtn;							\
      utf::convert(in, rtn);					\
      return rtn;						\
    }								\

    namespace utf8 {
      template <typename inT> CONVERT(inT, utf8_t);
    }

    namespace utfW {
      template <typename inT> CONVERT(inT, std::wstring);
    }
    namespace utf32 {
      template <typename inT> CONVERT(inT, utf32_t);
    }

    namespace utf16 {
      template <typename inT> CONVERT(inT, utf16_t);
    }

    CONVERT(std::wstring, std::string);
    CONVERT(std::string,  std::wstring);

    CONVERT(utf16_t, std::string);
    CONVERT(utf32_t, std::string);

#undef CONVERT

  }
}
