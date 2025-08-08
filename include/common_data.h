#ifndef COMMON_DATA_H
#define COMMON_DATA_H

const key_t SHM_KEY=1234;
//SHM是shared memory 这是一个唯一key，用于writer和rader找到同一块共享内存
//你是说这个其实可以自己用fork生成？
const key_t SEM_KEY=5678;//这个给信号量用


struct SharedData{
  char name[50];
  int age;
  bool data_ready;//标志位，表示数据是否写好
};
#endif