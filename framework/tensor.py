import numpy as np

class Tensor:
  def __init__(self, data, name=None):
    self.data = np.array(data)
    self.shape = self.data.shape
    self.name = name

  def __repr__(self):
    return f"Tensor(name={self.name}, shape={self.shape})"