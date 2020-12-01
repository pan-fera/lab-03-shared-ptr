// Copyright 2020 Your Name <your_email>

#include "SharedPtr.hpp"
int main(){
std::cout<<"ds"<< std::endl;
SharedPtr<int> A;

  int a = 5;
  int *s = (int*) malloc(sizeof(int));
  *s=a;
  SharedPtr<int> B(s);
  A=B;
  A=A;
return 0;
}
