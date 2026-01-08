import matplotlib.pyplot as plt


models = ["C++ Simulator", "Python Compiler"]
cycles_per_mac = [0.2627, 0.0316]


plt.figure()
plt.bar(models, cycles_per_mac)
plt.ylabel("Cycles per MAC")
plt.title("Normalized Performance Comparison")


plt.savefig("compiler_vs_simulator_cycles_per_mac.png")

print("Plot saved as compiler_vs_simulator_cycles_per_mac.png")
