#include<iostream>
#include<thread>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<string>

const int PORT=8080;
const int BUFFER_SIZE=1024;

void receive_messages(int server_fd){
  char buffer[BUFFER_SIZE]={0};
  ssize_t bytes_read;
  while((bytes_read=read(server_fd,buffer,BUFFER_SIZE-1))>0){
    buffer[bytes_read]='\0';
    std::cout<<'\n'<<buffer<<std::endl<<"Enter message: ";
    std::fflush(stdout);//刷新输出
  }
}


int main(){
  int client_fd=socket(AF_INET,SOCK_STREAM,0);
  if(client_fd==-1){
    perror("socket failed");
    return 1;
  }
  sockaddr_in server_address;
  server_address.sin_family=AF_INET;
  server_address.sin_port=htons(PORT);
  if(inet_pton(AF_INET,"127.0.0.1",&server_address.sin_addr)<=0){
    perror("Invalid address/ Address not supported");
    close(client_fd);
    return 1;
  }
  if(connect(client_fd,(sockaddr*)&server_address,sizeof(server_address))<0){
    perror("connect failed");
    close(client_fd);
    return 1;
  }
  std::cout<<"[Client] Succcessfully connected to the server. You can start chatting."<<std::endl;
  
  std::thread receiver_thread(receive_messages,client_fd);
  receiver_thread.detach();//分离子线程，后台自由运行
  std::string message;
  while(std::getline(std::cin,message)){
    if(message=="exit"){
      break;
    }
    send(client_fd,message.c_str(),message.length(),0);
  }
  close(client_fd);
  return 0;
}