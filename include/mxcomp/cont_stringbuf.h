#pragma once 

#include <sstream>
#include <string>
#include <memory>

namespace mxcomp {
     template <class charT, class traits = std::char_traits<charT>, class alloc = std::allocator<charT> >
     struct cont_stringbuf : public std::basic_streambuf < charT, traits >, std::basic_string<charT, traits, alloc>  {
     protected:
     using typename std::basic_streambuf<charT, traits>::int_type;
     using typename std::basic_streambuf<charT, traits>::char_type;
     using typename std::basic_string<charT, traits, alloc>::size_type;

          virtual int_type overflow(int_type ch) OVERRIDE {
	    this->push_back((charT)ch);
               return ch;
          }
          virtual std::streamsize xsputn(const char_type* s, std::streamsize count) OVERRIDE{
	    this->append(s, (size_type)count);
               return count;
          }
     };

     template <class charT, class traits = std::char_traits<charT>, class alloc = std::allocator<charT> >
     struct cont_shared_stringbuf : public std::basic_streambuf < charT, traits > {
          typedef std::basic_string<charT, traits, alloc> string_type;
     private:
          string_type* strBuffer = 0;
	  using typename std::basic_streambuf<charT, traits>::int_type;
	  using typename std::basic_streambuf<charT, traits>::char_type;

     public:
          cont_shared_stringbuf(string_type& str) : strBuffer(&str) {}
     protected:
          virtual int_type overflow(int_type ch) OVERRIDE{
               strBuffer->push_back((charT)ch);
               return ch;
          }
          virtual std::streamsize xsputn(const char_type* s, std::streamsize count) OVERRIDE{
	    strBuffer->append(s, (typename string_type::size_type)count);
               return count;
          }
     };


          /*
               Assign a contingous buffer to a string stream, so you can directly
               use the resulting buffer.

               Use:

               std::stringstream ss;
               auto buff = mxcomp::attachContinousBuffer(ss); 
               ss << "Whatever streams take " << 123 << etc;
               write(buff.data(), buff.length());

          */
    template <class charT, class traits,  typename allocator>
    std::unique_ptr<cont_stringbuf<charT, traits, allocator>> attachContinousBuffer(std::basic_stringstream<charT, traits, allocator>& s) {
         auto rtn = std::unique_ptr<cont_stringbuf<charT, traits, allocator>>(new cont_stringbuf<charT, traits>);
         //s.set_rdbuf(rtn.get());
         ((std::ios&)s).rdbuf(rtn.get());
         return rtn;
    }

    template <class charT, class traits, typename allocator>
    std::unique_ptr<cont_shared_stringbuf<charT, traits, allocator>>
         attachContinousBuffer(std::basic_stringstream<charT, traits, allocator>& s, std::string& use_this) {
         auto rtn = std::unique_ptr<cont_shared_stringbuf<charT, traits, allocator>>(new cont_shared_stringbuf<charT, traits>(use_this));
         //s.set_rdbuf(rtn.get());
	 ((std::ios&)s).rdbuf(rtn.get());
         return rtn;
    }
}
