#include<iostream>
using namespace std;

int main()
{
  int N = 10; //Number of MAC(Multiply Accumulate) Operations
  int cycles_per_mac = 2; //Assuming 2 cycles for multiply + add
  int total_cycles = N * cycles_per_mac;

  cout << "MAC Operations : " << N << endl;
  cout << "Cycles per MAC : " << cycles_per_mac << endl;
  cout << "Total Cycles : " << total_cycles << endl;

  return 0;

}