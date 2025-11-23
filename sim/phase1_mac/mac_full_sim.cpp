#include<iostream>
#include<queue>
#include<vector> //for future use
#include<cmath>
using namespace std;

int main()
{
  int rows = 4;
  int cols = 4;
  int total_pes = rows * cols;
  int total_macs = 500;
  int pipeline_depth = 4; //pipeline stages
  int II = 1;
  int sram_read_latency = 2; // cycles needed to fetch one operand from cache(SRAM)
  int sram_ports = 8; //how many concurrent reads SRAM can serve per cycle

  // Each MAC needs 2 SRAM reads (activation + weight)
  // We model a queue of pending SRAM read requests and process sram_ports per cycle.

  long long cycle = 0;
  long long completed = 0;
  long long in_pipeline = 0; //count of MACs currently in pipeline

  queue<long long> read_completion_time; //a queue to model read completion time

  // we'll also need counter for available PEs per cycle
  // at most total_pes new MACs can be issued per cycle
  // we'll issue upto min(available_pes, issued_by_II) new MACs each cycle if data is ready
  // to model data readiness, we track how many MACs have both reads completed and are waiting to be issued.

  long long ready_to_issue = 0;
  long long issued_total = 0;

  int max_start_per_cycle = total_pes;

  long long reads_total = (long long) total_macs * 2; //because each MACs require 2 reads

  long long reads_issued = 0;

  // pipeline completion tracking : each MAC, when issued, will finish after pipeline_depth cycles

  queue<long long> mac_finish_time;

  while(completed < total_macs)
  {
    cycle++;
    // (1) completes SRAM reads in this cycle


    // read_completion_time contains completion cycles for previously issued reads
    // move all reads that complete at this cycle into ready pool

    long long reads_completed_this_cycle = 0;

      while(!read_completion_time.empty() && read_completion_time.front() <= cycle)
      {
        read_completion_time.pop();
        reads_completed_this_cycle++;
      }

      static long long leftover_reads = 0; //leftover reads which haven't made a full pair yet

      leftover_reads += reads_completed_this_cycle;
      long long newly_ready_macs = leftover_reads / 2;

      if(newly_ready_macs > 0)
      {
        ready_to_issue += newly_ready_macs;
        leftover_reads %= 2; 
      }

      //(2)completes MAC pipeline operation

      while(!mac_finish_time.empty() && mac_finish_time.front() <= cycle)
      {
        mac_finish_time.pop();
        completed++;
        in_pipeline--;
      }

      // (3) issue new SRAM reads in this cycle upto sram_ports

      int ports_available = sram_ports;
      while(ports_available > 0 && reads_issued < reads_total)
      {
        read_completion_time.push(cycle + sram_read_latency);
        reads_issued++;
        ports_available--;
      }

      // (4) issue new MACs if data is ready and pipeline/PEs have capacity so issue upto min(ready_to_issue, max_start_per_cycle, remaining_macs)

      long long remaining_macs = total_macs - issued_total;
      long long can_issue = min<long long>(ready_to_issue, min<long long>(max_start_per_cycle, remaining_macs));

      for(long long i=0; i<can_issue; ++i)
      {
        mac_finish_time.push(cycle + pipeline_depth);
        in_pipeline++;
        issued_total++;
      }

      ready_to_issue -= can_issue;

      //if no reads left to issue and no ready to issue MACs, we still might be waiting for pipeline to drain so loop continues untill completed == total_macs
  }

  cout << "Simulation : \n";
  cout << "PE array : " << rows << " X " << cols << "(" << total_pes << " PEs) \n";
  cout << "Total MAC ops : " << total_macs << "\n";
  cout << "Pipeline depth : " << pipeline_depth << " cycles \n";
  cout << "SRAM read latency : " << sram_read_latency << " cycles \n";
  cout << "SRAM ports : " << sram_ports << "\n";
  cout << "Total Cycles : " << cycle << "\n";

  // if clock_freq = 0.5 GHz then cycle_time = 2 ns... but here i'm not assuming clock_freq.....
  
  return 0;

  /*to reduce clock cycles,

    increase sram_ports (more concurrent reads)
    reduce sram_read_latency
    increase rows/cols(more PEs) upto memory limits
    
  */
}