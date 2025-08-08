cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建自己的库
add_library(calculator STATIC 
  src/calculator.cpp
)

target_include_directories(calculator PUBLIC 
  "${PROJECT_SOURCE_DIR}/include"
) # PROJECT_SOURCE_DIR由project函数创造，指向项目的根目录，使用优先级大于MyProject_SOURCE_DIR
# 选择PUBILC，任何连接calculator的目标都自动继承这个路径，就都能连接到头文件


# file(GLOB SRC_FILES "src/*.cpp")
# 这个命令是GLOB下找到src内的cpp然后把他们赋值给SRC_FILE，后面才能直接用

add_executable(app_with_lib src/main.cpp)

target_link_libraries(app_with_lib PRIVATE calculator)
# 选择PRIVATE 就是仅自己用

# 如果选择INTERFACE就是给别人用 



# 查找并继承第三方库
find_package(Threads REQUIRED)

add_executable(app_with_threads src/main_threads.cpp)

target_link_libraries(app_with_threads PRIVATE Threads::Threads)

# intro(unsafe)
add_executable(multi_threading_intro src/multi_threading_intro.cpp)

target_link_libraries(multi_threading_intro PRIVATE Threads::Threads)

# safe
add_executable(multi_threading_safe src/multi_threading_safe.cpp)

target_link_libraries(multi_threading_safe PRIVATE Threads::Threads)


# 生产者和消费者
add_executable(producer_consumer src/producer_consumer.cpp)

target_link_libraries(producer_consumer PRIVATE Threads::Threads)

# 原子操作
add_executable(atomic_counter src/atomic_counter.cpp)
target_link_libraries(atomic_counter PRIVATE Threads::Threads)

# 异步
add_executable(async_future src/async_future.cpp)
target_link_libraries(async_future PRIVATE Threads::Threads)

# 承诺未来模型
add_executable(manual_future src/manual_future.cpp)
target_link_libraries(manual_future PRIVATE Threads::Threads)

# fork()
add_executable(fork_intro src/fork_intro.cpp)
# 无需连接线程库，fork是核心系统调用，无需额外的库

# 管道
add_executable(pipe_ipc src/pipe_ipc.cpp)

# 共享内存
add_executable(shm_writer src/shm_writer.cpp)
add_executable(shm_reader src/shm_reader.cpp)
target_include_directories(shm_writer PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(shm_reader PUBLIC "${PROJECT_SOURCE_DIR}/include")

# client server
add_executable(tcp_echo_server src/tcp_echo_server.cpp)
add_executable(tcp_echo_client src/tcp_echo_client.cpp)

add_executable(tcp_concurrent_server src/tcp_concurrent_server.cpp)
target_link_libraries(tcp_concurrent_server PRIVATE Threads::Threads)

# chatroom
add_executable(tcp_chatroom_server src/tcp_chatroom_server.cpp)
target_link_libraries(tcp_chatroom_server PRIVATE Threads::Threads)
add_executable(tcp_chatroom_client src/tcp_chatroom_client.cpp)
target_link_libraries(tcp_chatroom_client PRIVATE Threads::Threads)