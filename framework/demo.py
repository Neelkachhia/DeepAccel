import numpy as np
from tensor import Tensor
from ops import MatMul, Conv2D
from graph import Graph

A = Tensor(np.random.rand(32,64), name="A")
B = Tensor(np.random.rand(64,128), name="B")

mm = MatMul(A,B,name="matmul1")

X = Tensor(np.random.rand(1,5,5,3), name="input")
W = Tensor(np.random.rand(3,3,3,8), name="weights")

conv = Conv2D(X,W,name="conv1")

g = Graph()
g.add(mm)
g.add(conv)

outs = g.run()

print("Outputs: ")
for t in outs:
  print(t)

print("Total MACs in graph : ", g.total_macs())