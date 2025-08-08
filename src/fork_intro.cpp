#include<iostream>
#include<unistd.h>//fork()
#include<sys/wait.h>//waitpid()
//使用了一些c风格头文件，因为这些都是底层的系统调用

//fork()是克隆

int main(){
  std::cout<<"[Parent] My PID is:"<<getpid()<<std::endl;
  std::cout<<"[Parent] Before forking..."<<std::endl;

  pid_t child_pid=fork();
  //用fork的返回值区分父子进程
  if(child_pid<0){
    std::cerr<<"Fork failed"<<std::endl;
    return 1;
  }else if(child_pid==0){//子
    std::cout<<"[Child] Hello,I am child process"<<std::endl;
    std::cout<<"[Child] My PID is "<<getpid()<<std::endl;
    std::cout<<"[Child] My parent's PID is "<<getppid()<<std::endl;
    sleep(2);
    std::cout<<"[Child] I am finishing my work and exiting"<<std::endl;
    exit(0);//这个在后面的WEXITSTATUS(status)会得到0
  }else{//父
    std::cout<<"[Parent] I have created a child process with PID "<<child_pid<<std::endl;
    std::cout<<"[Parent] I will wait for my child to finish..."<<std::endl;
    //需要等待子进程结束收回进程资源
    int status;//这是个状态信息打包变量，所以waitpid要传它
    waitpid(child_pid,&status,0);//阻塞父进程，直到指定的子进程结束
    //指定child_pid,0代表阻塞式等待
    if(WIFEXITED(status)){
      std::cout<<"[Parent] My child has finished with exit code "<<WEXITSTATUS(status)<<std::endl;
    }else{
      std::cout<<"[Parent] My child finished abnormally"<<std::endl;
    }
    std::cout<<"[Parent] My job is done. Exiting."<<std::endl;
  }
  return 0;
}