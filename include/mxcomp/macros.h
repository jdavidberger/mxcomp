/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#define COMMA ,
#define ACCESSOR(type, name)			\
  type& name;					\
  const type& name const;

#define ACCESSOR_IMPL(type, name, code)	\
  type& name {					\
    code;					\
  }						\
  const type& name const {			\
    code;					\
  }						
