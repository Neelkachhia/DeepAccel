class Graph:
  def __init__(self):
    self.nodes = []
  
  def add_node(self,node):
    self.nodes.append(node)

  def total_macs(self):
    return sum(node.macs() for node in self.nodes)
  
  def total_cycles(self):
    return sum(
      node.estimated_cycles
      for node in self.nodes
      if node.estimated_cycles is not None
    )

  def dump(self):
    print("=== Graph ===")
    for node in self.nodes:
      print(node)
      for out in node.outputs:
        print(f"    -> {out.name}")