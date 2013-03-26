#include <mxcomp/tuple/apply.h>

namespace tupleExt {
  template <typename... Args, typename H>
    std::tuple<Args..., H> append(std::tuple<Args...>& lst, H& el){
    std::function< std::tuple<Args..., H> (Args&... args) > f = [el](Args&... args) { return std::make_tuple(args..., el); };
    return tupleExt::applyTuple(f, lst);
  }

  template <typename H, typename... Args>
    H& head(std::tuple<H, Args...>& lst){
    return std::get<0>(lst);
  }

  template <typename H, typename... Args>
    std::tuple<Args...> tail(std::tuple<H, Args...>& lst) {
    auto f = [](H& h, Args&... args){ return std::make_tuple(args...); };
    return tupleExt::applyTuple( f , lst);
  }

  template <typename T>
    struct of_type_ {
      
      template <typename... Out> 
      static auto get(std::tuple<Out...> out, const std::tuple<>&, int) -> std::tuple<Out...> {
	return out;
      }
      
      template <typename... Out, typename H, typename... In> 
	static auto get(std::tuple<Out...> out, std::tuple<H, In...> in, int) 
	-> typename std::enable_if<std::is_base_of<T, H>::value, decltype(of_type_<T>::get(append(out, head(in)), tupleExt::tail(in), 0))>::type 
      {
	return of_type_<T>::get(append(out, head(in)), tail(in), 0);
      }

      template <typename... Out, typename H, typename... In> 
	static auto get(std::tuple<Out...> out, std::tuple<H, In...> in, long) 
	RETURN(of_type_<T>::get(out, tail(in), 0) );
   
      template <typename H,typename... Args> static auto get(std::tuple<H, Args...>& tuple) 
	RETURN(of_type_<T>::get( std::make_tuple(), tuple, 0));

    };

  template <typename T, typename... Args>
    static inline auto of_type(std::tuple<Args...>& lst) RETURN( of_type_<T>::get(lst) )

}
