/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once

#include "tuple_ext.h"
#include "metaprogramming.h"
#include <type_traits>
#include <tuple>

namespace mxcomp {

  struct Member {
  Member(const std::string& _name) : name(_name){}
    std::string name;
  };

  static inline std::ostream& operator <<(std::ostream& o, const Member& m){
    return o << m.name;
  }

  template <typename T> 
    struct Member_ : public Member{
  Member_(const std::string& _name) : Member(_name){}

  };

  struct Function {};

  template <typename T, typename Rtn, typename... Args>
    struct Function_ : Member_<T>, public Function {
    using FunctionT = Rtn (T::*)(Args...);
    FunctionT function;
  Function_(const std::string& _name, FunctionT _function ) : function(_function), Member_<T>(_name) {}
    
    Rtn invoke(T& t, Args... args){
      return (t.*function)(args...);
    }
  };

  template <typename T, typename... Args>
    struct Function_<T, void, Args...> : Member_<T>, public Function {
    using FunctionT = void (T::*)(Args...);
    FunctionT function;
  Function_(const std::string& _name, FunctionT _function ) : function(_function), Member_<T>(_name) {}
    
    void invoke(T& t, Args... args){
      (t.*function)(args...);
    }
  };

  struct Field {};

  template <typename T, typename S>
    struct Field_ : public Member_<T>, public Field {
    using FieldT = S (T::*);
    FieldT field;
  Field_(const std::string& _name, FieldT _field ) : field(_field), Member_<T>(_name) {}
    
    const S& get(const T& t) const {
      return (&t)->*field;
    }

    S& get(T& t) const {
      return (&t)->*field;
    }
    void set(T& t, const S& s) const{
      (&t)->*field = s;
    }
  };

  template <typename T, typename S, class Enable = void>
    struct SuperClass_ {
      SuperClass_(){ static_assert(std::is_base_of<S,T>::value, "Given parameters are invalid: S is not a subclass of T."); }
    };

  template <typename T, typename S>
    struct SuperClass_<T, S, typename std::enable_if< std::is_base_of<S, T>::value>::type >  { };

  template <typename T, typename Rtn, typename... Args>
    static auto make_function( const std::string& name, Rtn (T::*function)(Args...) )
    RETURN((Function_<T, Rtn, Args...>(name, function)))

    template <typename T, typename S>
    static auto make_field( const std::string& name, S (T::*field))
    RETURN((Field_<T, S>(name, field)))

    
    struct pullName {
      std::string operator()(const Member& f){
	return f.name;
      }
    };

  template <typename T> struct MetaClass {};

  template <typename _T, typename Provider = MetaClass<_T>, typename enable = decltype(Provider::members(), void()) > 
    struct MetaClass_ {        
      using t = int;
      static auto members() STATIC_RETURN( Provider::members() );
      static auto fields() STATIC_RETURN( (mxcomp::tuples::ofType< Field  >( members() )) );    
  };
    
  template <typename T, typename... Args> 
    static auto make_fields(Field_<T, Args>... args) 
    RETURN( (std::make_tuple(args...)))   

    
    template<typename type> 
    struct DefineT {
      using T = type;
    };


#define METACLASS(type)					\
  template <> struct MetaClass< type > : DefineT<type>

#define MEMBERS(fs...)				\
  static auto members()				\
    STATIC_RETURN(std::make_tuple(fs))

#define FIELDS(fs...)				\
  static auto fields()				\
    STATIC_RETURN(make_fields(fs))
    
#define FIELD(name)				\
  make_field( #name, &T::name)

#define FUNCTION(name)				\
  make_field( #name, &T::name)


}
