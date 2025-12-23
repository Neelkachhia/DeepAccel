import numpy as np
from tensor import Tensor

class Op:
  def forward(self):
    raise NotImplementedError
  
  def macs(self):
    """Return number of MACs (for cost modelling)"""
    raise NotImplementedError
  
class MatMul(Op):
  def __init__(self, A: Tensor, B: Tensor, name=None):
    assert A.shape[1] == B.shape[0]
    self.A = A
    self.B = B
    self.name = name

  def forward(self):
    out = self.A.data @ self.B.data
    return Tensor(out, name=self.name)
  
  def macs(self):
    M, K = self.A.shape
    _, N = self.B.shape
    return M * N * K
  
class Conv2D(Op):
  def __init__(self, X: Tensor, W: Tensor, stride=1, name=None):
    """
    X : (N, H, W, C)
    W : (KH, KW, C, F)
    """

    self.X = X
    self.W = W
    self.stride = stride
    self.name = name

  def im2col(self):
    X = self.X.data
    W = self.W.data
    N, H, Wd, C = X.shape
    KH, KW, _, F = W.shape

    OH = (H-KH) // self.stride + 1
    OW = (Wd-KW) // self.stride+1

    cols = []

    for n in range(N):
      for i in range(0, H-KH+1, self.stride):
        for j in range(0, Wd-KW+1, self.stride):
          patch = X[n, i:i+KH, j:j+KW, :].flatten()
          cols.append(patch)
    
    X_col = np.array(cols)
    W_col = W.reshape(-1, F)

    return X_col, W_col, (N, OH, OW, F)
  
  def forward(self):
    X_col, W_col, out_shape = self.im2col()
    out = X_col @ W_col
    return Tensor(out.reshape(out_shape), name=self.name)
  
  def macs(self):
    X = self.X.shape
    W = self.W.shape
    N, H, Wd, C = X
    KH, KW, _, F = W

    OH = (H-KH) // self.stride+1
    OW = (Wd-KW) // self.stride+1

    return N*OH*OW*(KH*KW*C) * F