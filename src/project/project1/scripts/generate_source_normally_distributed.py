import numpy as np

# Set the random seed for reproducibility
np.random.seed(42)

# Define the mean and standard deviation for the normally distributed fixed source
mean = 1.0E-5
std_dev = 1.0E-6

# Generate a 50x50 matrix for q(i,j) with normally distributed values
q_matrix = np.random.normal(mean, std_dev, (50, 50))

# Save the q_matrix to a CSV file
np.savetxt("q_matrix.csv", q_matrix, delimiter=",")
