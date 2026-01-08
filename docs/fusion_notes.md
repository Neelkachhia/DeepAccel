## Operator Fusion (Step 10.3)

- Implemented MatMul + ReLU fusion
- Removed intermediate activation tensor
- Preserved MAC count
- Reduced estimated cycles from ~36k to ~32k
- Demonstrates compiler-level optimization
