from scheduler import Scheduler

scheduler = Scheduler(total_pes=16, dram_bw=4, dram_latency=200, sram_cap_words=8192)

M, N, K = 32, 128, 64

tile,cycles = scheduler.schedule_matmul_cycles(M,N,K)

print("Best tile (TM, TN, TK):", tile)
print("Estimated cycles: ",cycles)