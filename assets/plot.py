import matplotlib.pyplot as plt

keyRange = 200000

# Data

A = [48,75,96,127,134,138,142,136,133,131]
B = [68,117,158,189,192,199,201,202,190,188]
C = [84,155,212,252,265,273,280,283,271,262]


# Number of threads
num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# Plot
plt.plot(num_threads, C, label="9% add, 1% remove, 90% contains", marker='v', markevery=[2, 4, 8], color = 'blue')
plt.plot(num_threads, B, label="20% add, 10% remove, 70% contains", marker='^', markevery=[2, 4, 8], color = 'red')
plt.plot(num_threads, A, label="50% add, 50% remove, 0% contains", marker='o', markevery=[2, 4, 8], color = 'magenta')

plt.title(f'Number of Threads vs Throughput\nKey Range: {keyRange}', fontsize=11, fontweight='bold')
plt.xlabel('Number of Threads', fontsize=10, fontweight='bold')
plt.ylabel('Throughput (operations per millisecond)', fontsize=10, fontweight='bold')
plt.xticks([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

plt.legend(fontsize=11)

name = f"MAC-{keyRange}.pdf"
plt.savefig(name)

plt.show()
