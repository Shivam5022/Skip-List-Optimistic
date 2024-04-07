import matplotlib.pyplot as plt

keyRange = 20000

# Data
A = [19,36,53,69,87,104,117,136,150,160,175,184,200,215,230,243,262,271,281,282]
B = [23,42,64,84,108,131,146,168,191,212,222,239,252,262,285,297,316,322,333,335]
C = [26,50,72,93,114,137,159,187,205,234,260,289,304,310,320,332,351,356,381,393]

# Number of threads
num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

# Plot
plt.plot(num_threads, C, label="9% add, 1% remove, 90% contains", marker='v', markevery=[2, 4, 8, 16, 19], color = 'blue')
plt.plot(num_threads, B, label="20% add, 10% remove, 70% contains", marker='^', markevery=[2, 4, 8, 16, 19], color = 'red')
plt.plot(num_threads, A, label="50% add, 50% remove, 0% contains", marker='o', markevery=[2, 4, 8, 16, 19], color = 'magenta')

plt.title(f'Number of Threads vs Throughput\nKey Range: {keyRange}', fontsize=11, fontweight='bold')
plt.xlabel('Number of Threads', fontsize=10, fontweight='bold')
plt.ylabel('Throughput (operations per millisecond)', fontsize=10, fontweight='bold')
plt.xticks([2, 4, 6, 8, 10, 12, 14, 16, 18, 20])

plt.legend(fontsize=11)

name = f"{keyRange}.pdf"
plt.savefig(name)

plt.show()
