//异步编程，让他后台自己跑吧，在未来某个时刻去要结果
//承诺未来模型，用于不同线程传来传去的情况
#include<iostream>
#include<thread>
#include<future>
#include<chrono>

int fetch_data_from_db(const std::string& query){
  std::cout<<"[DB Task] Starting for query:"<<query<<std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout<<"[DB Task] Data fetched"<<std::endl;
  return 123;
}

int main(){
  std::cout<<"[Main Thread] Preparing to fetch..."<<std::endl;
  std::future<int> result_future=std::async(std::launch::async,fetch_data_from_db,"select * from users;");
  //那个看着像sql的语句其实是query
  std::cout<<"[Main Thread] Task launched! do other things"<<std::endl;
  for(int i=0;i<5;i++){
    std::cout<<"[Main Thread] Working on something else..."<<i+1<<"/5"<<std::endl;
    //这是个加载动画
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  std::cout<<"\n[Main Thread] Need the data. Waiting for the result..."<<std::endl;
  int result=result_future.get();
  std::cout<<"[Main Thread] The result is:"<<result<<std::endl;
  std::cout<<"[Main Thread] Finished"<<std::endl;
  return 0;
}