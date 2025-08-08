#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<thread>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const int BUFFER_SIZE=1024;
const int PORT=8080;

void handle_client(int client_socket){//从与客户端通话开始
  //获取客户端IP和端口信息用于日志打印
  sockaddr_in client_addr;
  socklen_t client_addr_len=sizeof(client_addr);
  getpeername(client_socket,(sockaddr*)&client_addr,&client_addr_len);
  //
  char client_ip[INET_ADDRSTRLEN];//in.h头文件中的宏常量，对于IPv4就用这个当地址字符串长度而不是自己数
  inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip));
  //server端用，把网络格式转为人能看懂的
  std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Handling client from"<<client_ip
    <<":"<<ntohs(client_addr.sin_port)<<std::endl;
    //与htons对应

  //原本的步骤
  char buffer[BUFFER_SIZE]={0};
  ssize_t bytes_read;
  while((bytes_read=read(client_socket,buffer,BUFFER_SIZE-1))>0){
    buffer[bytes_read]='\0';
    std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Received "<<buffer<<std::endl;
    send(client_socket,buffer,bytes_read,0);
  }
  if(bytes_read==0){
    std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Client disconnected"<<std::endl;
  }else{
    perror("read failed");
  }
  close(client_socket);
  std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Connection closed"<<std::endl;
}


int main(){
  int server_fd=socket(AF_INET,SOCK_STREAM,0);
  if(server_fd<0){
    perror("socket failed");
    return 1;
  }

  int opt=1;
  setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
  
  sockaddr_in address;
  address.sin_family=AF_INET;
  address.sin_addr.s_addr=INADDR_ANY;
  address.sin_port=htons(PORT);
  if(bind(server_fd,(sockaddr*)&address,sizeof(address))<0){
    perror("bind");
    close(server_fd);
    return 1;
  }

  if(listen(server_fd,10)<0){
    perror("listen failed");
    close(server_fd);
    return 1;
  }
  std::cout<<"[Main Thread] Server is listening on port"<<PORT<<"..."<<std::endl;

  std::vector<std::thread> client_threads;
  while(true){
    std::cout<<"[Main Thread] Waiting for a new connection..."<<std::endl;
    int client_socket=accept(server_fd,nullptr,nullptr);
    //使用空指针表示不需要这个信息的方式，因为第二个参数是输出参数(连接过来的客户端地址会存到其中)
    if(client_socket<0){
      perror("accept failed");
      continue;
    }
    std::cout<<"[Main Thread] Accepted a new connection"<<std::endl;
    client_threads.push_back(std::thread(handle_client,client_socket));
  }
  for(auto &thread:client_threads){
    if(thread.joinable()){
      thread.join();
    }
  }
  close(server_fd);
  return 0;
}