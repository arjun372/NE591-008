import sys
import numpy as np

# Check if the correct number of arguments is provided
if len(sys.argv) != 3:
    print("Usage: python script.py m n")
    sys.exit(1)

# Parse the command-line arguments
m = int(sys.argv[1])
n = int(sys.argv[2])

q = np.zeros((m, n))

# Calculate the midpoint indices
midpoint_x = (m - 1) // 2
midpoint_y = (n - 1) // 2

# Set the midpoint to 1.0
q[midpoint_x, midpoint_y] = 1.0

# Construct the output filename using the provided m and n values
output_filename = f"../sources/raw/sources_{m}x{n}.csv"

# Save the matrix to a CSV file with the constructed filename
np.savetxt(output_filename, q, delimiter=",", fmt="%.6f")

print(f"Matrix saved to {output_filename}")
