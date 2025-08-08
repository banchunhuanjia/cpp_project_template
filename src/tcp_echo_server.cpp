#include<iostream>
#include<string>
#include<unistd.h>//read,write,close
#include<sys/socket.h>//socket
#include<netinet/in.h>//sockaddr_in结构体
#include<arpa/inet.h>//inet_addr 虽然我们没用上这个函数，但这些头文件是捆绑的
#include<cstring>

const int PORT=8080;
const int BUFFER_SIZE=1024; 


int main(){
  //1.创建socket 要个手机
  int server_fd=socket(AF_INET,SOCK_STREAM,0);
  //domain type protocol
  //使用IPv4 流式套接字 默认协议
  if(server_fd==-1){
    perror("scoket failed");
    return 1;
  }
  std::cout<<"[Server] Socket created successfully"<<std::endl;

  //解决address already in use技巧: 允许端口重用
  int opt=1;//1代表开启，这opt算是个选项
  setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
  //SOL_SOCKET代表选项的级别是套接字层

  //2.绑定IP和端口 配个电话号码
  sockaddr_in address;//in是互联网的意思
  //sin是sockaddr internet
  address.sin_family=AF_INET;//IPv4
  address.sin_addr.s_addr=INADDR_ANY;//绑定所有网络接口的IP
  address.sin_port=htons(PORT);//设置端口号
  // htons保证都使用统一的网络字节序(host ot network short)
  if(bind(server_fd,(sockaddr*)&address,sizeof(address))<0){
    //socket_fd address_struct_pointer address_length 
    perror("bind failed");
    close(server_fd);
    return 1;
  }
  std::cout<<"[Server] Socket bound to port"<<PORT<<std::endl;

  //3.开始监听 打开电话铃声
  if(listen(server_fd,3)<0){//socket_fd backlog_queue_size
    //最多允许三个连接排队
    perror("listen failed");
    close(server_fd);
    return 1;
  }
  std::cout<<"[Server] Listening for incoming connections..."<<std::endl;

  //4.接受客户端连接 接电话
  int addrlen=sizeof(address);
  int client_socket=accept(server_fd,(sockaddr*)&address,(socklen_t*)&addrlen);
  //listening_socket_fd client_address_struct_pointer address_length_pointer
  //从监听队列里取出一个连接，为他创建一个专属的socket文件描述符
  if(client_socket<0){
    perror("accept failed");
    close(server_fd);
    return 1;
  }
  std::cout<<"[Server] Connection accepted"<<std::endl;

  //---开始与客户端通话---
  char buffer[BUFFER_SIZE]={0};
  ssize_t bytes_read=read(client_socket,buffer,BUFFER_SIZE-1);
  if(bytes_read<0){
    perror("read failed");
  }else if(bytes_read==0){
    std::cout<<"[Server] Clinet disconnected"<<std::endl;
  }else{
    buffer[bytes_read]='\0';
    std::cout<<"[Server] Message received from client "<<buffer<<std::endl;

    //5.回声 把收到的数据原封不动发回
    send(client_socket,buffer,bytes_read,0);
    std::cout<<"[Server] Echo sent back to client"<<std::endl;
  }

  //6.关闭连接释放资源
  close(client_socket);
  close(server_fd);
  std::cout<<"[Server] Sockets closed"<<std::endl; 
  return 0;
}


//只能处理一个客户端连接，之后会用循环和多线程来处理多个客户端