#include<iostream>
#include"calculator.h"

int main(){
  int x=10;
  int y=5;
  std::cout<<"Starting calculator..."<<std::endl;
  std::cout<<x<<"+"<<y<<"="<<add(x,y)<<std::endl;
  std::cout<<x<<"-"<<y<<"="<<subtract(x,y)<<std::endl;

  std::cout<<"Finished"<<std::endl; 
  return 0;
}