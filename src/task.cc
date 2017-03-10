#include <mxcomp/task.h>

namespace mxcomp {
     
     Task::~Task()
     {
     }

     Task* TaskQueue::pop_task() {   
          std::unique_lock<std::mutex> lock(modify_lock);
          while (tasks.size() == 0) {
               task_add.wait(lock);
          }
          
          auto task = tasks.front();
          tasks.pop();
          tasks = std::queue<Task*>(tasks);
          return task;          
     }

     void ThreadFunction(TaskQueue* queue) {
          while (queue->running) {               
               auto task = queue->pop_task();                
               task->Action();
          }
     }
     TaskQueue::~TaskQueue(){
          Stop();
     }
     void TaskQueue::Add(Task* task) {
          std::lock_guard<std::mutex> lock(modify_lock);          
          tasks = std::queue<Task*>(tasks);
          tasks.push(task);          
          task_add.notify_one();          
     }
     void TaskQueue::Stop() {
          running = false;
          for (size_t i = 0; i < pool.size(); i++) {
               if (pool[i]) {
                    pool[i]->join();                    
                    delete pool[i];
                    pool[i] = 0;
               }
          }
     }
     void TaskQueue::Run() {
          running = true;
          for (size_t i = 0; i < pool.size(); i++) {
               pool[i] = new std::thread(&ThreadFunction, this);               
          }
     }

     TaskQueue::TaskQueue(size_t threads, bool run){   
          pool.resize(threads, 0);          
		  if (run)
			  Run();
     }

}