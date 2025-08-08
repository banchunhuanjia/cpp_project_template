#include<iostream>
#include<thread>
#include<chrono>
#include<future>

//promise,thread,unique_ptr不能拷贝，必须通过右值引用(&&)或指针传递
//右值引用就是告诉编译器他会被销毁，所以可以直接偷过来不用复制
void fetch_data_from_db(const std::string& query,std::promise<int>&& p){
  //因为p要set的，也就是需要修改，所以不能在传参的地方加const
  std::cout<<"[DB Task] Starting for query:"<<query<<std::endl;
  try{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int value=123;
    std::cout<<"[DB Task] Data fetched"<<std::endl;
    p.set_value(value);
  }catch(...){
    p.set_exception(std::current_exception());
  }
}

int main(){
  std::cout<<"[Main Thread] Preparing to fetch..."<<std::endl;
  std::promise<int> p;
  std::future<int> f=p.get_future();
  std::thread t(fetch_data_from_db,"Select * from users;",std::move(p));
  //move是一个强制地身份转换，因为p原本是左值，要把它伪装成右值
  //main中的p是空壳，不能再使用
  std::cout<<"Task launched! do other things"<<std::endl;
  for(int i=0;i<5;i++){
    std::cout<<"[Main Thread] Working on something else..."<<i+1<<"/5"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  int result=f.get();
  std::cout<<"[Main Thread] The result is:"<<result<<std::endl;
  t.join();
  //join的作用是让主线程等子线程结束再进行主线程
  std::cout<<"[Main Thread] Finished"<<std::endl;
  return 0;
}