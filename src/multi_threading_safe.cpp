#include<iostream>
#include<thread>
#include<vector>
#include<mutex>

volatile int counter=0;
std::mutex mtx;//是互斥锁
//lock_guard是管理互斥锁的工具

void worker_task_safe(){
  for(int i=0;i<1000000;i++){
    std::lock_guard<std::mutex> lock(mtx);
    // --临界区开始
    counter++;
    // --临界区结束
    // 结束循环后lock会自动销毁
  }
}

int main(){
  const int num_threads=10;
  std::cout<<"Starting"<<std::endl;
  auto start_time=std::chrono::high_resolution_clock::now();
  std::vector<std::thread> threads;
  for(int i=0;i<num_threads;i++){
    threads.push_back(std::thread(worker_task_safe));
  }
  std::cout<<"Waiting"<<std::endl;
  for(auto &thread:threads){
    thread.join();
  }
  auto end_time=std::chrono::high_resolution_clock::now();
  auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
  std::cout<<"All threads finished"<<std::endl;
  long long except_value=(long long)num_threads*100000;
  std::cout<<"Expected:"<<except_value<<std::endl;
  std::cout<<"Actual:"<<counter<<std::endl;
  std::cout<<"Time taken:"<<duration.count()<<std::endl;
  return 0;
}