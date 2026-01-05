class Graph:
  def __init__(self):
    self.nodes = []
  
  def add_node(self,node):
    self.nodes.append(node)

  def total_macs(self):
    return sum(node.macs() for node in self.nodes)

  def dump(self):
    print("=== Graph ===")
    for node in self.nodes:
      print(node)
      for out in node.outputs:
        print(f"    -> {out.name}")