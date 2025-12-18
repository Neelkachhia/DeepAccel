#include<iostream>
#include<queue>
#include<algorithm>
#include<fstream>

using namespace std;

struct DramReq
{
  long long complete_cycle;
};

int main()
{
  // Accelerator
  int rows = 4, cols = 4;
  int total_pes = rows * cols;
  int pipeline_depth = 4;

  // SRAM
  int sram_ports = 8;
  int sram_latency = 2;

  // DRAM
  int dram_latency = 200;
  int dram_bw = 4;
  int burst_words = 16;

  // Tiling
  int tile_m = 128, tile_n = 128;
  int macs_per_tile = tile_m * tile_n;

  // Problem Size
  int total_m = 256, total_n = 256;
  int total_tiles = (total_m / tile_m) * (total_n / tile_n);

  // Energy constants
  double ENERGY_PER_MAC = 1.0;
  double ENERGY_SRAM_READ = 0.2;
  double ENERGY_DRAM_WORD = 20.0;

  // Energy Counters
  long long mac_count = 0;
  long long sram_read_count = 0;
  long long dram_word_count = 0;

  // Simulation State
  long long cycle = 0;
  int current_tile = 0;
  long long tile_macs_completed = 0;
  long long in_pipeline = 0;
  long long sram_reads_completed = 0;

  queue<long long> sram_read_complete;
  queue<pair<long long, int>> mac_finish;
  queue<DramReq> dram_queue;

  bool tile_loaded = false;
  bool tile_compute_phase = false; //explicit tile execution state
  bool dram_inflight = false; //tracking dram request in flight

  while(current_tile < total_tiles)
  {
    cycle++;

    // (1) DRAM completion
    if(!dram_queue.empty() && dram_queue.front().complete_cycle <= cycle)
    {
      dram_queue.pop();
      tile_loaded = true;
      tile_compute_phase = true;
      dram_inflight = false;
    }

    // (2) SRAM read completion
    while(!sram_read_complete.empty() && sram_read_complete.front() <= cycle)
    {
      sram_read_complete.pop();
      sram_reads_completed++;
    }

    // (3) MAC completion
    while(!mac_finish.empty() && mac_finish.front().first <= cycle)
    {
      mac_finish.pop();
      tile_macs_completed++;
      in_pipeline--;
    }

    
    // (4) Tile finished
    if(tile_compute_phase && tile_macs_completed == macs_per_tile)
    {
      current_tile++;
      tile_macs_completed = 0;
      sram_reads_completed = 0;
      in_pipeline = 0;
      tile_loaded = false;
      tile_compute_phase = false;
      dram_inflight = false;
      continue;
    }

    // (5) DRAM fetch
    if(!tile_loaded && !dram_inflight)
    {
      long long transfer_time = burst_words / dram_bw;
      long long complete_at = cycle + dram_latency + transfer_time;
      dram_queue.push({complete_at});
      dram_word_count += burst_words;
      dram_inflight = true;
    }

    // (6) SRAM reads
    if(tile_compute_phase)
    {
      int ports = sram_ports;
      while(ports > 0 && sram_reads_completed < macs_per_tile * 2)
      {
        sram_read_complete.push(cycle + sram_latency);
        sram_read_count++;
        ports--;
      }
    }

    // (7) Issue MACs
    long long macs_ready = sram_reads_completed / 2;
    long long macs_left = macs_per_tile - tile_macs_completed - in_pipeline;
    long long macs_to_issue = min({macs_ready,(long long)total_pes, macs_left});
    
    for(int i=0; i<macs_to_issue; i++)
    {
      mac_finish.push({cycle + pipeline_depth,0});
      in_pipeline++;
      sram_reads_completed -= 2;
      mac_count++;
    }
  }
  
    double total_energy = (mac_count * ENERGY_PER_MAC) + (sram_read_count * ENERGY_SRAM_READ) + (dram_word_count * ENERGY_DRAM_WORD);

    cout << "Total cycles = " << cycle << endl;
    cout << "Total MACs = " << mac_count << endl;
    cout << "SRAM reads = " << sram_read_count << endl;
    cout << "DRAM words = " << dram_word_count << endl;
    cout << "Total energy = " << total_energy << endl;
    cout << "Energy / MAC = " << total_energy / mac_count << endl;

    ofstream out("results.csv", ios::app);

    out << tile_m << " , "
        << tile_n << " , "
        << cycle << " , "
        << mac_count << " , "
        << sram_read_count << " , "
        << dram_word_count << " , "
        << total_energy << " , "
        << (total_energy / mac_count) << " \n ";

    out.close();

  return 0;
}