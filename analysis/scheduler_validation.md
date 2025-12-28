# Scheduler Validation and Cost Model Evolution

This document records the validation and evolution of the analytical cost
model used for scheduling tiled matrix multiplication on the simulated
accelerator.

---

## Step 9.6 — Baseline Cost Model

The initial scheduler modeled execution cost as:

    max(compute_cycles, dram_latency + transfer_cycles)

This model was intentionally conservative and treated each tile independently.
The scheduler consistently selected small tiles (e.g., 8×8×8), indicating
a latency-dominated execution regime.

---

## Step 9.7 — Micro-Kernel Validation

A single micro-kernel (TM=8, TN=8, TK=1) was simulated in the C++ cycle-level
simulator.

Observed result:
- Simulated cycles per micro-kernel: ~226 cycles
- Analytical estimate: ~232 cycles

The close agreement validated the analytical model as a safe lower bound.

---

## Step 9.8 — K-Reuse and SRAM-Aware Model

The cost model was extended to:
- Amortize DRAM bandwidth across K tiles
- Enforce SRAM capacity constraints

Despite these improvements, the scheduler continued to select small tiles.
This indicated that DRAM latency, not bandwidth, was the dominant bottleneck
for the given workload.

---

## Step 9.9 — Latency Hiding via Double Buffering

The model was further refined to hide DRAM latency after warm-up using a
double-buffered execution model.

The revised formulation:

    total_cycles = max(compute, transfer) + dram_latency

Even with latency hiding, tile selection remained unchanged for small problem
sizes, confirming that computation was insufficient to amortize startup latency.

---

## Step 9.10 — Multi-Tile Throughput Model

A throughput-oriented model was introduced to amortize latency across all tiles:

- DRAM cost modeled once per (M, N) tile
- Compute cost modeled across all K micro-tiles

For the workload (M=32, N=128, K=64), multiple tile configurations (e.g.,
8×8×8 and 16×16×16) were analytically shown to have identical total cost.
This results in a flat optimal cost surface, where the scheduler selects the
first minimal configuration.

---

## Key Insight

The unchanged tile selection is not a modeling error. It reflects a
latency-dominated regime with insufficient computation to break symmetry
between tile choices.

Larger tiles become optimal only when:
- Problem size increases
- DRAM latency decreases
- Compute throughput increases
- Or graph-level fusion amortizes startup costs

This motivates moving from operator-level scheduling to compiler-level
IR and multi-operator fusion.
