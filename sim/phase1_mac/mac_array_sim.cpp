#include<iostream>
#include<cmath>
using namespace std;

int main()
{
  int rows = 4;
  int cols = 4;

  int total_macs = 64;
  int cycles_per_mac = 1; // 1 MAC per cycle per PE

  int total_pes = rows * cols; // pe = processing elements, No. of PEs in the matrix(array)
  int macs_per_cycle = total_pes * (1 / cycles_per_mac);
  int total_cycles = ceil((double) total_macs / total_pes);

  cout << "MAC Array Size : " << rows << " X " << cols << endl;
  cout << "Total PEs : " << total_pes << endl;
  cout << "Total MAC Operations : " << total_macs << endl;
  cout << "Cycles Needed : " << total_cycles << endl;

  return 0;
}
