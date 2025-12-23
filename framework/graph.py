class Graph:
  def __init__(self):
    self.ops = []

  def add(self,op):
    self.ops.append(op)

  def run(self):
    outputs = []
    for op in self.ops:
      out = op.forward()
      outputs.append(out)
    return outputs
  
  def total_macs(self):
    return sum(op.macs() for op in self.ops)