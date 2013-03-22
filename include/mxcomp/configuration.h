/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */
#pragma once

#include <libconfig.h++>
#include <map>

namespace rikitiki {
/** Wrap up libconfig's Config object for a few reasons.

    - Expose a global configuration. Not a giant fan of singletons but this one is useful.
    - Shadow all the 'lookupValue' methods for strings, so that we can do variable expansion.
*/
    struct Configuration : public libconfig::Config {
    private:
      Configuration();
      void ReadGlobal();
    public:
      /** Global configuration reference     
       */
      static Configuration& Global();     
    };

}
