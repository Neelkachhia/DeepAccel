#include<iostream>
#include<queue>
using namespace std;

struct DramReq
{
  long long complete_cycle;
};

int main()
{
  // Accelerator Parameters
  int rows = 4, cols = 4;
  int total_pes = rows * cols;

  int pipeline_depth = 4;

  // SRAM performance
  int sram_ports = 8;
  int sram_latency = 2;

  // DRAM performance
  int dram_latency = 200;
  int dram_bw = 4;
  int burst_words = 16;

  // Tiling parameters
  int tile_m = 64;
  int tile_n = 64;
  int macs_per_tile = tile_m * tile_n;

  // Total Problem Size 
  int total_m = 256;
  int total_n = 256;

  // Number of tiles
  int total_tiles = (total_m / tile_m) * (total_n / tile_n);

  // simulation state
  long long cycle = 0;
  int current_tile = 0;
  long long tile_macs_comleted = 0;
  long long in_pipeline = 0;
  long long sram_reads_completed = 0;

  queue<long long> sram_read_complete;
  queue<pair<long long, int>> mac_finish;
  queue<DramReq> dram_queue;

  bool tile_loaded = false;

  while(current_tile < total_tiles)
  {
    cycle++;

    // (1) DRAM tile fetch completion
    if(!dram_queue.empty() && dram_queue.front().complete_cycle <= cycle)
    {
      dram_queue.pop();
      tile_loaded = true;
    }

    // (2) SRAM read completion
    while (!sram_read_complete.empty() && sram_read_complete.front() <= cycle)
    {
      sram_read_complete.pop();
      sram_reads_completed++;
    }

    // (3) MAC pipeline completion
    while(!mac_finish.empty() && mac_finish.front().first <= cycle)
    {
      mac_finish.pop();
      tile_macs_comleted++;
      in_pipeline--;
    }

    // (4) if tile finished -> reset state
    if(tile_macs_comleted == macs_per_tile)
    {
     current_tile++;
     tile_macs_comleted = 0;
     sram_reads_completed = 0;
     tile_loaded = false;
     continue;
    }

    // (5) fetch tile from DRAM if not loaded
    if(!tile_loaded && dram_queue.empty())
    {
     long long transfer_time = burst_words / dram_bw;
     long long complete_at = cycle + dram_latency + transfer_time;
     dram_queue.push({complete_at});
    }
    

    // (6) Issue SRAM reads
    if(tile_loaded)
    {
      int ports = sram_ports;
      while(ports > 0 && sram_reads_completed < macs_per_tile * 2)
      {
        sram_read_complete.push(cycle + sram_latency);
        ports--;
      }
    }

    // (7) Issue MACs
    long long macs_ready = sram_reads_completed / 2;
    long long macs_left = macs_per_tile - tile_macs_comleted - in_pipeline;
    long long macs_to_issue = std::min(macs_ready, std::min((long long)total_pes, macs_left));

    for(int i=0; i<macs_to_issue; i++)
    {
      mac_finish.push({cycle + pipeline_depth, 0});
      in_pipeline++;
      sram_reads_completed -= 2;
    }
  }

  cout << "Total cycles with tiling = " << cycle << endl;
  return 0;
}