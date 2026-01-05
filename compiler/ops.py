from ir import IRNode

class MatMul(IRNode):
  def __init__(self, name, A_shape, B_shape):
    M,K1 = A_shape
    K2,N = B_shape
    assert K1 == K2, "Invalid MatMul shapes"
    super().__init__(name, (M,N))
    self.A_shape = A_shape
    self.B_shape = B_shape
  
  def macs(self):
    M,N = self.shape
    K = self.A_shape[1]
    return M*N*K
  
class ReLU(IRNode):
  def __init__(self, name, input_shape):
    super().__init__(name, input_shape)

  def macs(self):
    total = 1
    for d in self.shape:
      total *= d
    return total