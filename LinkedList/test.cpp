#include "list.h"
#include <iostream>
using std::cout;

int main() {
  list<int> l;

  for (int i = 0; i < 5; i++)
    l.push_back(i);


  auto b = l.begin();
  while (b != l.cend()){
    cout << *b << " ";
    ++b;
  }
  cout << std::endl;
}
