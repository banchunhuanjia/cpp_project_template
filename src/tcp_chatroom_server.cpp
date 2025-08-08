#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

std::mutex mtx;
std::vector<int> clients;
const int PORT=8080;
const int BUFFER_SIZE=1024;

void handle_client(int client_socket){
  sockaddr_in client_addr;
  socklen_t client_addr_len=sizeof(client_addr);
  getpeername(client_socket,(sockaddr*)&client_addr,&client_addr_len);
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip));
  std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Handling client form "
    <<client_ip<<":"<<ntohs(client_addr.sin_port)<<std::endl;
  {
    std::lock_guard<std::mutex> lock(mtx);
    clients.push_back(client_socket);
  }//lock_guard不能手动unlock
  char buffer[BUFFER_SIZE]={0};
  ssize_t bytes_read;
  while((bytes_read=read(client_socket,buffer,BUFFER_SIZE-1))>0){
    std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Received "<<buffer<<std::endl;
    buffer[bytes_read]='\0';
    //这两项应该在消息广播前
    {
      std::lock_guard<std::mutex> lock(mtx);
      for(auto &client:clients){
        if(client==client_socket)continue;
        send(client,buffer,bytes_read,0);
      }
    }
  }
  if(bytes_read==0){
    std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Client disconnected"<<std::endl;
  }else{
    perror("read failed");
  }
  {
    std::lock_guard<std::mutex> lock(mtx);
    //std::move(clients.begin(),client.end(),client_socket);
    for(auto it=clients.begin();it!=clients.end();){
      if(*it==client_socket){
        it=clients.erase(it);
        break;
      }else{
        ++it;
      }
    }
    std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Removed client from list" 
      <<clients.size()<<" clients remaining"<<std::endl;
  }
  close(client_socket);//先在列表中删掉在close
  std::cout<<"[Thread "<<std::this_thread::get_id()<<"] Connection closed"<<std::endl;
}


int main(){
  int server_fd=socket(AF_INET,SOCK_STREAM,0);
  if(server_fd<0){
    perror("socket failed");
    return 1;
  }
  std::cout<<"[Server] Socket created successfully"<<std::endl;
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
    perror("linsten failed");
    close(server_fd);
    return 1;
  }
  std::cout<<"[Main Thread] Server is listening on port"<<PORT<<"..."<<std::endl;
  //我们选择用detach来处理join的问题了，所以不用创建线程数组
  while(true){
     std::cout<<"[Main Thread] Waiting for a new connection..."<<std::endl;
     int client_socket=accept(server_fd,nullptr,nullptr);
     if(client_socket<0){
      perror("accept failed");
      continue;
     }
     std::cout<<"[Main Thread] Accepted a new connection"<<std::endl;
     std::thread(handle_client,client_socket).detach();
  }
  close(server_fd);
  return 0;
}