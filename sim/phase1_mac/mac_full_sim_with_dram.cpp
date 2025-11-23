#include<iostream>
#include<queue>
#include <utility>
using namespace std;

struct DramRequest
{
  long long complete_cycle;
};

int main()
{
  int rows = 4, cols = 4;
  int total_pes = rows * cols;
  long long total_macs = 500;
  int pipeline_depth = 4;
  int sram_ports = 8;
  int sram_latency = 2;
  int dram_latency = 200; //cycles for first word
  int dram_bw = 4; //words per cycle (32-bit words)
  int burst_words = 16; //fetch 16 words in one burst
  int word_per_mac = 2; //each MAC needs 2 words (A and B)

  long long cycle = 0;
  long long completed = 0;
  long long in_pipeline = 0;
  long long reads_total = total_macs * word_per_mac;
  long long reads_issued = 0;

  queue<long long> sram_read_complete;
  queue<pair<long long, int>> mac_finish;
  queue<DramRequest> dram_queue;

  long long sram_reads_completed = 0;
  long long dram_reads_completed = 0;

  bool sram_has_data = false; //SRAM starts empty

  while(completed < total_macs)
  {
    cycle++;

    // (1) Process completed DRAM fetches
    while(!dram_queue.empty() && dram_queue.front().complete_cycle <= cycle)
    {
      dram_queue.pop();
      sram_has_data = true;
    }

    // (2) Process completed SRAM reads
    while(!sram_read_complete.empty() && sram_read_complete.front() <= cycle)
    {
      sram_read_complete.pop();
      sram_reads_completed++;
    }

    // (3) Process Completed MACs
    while(!mac_finish.empty() && mac_finish.front().first <= cycle)
    {
      mac_finish.pop();
      completed++;
      in_pipeline--;
    }

    // (4) Issue SRAM reads IF SRAM has the Data (no DRAM miss)
    if(!sram_has_data)
    {
      //SRAM miss -> issue DRAM fetch if not already fetching
      if(dram_queue.empty())
      {
        long long transfer_time = burst_words / dram_bw;
        long long complete_at = cycle + dram_latency + transfer_time;
        dram_queue.push({complete_at});
      }
      else
      {
        //SRAM Hit
        int ports = sram_ports;
        while(ports > 0 && reads_issued < reads_total)
        {
          long long complete_at = cycle + sram_latency;
          sram_read_complete.push(complete_at);
          reads_issued++;
          ports--;
        }
      }
    }

    // (5) issue MACs if enough data available
    // we need 2 reads completed per MAC
    long long macs_possible = sram_reads_completed / 2;
    long long macs_remaining = total_macs - completed - in_pipeline;
    long long macs_to_issue = std::min(macs_possible, std::min((long long) total_pes, macs_remaining));

    for(int i=0; i<macs_to_issue; i++)
    {
      long long finish_at = cycle + pipeline_depth;
      mac_finish.push({finish_at,0});
      in_pipeline++;
      sram_reads_completed -= 2;
    }
  }

  cout << "Total Cycles = " << cycle << endl;
  return 0;
}