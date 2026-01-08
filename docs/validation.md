## Important Note on Workload Size

The Python compiler and C++ simulator were evaluated on
different absolute workload sizes.

- Python compiler models the full graph execution
  (524,288 MACs).
- C++ simulator models a single tiled execution
  (65,536 MACs).

Therefore, absolute cycle counts are not compared directly.
Instead, normalized metrics are used.

## Normalized Comparison

| Model | Total MACs | Cycles | Cycles / MAC |
|------|-----------|--------|--------------|
| C++ Simulator | 65,536 | 17,221 | 0.2627 |
| Python Compiler | 524,288 | 16,584 | 0.0316 |

## Analysis

The Python compiler assumes ideal pipelined execution and
perfect overlap between operators after warm-up. As a result,
it reports significantly lower cycles per MAC.

The C++ simulator explicitly models SRAM and DRAM accesses,
pipeline fill/drain effects, and conservative scheduling.
This leads to higher cycles per MAC.

Although the absolute cycle counts differ, the normalized
metrics clearly demonstrate the performance benefit of
compiler-level fusion and pipelining.
