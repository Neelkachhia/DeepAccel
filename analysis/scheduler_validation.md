### Micro-Kernel Validation Result

Configuration: TM=8, TN=8, TK=1

- Analytical estimate: ~232 cycles
- Simulator result: 226 cycles

The close match validates the analytical cost model
as a reliable predictor for scheduling decisions.

### Step 9.8 Observation

After adding K-reuse and SRAM capacity constraints, the scheduler still prefers
small tiles due to dominant DRAM latency. This motivates explicit latency-hiding
mechanisms such as double buffering in the next step.
