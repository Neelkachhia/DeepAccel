from DeepAccel.compiler.ops import MatMul,ReLU
from DeepAccel.compiler.graph import Graph
from DeepAccel.compiler.cost_model import CostAnnotator
from DeepAccel.framework.scheduler import Scheduler
from DeepAccel.compiler.fusion import fuse_matmul_relu

scheduler = Scheduler(
  total_pes = 16,
  dram_bw = 4,
  dram_latency = 200
)


annotator = CostAnnotator(scheduler)

g = Graph()


mm1 = MatMul("mm1", (32,64), (64,128))
relu1 = ReLU("relu1", (32,128))
mm2 = MatMul("mm2", (32,128), (128,64))

relu1.add_input(mm1)
mm2.add_input(relu1)

g.add_node(mm1)
g.add_node(relu1)
g.add_node(mm2)

print("Before fusion:")
g.dump()

fuse_matmul_relu(g)

print("\n after fusion: ")
g.dump()

annotator.annotate(g)

g.dump()
print("Total MACs in graph : ",g.total_macs())
print("Total estimated cycles : ",g.total_cycles())