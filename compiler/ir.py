class IRNode:
  def __init__(self,name,shape):
    self.name = name
    self.shape = shape

    self.inputs = []
    self.outputs = []

    self.schedule = None
    self.estimated_cycles = None
  
  def add_input(self,node):
    self.inputs.append(node)
    node.outputs.append(self)

  def set_cost(self,cycles):
    self.estimated_cycles = cycles

  def __repr__(self):
    return f"{self.name}(shape={self.shape}, cycles = {self.estimated_cycles})"