from DeepAccel.compiler.ops import MatMul, ReLU, FusedMatMulReLU

def fuse_matmul_relu(graph):
  new_nodes = []
  skip = set()

  for node in graph.nodes:
    if node in skip:
      continue

    if isinstance(node, MatMul) and len(node.outputs) == 1:
      out = node.outputs[0]

      if isinstance(out, ReLU):
        fused = FusedMatMulReLU(name = f"{node.name}_relu_fused", 
                                A_shape=node.A_shape,
                                B_shape=node.B_shape
                                )
        for inp in node.inputs:
          fused.add_input(inp)
        
        for o in out.outputs:
          o.inputs.remove(out)
          o.add_input(fused)
        
        new_nodes.append(fused)
        skip.add(node)
        skip.add(out)
        continue

    new_nodes.append(node)
    
  graph.nodes = new_nodes
                                