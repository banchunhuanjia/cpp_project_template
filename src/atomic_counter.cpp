//原子操作 std::atomic
//比锁轻量的无锁技术
//不过保护一段复杂逻辑还是要用std::mutex
#include<iostream>
#include<vector>
#include<atomic>
#include<thread>
#include<chrono>

std::atomic atomic_counter(0);

void atomic_worker_task(){
  for(int i=0;i<1000000;i++)atomic_counter++;
}
int main(){
  const int num_threads=10;
  long long increments_per_thread=1000000;
  std::cout<<"Starting"<<std::endl;
  auto start_time=std::chrono::high_resolution_clock::now();
  std::vector<std::thread> threads;
  for(int i=0;i<num_threads;i++){
    threads.push_back(std::thread(atomic_worker_task));
  }
  for(auto &thread:threads){
    thread.join();
  }

  auto end_time=std::chrono::high_resolution_clock::now();
  auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  long long excepted_value=(long long)num_threads*increments_per_thread;
  std::cout<<"Finished"<<std::endl;
  std::cout<<"Expected counter value:"<<excepted_value<<std::endl;
  std::cout<<"Actual counter value:"<<atomic_counter<<std::endl;
  std::cout<<"Time taken:"<<duration.count()<<std::endl;
  return 0;
}