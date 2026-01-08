class GraphExecutionModel:
  def __init__(self, dram_latency):
    self.dram_latency = dram_latency
    
  def estimate(self, graph):
    compute_cycles = [
      node.estimated_cycles
      for node in graph.nodes
      if node.estimated_cycles is not None]
        

    if not compute_cycles:
      return 0
    
    # warmup = self.dram_latency
    # steady_state = max(compute_cycles)
    # drain = min(compute_cycles)

    return self.dram_latency + max(compute_cycles)
  