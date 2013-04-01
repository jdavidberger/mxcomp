/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include <mxcomp/python.h>

struct Pet {
  std::string name;
};

struct Person {
  Pet p;
  std::string name;
  int age; 
  int getAge() const { return age; }
};

namespace mxcomp {
  METACLASS(Person){
    MEMBERS(FIELD(age), 
	    FIELD(name), 
	    FIELD(p),
	    FUNCTION(getAge));
  };

  METACLASS(Pet){
    MEMBERS(FIELD(name));
  };
}

MODULE(ReflectTest, Person, Pet);
