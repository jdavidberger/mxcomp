/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#ifdef _MSC_VER
#include <windows.h>
typedef HANDLE pthread_mutex_t;
int pthread_mutex_lock(pthread_mutex_t *mutex) {
     return WaitForSingleObject(*mutex, INFINITE);
}
int pthread_mutex_unlock(pthread_mutex_t *mutex) {
     return ReleaseMutex(*mutex);
}
#else
#include <pthread.h>
#endif

/**
   Convienence class for thread lock/unlocking based on scope. 
 */
class Guard {  
     
  pthread_mutex_t* mut;
 public:
  /** Construct a guard object. 
      \param _mut mutex which is locked in the constructor and unlocked in the deconstructor
   */
 Guard(pthread_mutex_t& _mut) : mut(&_mut){
    pthread_mutex_lock(mut);
  }
  /** Releases the mutex
   */
  ~Guard(){
    pthread_mutex_unlock(mut);
  }
};
