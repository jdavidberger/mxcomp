#include <assert.h>

namespace mxcomp {
  
  template <size_t N, typename... Types> struct GetType { };

  template <size_t N, typename T, typename... Types>
    struct GetType<N, T, Types...> {
    using type = typename GetType<N-1, Types...>::type;
    };
  
  template <typename T, typename... Types>
    struct GetType<0, T, Types...> {
    using type = T;
  };

  template <typename S, size_t N, typename... Types>
    struct GetIndex {    
    };
  
  template <typename S, size_t N, typename T, typename... Types>
    struct GetIndex<S, N, T, Types...> {
    const static size_t value = GetIndex<S, N+1, Types...>::value;
  };

  template <typename T, size_t N, typename... Types>
    struct GetIndex<T, N, T, Types...> {
    const static size_t value = N;
  };

  template <size_t S, size_t N, typename... Types>
    struct GetIndex<char[S], N, std::string, Types...> {
    const static size_t value = N;
  };

  template <typename... Types> struct Variant_;

  template<size_t N, class... Types>
    typename GetType<N, Types...>::type& get( Variant_<Types...>& v) {
    return v.template Get<N>();
  }

  template<size_t N, class... Types>
    const typename GetType<N, Types...>::type& get1( const Variant_<Types...>& v) {
    return v.template Get<N>();
  }

  template <typename... Types>
    struct Variant_ {
      size_t type = (size_t)-1;
      typedef void (Variant_<Types...>::*DeleteFunction)();
      DeleteFunction del = 0;
      void* data = 0;

      template <size_t N>
      void _del () {
	if(data != 0){
	  using T = typename GetType<N, Types...>::type;
	  delete (T*)data;
	  data = 0;
	  type = (size_t)-1;
	}
      }
      Variant_<Types...>(){}

      template <size_t N>
      void init(){
	if(data == 0){
	  assert(type  == (size_t)-1);
	  using T = typename GetType<N, Types...>::type;
	  type = N; 
	  data = new T();
	  del = &Variant_<Types...>::_del<N>;
	}
      }
      
      ~Variant_<Types...>() {
	if(data){
	  assert(del);
	  (this->*del)();
	}
      }

      template <size_t N>
      void Set( const typename GetType<N, Types...>::type& val ) {
	init<N>();
	assert(type == N || type  == (size_t)-1);
	type = N;
	*((typename GetType<N, Types...>::type *)data) = val;
      }

      template <size_t N>
      typename GetType<N, Types...>::type& Get() {
	init<N>();
	assert(type == N && data);
	return *((typename GetType<N, Types...>::type *)data);
      }

      template <size_t N>
      const typename GetType<N, Types...>::type& Get() const {
	assert(type == N && data);
	return *((const typename GetType<N, Types...>::type *)data);
      }

      template <typename T>
      Variant_<Types...>& operator=(const T& val) {
	Set< GetIndex<T, 0, Types...>::value >(val);
	return *this;
      }
	
	template <typename T>
	const T& as() const {
	  return Get<GetIndex<T, 0, Types...>::value > ();
	}

      template <typename T>
      T& as() {
	return Get<GetIndex<T, 0, Types...>::value > ();
      }

    protected:
      Variant_<Types...>(const Variant_<Types...>& v){

      }
      
    };
}
