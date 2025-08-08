#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/wait.h>

//这个项目给我们的启示是任何可能失败的系统调用都要检查他的返回值
//打印详细错误会更好

int main(){
  int pipe_fd[2];//存放管道的文件描述符
  
  if(pipe(pipe_fd)==-1){
    perror("pipe");//打印错误信息用的，传入的参数是提示我是哪里出的问题
    //比cerr的优势是能知道失败原因
    return 1;
  }

  pid_t child_pid=fork();
  if(child_pid<0){
    perror("fork");
    return 1;
  }
  if(child_pid==0){
    close(pipe_fd[1]);//关闭写入端
    char buffer[128];
    //read和write是底层c风格API，不认识string只能用char
    ssize_t bytes_read;//signed size_t,这个变量几乎是用在I/O系统调用返回错误码用的
    std::cout<<"[Child] Waiting to receive a message from the pipe ..."<<std::endl;
    bytes_read=read(pipe_fd[0],buffer,sizeof(buffer)-1);//read是一个阻塞函数，直到有数据
    //所以需要close一端，这样read才不用永远阻塞
    if(bytes_read>0){
      buffer[bytes_read]='\0';
      std::cout<<"[Child] Message received "<<buffer<<std::endl;
    }else{
      std::cout<<"[Child] Read failed or pipe was closed"<<std::endl;
    }
    close(pipe_fd[0]);
    exit(0);
  }else{
    close(pipe_fd[0]);
    std::string message="Hello my child,this is a secret message";
    std::cout<<"[Parent] Sending a message to the child"<<std::endl;
    write(pipe_fd[1],message.c_str(),message.length());
    std::cout<<"[Parent] Message sent"<<std::endl;
    close(pipe_fd[1]);
    wait(NULL);
    std::cout<<"[Parent] Child has finished"<<std::endl;
  }
  return 0;
}