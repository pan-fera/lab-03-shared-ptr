// Copyright 2020 Your Name <your_email>

#include "../include/SharedPtr.hpp"
int main(){
std::cout<<"ds"<< std::endl;
SharedPtr<int> A;

  int a = 5;
  int *s = (int*) malloc(sizeof(int));
  *s=a;
  SharedPtr<int> B(s);
  A=B;
  A=A;
  std::cout<<A.use_count();
return 0;
}
