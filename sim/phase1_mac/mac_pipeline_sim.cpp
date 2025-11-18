#include<iostream>
using namespace std;

int main()
{
  int total_macs = 10;

  //No. of cycles needed for all stages in pipeline... S0 : fetch(1), S1 : Multiply(2), S2 : Add(1), S3 : Write back(1) 
  int pipeline_depth = 5;

  int II = 1; // II = Initiation Interal (basically cycles needed between starting new operations)

  int total_cycles = pipeline_depth + (total_macs - 1) * II;

  cout << "Total MAC Operations : " << total_macs << endl;
  cout << "Pipeline Depth : " << pipeline_depth << "\n";
  cout << "Initiation Interval : " << II << "\n";
  cout << "Total Cycles : " << total_cycles << endl;

  return 0;

}