from ops import MatMul,ReLU
from graph import Graph

g = Graph()

mm1 = MatMul("mm1", (32,64), (64,128))
relu1 = ReLU("relu1", (32,128))
mm2 = MatMul("mm2", (32,128), (128,64))

relu1.add_input(mm1)
mm2.add_input(relu1)

g.add_node(mm1)
g.add_node(relu1)
g.add_node(mm2)

g.dump()
print("Total MACs in graph : ",g.total_macs())