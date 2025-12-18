import pandas as pd
import matplotlib.pyplot as plt
import os

os.makedirs("plots", exist_ok=True)

df = pd.read_csv("../phase1_mac/results.csv", 
                 names = ["tile_m", "tile_n", "cycles", "macs", "sram_reads", "dram_words", "total_energy", "energy_per_mac"])

df["tile_size"] = df["tile_m"] * df["tile_n"]

print(df)

# Plot 1 : Cycles VS Tile Size
plt.figure()
plt.plot(df["tile_size"], df["cycles"], marker='o') 
plt.xlabel("Tile Size (tile_m * tile_n)")
plt.ylabel("Total Cycles")
plt.title("Performance VS Tile Size")
plt.grid(True)
plt.savefig("plots/cycles_VS_tile_size.png")
plt.close()

# Plot 2 : Total Energy VS Tile Size
plt.figure()
plt.plot(df["tile_size"], df["total_energy"], marker='o') 
plt.xlabel("Tile Size (tile_m * tile_n)")
plt.ylabel("Total Energy")
plt.title("Energy VS Tile Size")
plt.grid(True)
plt.savefig("plots/energy_VS_tile_size.png")
plt.close()

# Plot 3 : Energy per MAC VS Tile Size
plt.figure()
plt.plot(df["tile_size"], df["energy_per_mac"], marker='o') 
plt.xlabel("Tile Size (tile_m * tile_n)")
plt.ylabel("Energy per MAC")
plt.title("Energy Efficiency VS Tile Size")
plt.grid(True)
plt.savefig("plots/energy_per_mac_VS_tile_size.png")
plt.close()

# Plot 4 : DRAM VS SRAM Accesses
plt.figure()
plt.plot(df["tile_size"], df["dram_words"], label = "DRAM Words", marker='o') 
plt.plot(df["tile_size"], df["sram_reads"], label = "SRAM Reads", marker='o') 
plt.xlabel("Tile Size (tile_m * tile_n)")
plt.ylabel("Memory Access Count")
plt.title("Memory Traffic VS Tile Size")
plt.legend()
plt.grid(True)
plt.savefig("plots/memory_traffic_VS_tile_size.png")
plt.close()