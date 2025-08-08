#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>

#include"common_data.h"

int main(){
  std::cout<<"[Reader] Starting..."<<std::endl;
  //1.拿到shm_id
  int shm_id=shmget(SHM_KEY,sizeof(SharedData),0666);//无IPC_CREAT表示只获取不创建
  if(shm_id<0){
    perror("shmget");
    std::cerr<<"[Reader] Hint:Is the writer process running?"<<std::endl;
    return 1;
  }
  std::cout<<"[Reader] Shared memory segment created/found with ID "<<shm_id<<std::endl;
  //2.拿到本进程来
  void* shm_addr=shmat(shm_id,NULL,0);
  if(shm_addr==(void*)-1){
    perror("shmat");
    return 1;
  }
  std::cout<<"[Reader] Shared memory attached at address "<<shm_addr<<std::endl;
  //3.通过地址拿到内容
  SharedData* data=static_cast<SharedData*>(shm_addr);

  std::cout<<"[Reader] Getting semaphore..."<<std::endl;
  key_t sem_id=semget(SEM_KEY,1,0666);
  if(sem_id<0){
    perror("semget");
    return 1;
  }

  //4.等待信号量(P操作)
  std::cout<<"[Reader] Waiting for the signal from writer..."<<std::endl;
  struct sembuf sop_p={0,-1,0};
  //信号量的值>0就减一让进程执行，如果==0就等待
  if(semop(sem_id,&sop_p,1)==-1){
    perror("semop P");
    return 1;
  }
  std::cout<<"[Reader] Signal received! Reading data..."<<std::endl;

  //5.读取
  std::cout<<"\n---Data Read from Shared Memory---"<<std::endl;
  std::cout<<"Name "<<data->name<<std::endl;
  std::cout<<"Age "<<data->age<<std::endl;
  //6.分离共享内存
  if(shmdt(shm_addr)==-1){
    perror("shmdt");
    return 1;
  }
  std::cout<<"[Reader] Shared memory detached"<<std::endl;
  //读取端不应该删除共享内存
  return 0;
}