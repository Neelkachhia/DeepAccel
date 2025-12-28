import numpy as np

def im2col(input, KH, KW, stride=1):
  N, H, W, C = input.shape
  OH = (H - KH) // stride + 1
  OW = (W - KW) // stride + 1

  cols = []
  for n in range(N):
    for i in range(0, H-KH+1, stride):
      for j in range(0, W-KW+1, stride):
        patch = input[n, i:i+KH, j:j+KW, :].flatten()
        cols.append(patch)

  return np.array(cols)

x = np.random.rand(1,5,5,3)
X_col = im2col(x,3,3)
print(X_col.shape)