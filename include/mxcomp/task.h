#pragma once

#include <tuple>
#include <queue>
#include <mxcomp\tuple_ext.h>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace mxcomp {

     struct __declspec(dllexport) Task
     {
          virtual ~Task();
          virtual void Action() = 0;
     };


     template <typename T, typename FunctionT, typename... Args>
     struct Task_ : public Task {     
          std::tuple<Args...> args;
          T& t; 
          //typedef void (T::*FunctionT)(Args...);
          FunctionT f;

          Task_(T* _t, FunctionT _f, Args... _args) : t(*_t), f(_f), args(std::make_tuple(_args...)) {
               
          }

          void Action() {                    
               mxcomp::tuples::applyTuple(&t, f, args);
          }
     };

     template <typename T, typename F, typename... Args>
     Task*
     CreateTask(T* t, F f, Args... args)
     {
          return new Task_<T, F, Args...>(t, f, args...);
     }

     struct __declspec(dllexport) TaskQueue {
          bool running;
          std::mutex modify_lock; 
          std::condition_variable task_add;
          std::vector<std::thread*> pool;
          std::queue<Task*> tasks;

          void Stop();
          void Run();
          void Add(Task* task);
          Task* pop_task();
          TaskQueue(size_t threads = 20);
          ~TaskQueue();
     };
}