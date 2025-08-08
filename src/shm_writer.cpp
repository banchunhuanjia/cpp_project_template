#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>//信号量 等待-通知机制
#include<cstring>
#include<unistd.h>

#include"common_data.h"

int main(){
  std::cout<<"[Writer] Starting..."<<std::endl;
  //1.获取共享内存段的ID
  int shm_id=shmget(SHM_KEY,sizeof(SharedData),0666|IPC_CREAT);
  //key,size,flag
  //0666 八进制数，用来定义新创建共享内存段的访问权限
  //666 三个数位分别代表所有者 同组用户 其他用户 每个权限值可以是读(4),写(2),执行(1)
  //所以就是这三种用户都具有读写能力，之前我们好像遇到过7的
  //IPC_CREAT为代表不存在就创建新的的行为标志
  //|按位或(二进制数据有一个是1结果是1)，让两个flag都发挥作用
  if(shm_id<0){
    perror("shmget");
    return 1;
  }
  std::cout<<"[Writer] Shared memory segment created/found with ID "<<shm_id<<std::endl;
  //2.共享内存附加到本进程的地址空间
  void* shm_addr=shmat(shm_id,NULL,0);
  //id addr flag 
  //用NULL是让内核自动选址，0表示默认附加行为 这是一个工程习惯
  //因为内核不关心我在内存中放了什么数据，所以返回值是通用指针void*,可在强转后解引用和指针运算
  if(shm_addr==(void*)-1){
    perror("shmat");
    return 1;
  }
  std::cout<<"[Writer] Shared memory attached at address "<<shm_addr<<std::endl;
  //3.通过地址拿到结构体变量
  SharedData* data=static_cast<SharedData*>(shm_addr);

  //---添加信号量内容---
  std::cout<<"[Writer] Setting up semaphore..."<<std::endl;
  int sem_id=semget(SEM_KEY,1,0666|IPC_CREAT);
  //key num_sems flag
  if(sem_id<0){
    perror("semget");
    return 1;
  }
  //初始化信号量
  semctl(sem_id,0,SETVAL,0);//id sem_num command

  //4.初始化，写入数据
  data->data_ready=false;
  std::cout<<"[Writer] Writing data to shared memory..."<<std::endl;
  strcpy(data->name,"Dayang");
  data->age=20;
  sleep(3);
  data->data_ready=true;
  std::cout<<"[Writer] Data is ready for the reader"<<std::endl;
  
  //5.释放信号量通知reader(v操作)
  std::cout<<"[Writer] Data is ready Signaling the reader..."<<std::endl;
  struct sembuf sop_v={0,1,0};
  //这个结构体定义的是要做什么 v是1，p是-1
  //sem_num sem_op sem_flg,这个0还是默认的阻塞行为
  if(semop(sem_id,&sop_v,1)==-1){//id operations_array num_ops
    perror("semp V");
    return 1;
  }
  
  std::cout<<"[Writer] Waiting for user to exit"<<std::endl;
  //阻塞程序，简单暂停器，为了让reader执行完我们再结束，不然共享内存被删了
  std::cin.get();//按一下回车就会接收输入并返回
  //6.将共享内存分离出本进程
  if(shmdt(shm_addr)==-1){
    perror("shmdt");
    return 1;
  }
  //7.可选的删掉共享内存和信号量
  shmctl(shm_id,IPC_RMID,NULL);//这个NULL是当删除时第三个参数用不上，按惯例传NULL(占位符)
  semctl(sem_id,0,IPC_RMID);//0是0号sem的意思
  //id command buffer
  std::cout<<"[Writer] Cleaned up resources and exited"<<std::endl;
  return 0;
}