#include <iostream>
#include <deque>

using std::endl;
using std::cout;
using std::cin;
using std::deque;

void bf(int n, int k);

int main(void) {

  bf(3, 4);
  return 0;

}

void bf(int n, int k) {
  deque<int> s;
  do {
    // fill the rest with zero
    while(s.size() < n)
      s.push_back(0);
    
    // This is the solution
    for (auto e : s)
      cout << e << " ";
    cout << endl;
    
    int t = s.back();
    s.pop_back();
    while (!s.empty() && t == k-1) {
      t = s.back();
      s.pop_back();
    }
    
    if (t<k-1)
      s.push_back(t + 1);
  } while (!s.empty());
}