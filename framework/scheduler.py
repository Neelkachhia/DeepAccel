class Scheduler:
  def __init__(self, total_pes, dram_bw, dram_latency, sram_cap_words=8192):
    self.total_pes = total_pes
    self.dram_bw = dram_bw
    self.dram_latency = dram_latency
    self.sram_cap_words = sram_cap_words
  
  def estimate_matmul_cycles_full(self,M,N,K,TM,TN,TK):
    #MACs for full tile
    macs = TM * TN * TK
    compute_cycles = macs/self.total_pes

    #DRAM Traffic
    dram_words = TM * TK + TK * TN
    dram_cycles = self.dram_latency + (dram_words / self.dram_bw)

    # SRAM capacity constraint
    sram_words = TM * TK + TK * TN + TM * TN
    if sram_words > self.sram_cap_words:
      return float("inf")

    return max(compute_cycles, dram_cycles)

  def schedule_matmul_cycles(self,M,N,K):
    best = None
    best_cycles = float("inf")

    for TM in [8,16,32,64]:
      for TN in [8,16,32,64]:
        for TK in [8,16,32,64]:
          if TM > M or TN > N or TK > K:
            continue
          cycles = self.estimate_matmul_cycles_full(M,N,K,TM,TN,TK)
          if cycles < best_cycles:
            best_cycles = cycles
            best = (TM, TN, TK)
    return best, best_cycles
          