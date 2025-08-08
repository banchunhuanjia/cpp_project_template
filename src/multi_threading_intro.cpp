#include<iostream>
#include<thread>
#include<vector>

volatile int counter=0;

void worker_task(){
  for(int i=0;i<100000;i++) counter++;
}

int main(){
  const int num_threads=10;
  std::cout<<"Starting"<<std::endl;
  std::vector<std::thread> threads;
  for(int i=0;i<num_threads;i++){
    threads.push_back(std::thread(worker_task));
  }
  std::cout<<"Waiting"<<std::endl;
  for(auto &thread:threads){
    thread.join();
  }
  std::cout<<"All threads finished"<<std::endl;
  long long except_value=(long long)num_threads*100000;
  std::cout<<"Expected:"<<except_value<<std::endl;
  std::cout<<"Actual:"<<counter<<std::endl;
  return 0;
}