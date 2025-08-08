#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<chrono> //实现时间延迟

std::mutex mtx;
std::queue<int> data_queue;
std::condition_variable cv; //条件变量，用于线程间的等待和通信
//它不是锁而是一个通知板
bool production_finished=false;


void producer_task(){
  std::cout<<"[PRODUCER] Starting production..."<<std::endl;
  for(int i=1;i<=10;i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::unique_lock<std::mutex> lock(mtx);
    //需要于condition_variable公用时使用它，简单锁使用lock_gruad

    std::cout<<"[PRODUCER] Producing data: "<<i<<std::endl;
    data_queue.push(i);
    //这里其实可以算是这样的,而且lock应该设置在cout之前，因为打印信息也算是拿到修改公共资源的许可
    //为了严谨就是先上锁在宣布
    // {
    //   std::unique_lock<std::mutex> lock(mtx);
    //   std::cout<<"[PRODUCER] Producing data: "<<i<<std::endl;
    //   data_queue.push(i);
    // }
    cv.notify_one();
  }

  {//创建新作用域，让lock能自动释放
    std::unique_lock<std::mutex> lock(mtx);
    production_finished=true;
    std::cout<<"[PRODUCER] Finished production"<<std::endl;
  }
  cv.notify_all();
  //唤醒所有还在等待的消费者的意义是如果结束了它们还在睡眠会阻塞在wait环节，让程序卡住
}

void consumer_task(){
  std::cout<<"[CONSUMER] Waiting for data ..."<<std::endl;
  while(true){
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{return !data_queue.empty()||production_finished;});

    if(production_finished&&data_queue.empty()){
      std::cout<<"[CONSUMER] No more data. Exiting"<<std::endl;
      break;
    }

    int data=data_queue.front();
    data_queue.pop();
    lock.unlock();
    //对于锁的使用就是只有在对公共参数的维护时才使用，如果线程执行时还占用锁会失去并行的本意

    std::cout<<"[CONSUMER] Processing data "<<data<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

int main(){
  std::thread producer(producer_task);
  std::thread consumer(consumer_task);

  producer.join();
  consumer.join();

  std::cout<<"Finished"<<std::endl;
  return 0;
}