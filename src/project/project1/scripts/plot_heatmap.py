import numpy as np
import matplotlib.pyplot as plt

# Load the q_matrix from the CSV file
q_matrix = np.loadtxt("../examples/project1_source-terms_2.csv", delimiter=",")
print(q_matrix)
# Create a heatmap using matplotlib
plt.imshow(q_matrix, cmap='hot', interpolation='nearest', origin='lower', extent=[0, 100, 0, 100])
plt.colorbar(label='Source strength (neutrons/cm³/s)')

# Set the plot title and axis labels
plt.title('Fixed Source Distribution Graphite')
plt.xlabel('x (cm)')
plt.ylabel('y (cm)')

# Save the heatmap to a file
plt.savefig("project1_source-terms_2_heatmap.png", dpi=600)

# Show the heatmap
plt.show()

# Load the q_matrix from the CSV file
q_matrix = np.loadtxt("../examples/project1_flux_2.csv", delimiter=",")
print(q_matrix)
# Create a heatmap using matplotlib
plt.imshow(q_matrix, cmap='hot', interpolation='nearest', origin='lower', extent=[0, 100, 0, 100])
plt.colorbar(label='Source strength (neutrons/cm³/s)')

# Set the plot title and axis labels
plt.title('Fixed Source Distribution Graphite')
plt.xlabel('x (cm)')
plt.ylabel('y (cm)')

# Save the heatmap to a file
plt.savefig("project1_flux_2_heatmap.png", dpi=600)

# Show the heatmap
plt.show()
