#include<iostream>
#include<thread>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include<string>

const int PORT=8080;
const int BUFFER_SIZE=1024;

int main(){
  //1.要个手机
  int client_fd=socket(AF_INET,SOCK_STREAM,0);
  if(client_fd==-1){
    perror("socket failed");
    return 1;
  }
  std::cout<<"[Client] Socket created successfully"<<std::endl;
  //2.申请个号码
  sockaddr_in server_address;
  server_address.sin_family=AF_INET;
  //没有绑定所有网络接口的步骤
  server_address.sin_port=htons(PORT);

  //将IP地址字符(p)串编程网络字节序(n)的整数 client端用
  if(inet_pton(AF_INET,"127.0.0.1",&server_address.sin_addr)<=0){
    //&server_address.sin_addr是转换的结果要存储的目标地址
    perror("Invalid address/ Address not supported");
    close(client_fd);
    return 1;
  }
  //3.拨打电话
  if(connect(client_fd,(sockaddr*)&server_address,sizeof(server_address))<0){
    //socket_fd server_address_pointer address_length
    perror("connect failed");
    close(client_fd);
    return 1;
  }
  std::cout<<"[Client] Succcessfully connected to the server"<<std::endl;
  //---开始与服务器通信
  std::string message;
  char buffer[BUFFER_SIZE]={0};
  std::cout<<"Enter a message to send to the server:"<<std::endl;
  std::getline(std::cin,message);
  //4.发数据给服务器
  send(client_fd,message.c_str(),message.length(),0);
  std::cout<<"[Client] Message sent "<<message<<std::endl;
  //5.接收回声
  ssize_t bytes_read=read(client_fd,buffer,BUFFER_SIZE-1);
  if(bytes_read>0){//因为后面就是close，所以这样可以减少重复代码 
    buffer[bytes_read]='\0';
    std::cout<<"[Client] Echo received "<<buffer<<std::endl;
  }else{
    std::cout<<"[Client] Read failed or server closed connection"<<std::endl;
  }
  //6.挂电话
  close(client_fd);
  std::cout<<"[Client] Connection closed"<<std::endl;
  return 0;
}