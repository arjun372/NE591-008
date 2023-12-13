import numpy as np


def multi_lobe(x, y, m, n, k_x, k_y):
    return np.sin(k_x * np.pi * x / m) * np.cos(k_y * np.pi * y / n)


m = 64
n = 64
k_x = 3
k_y = 3

q = np.zeros((m, n))

for i in range(m):
    for j in range(n):
        q[i, j] = multi_lobe(i, j, m, n, k_x, k_y)

# Shift the values to make them positive and normalize the source strength to a desired maximum value, e.g., 1.0
q = (q - np.min(q)) / (np.max(q) - np.min(q))

# Save the matrix to a CSV file
np.savetxt("../sources/raw/sources_64x64.csv", q, delimiter=",", fmt="%.6f")
