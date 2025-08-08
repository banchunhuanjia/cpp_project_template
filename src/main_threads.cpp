#include<iostream>
#include<thread>

void thread_function(){
  std::cout<<"Hello form thread"<<std::endl;
}

int main(){
  std::cout<<"Starting main thread..."<<std::endl;
  std::thread t(thread_function);
  t.join();
  std::cout<<"Thread finished. Main thread exiting "<<std::endl;
  return 0;
}