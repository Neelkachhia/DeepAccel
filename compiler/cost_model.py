from DeepAccel.compiler.ops import MatMul, ReLU

class CostAnnotator:
  def __init__(self,scheduler):
    self.scheduler = scheduler

  def annotate(self,graph):
    for node in graph.nodes:
      if isinstance(node,MatMul):
        M,N = node.shape
        K = node.A_shape[1]

        tile,cycles = self.scheduler.schedule_matmul(M,N,K)
        node.schedule = tile
        node.set_cost(cycles)

      elif isinstance(node,ReLU):
        node.schedule = None
        node.set_cost(node.macs())