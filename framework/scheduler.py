class Scheduler:
  def __init__(self, total_pes, dram_bw, dram_latency, sram_cap_words=8192):
    self.total_pes = total_pes
    self.dram_bw = dram_bw
    self.dram_latency = dram_latency
    self.sram_cap_words = sram_cap_words
  
  def estimate_matmul_cycles_db(self,M,N,K,TM,TN,TK):
    #full tile MACs
    macs = TM * TN * TK
    compute_cycles = macs/self.total_pes

    #DRAM Transfer only(latency hidden after warmup)(warmup ---> first tile pays the latency)
    dram_words = TM * TK + TK * TN
    dram_transfer_cycles = dram_words / self.dram_bw

    # SRAM capacity constraint
    sram_words = TM * TK + TK * TN + TM * TN
    if sram_words > self.sram_cap_words:
      return float("inf")

    #Double-buffered model
    return max(compute_cycles, dram_transfer_cycles) + self.dram_latency

  def schedule_matmul(self,M,N,K):
    best = None
    best_cycles = float("inf")

    for TM in [8,16,32,64]:
      for TN in [8,16,32,64]:
        for TK in [8,16,32,64]:
          if TM > M or TN > N or TK > K:
            continue
          cycles = self.estimate_matmul_multitile(M,N,K,TM,TN,TK)
          if cycles < best_cycles:
            best_cycles = cycles
            best = (TM, TN, TK)
    return best, best_cycles
  
#step 9.10

  def estimate_matmul_multitile(self, M, N, K, TM, TN, TK):
    tiles_m = M // TM
    tiles_n = N // TN
    tiles_k = K // TK

    # SRAM capacity constraint
    sram_words = TM * TK + TK * TN + TM * TN
    if sram_words > self.sram_cap_words:
        return float("inf")

    # ---- DRAM cost (once per MxN tile) ----
    dram_words_per_mn = TM * TK + TK * TN
    dram_cycles_per_mn = dram_words_per_mn / self.dram_bw
    dram_total = self.dram_latency + (tiles_m * tiles_n * dram_cycles_per_mn)

    # ---- Compute cost (per K tile) ----
    compute_per_k = (TM * TN * TK) / self.total_pes
    compute_total = tiles_m * tiles_n * tiles_k * compute_per_k

    return max(dram_total, compute_total)

          