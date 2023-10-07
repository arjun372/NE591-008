import numpy as np

np.random.seed(42)

# Define the mean and standard deviation for the normally distributed fixed source
mean = 1.0E-5
std_dev = 1.0E-20
min_ = 1.0E-7
max_ = 2.0E-4


def gaussian(x, y, mu_x, mu_y, sigma_x, sigma_y):
    return np.exp(-((x - mu_x) ** 2 / (2 * sigma_x ** 2) + (y - mu_y) ** 2 / (2 * sigma_y ** 2)))


def apply_gaussian(m, n, filepath):
    gaussian_params = [
        {"mu_x": mean, "mu_y": mean, "sigma_x": std_dev, "sigma_y": std_dev},
    ]
    q = np.zeros((m, n))
    # for params in gaussian_params:
    #     for i in range(m):
    #         for j in range(n):
    #             q[i, j] += gaussian(i, j, params["mu_x"], params["mu_y"], params["sigma_x"], params["sigma_y"])
    # q += np.random.uniform(min_, max_, (m, n))
    # Normalize the source strength to a desired maximum value, e.g., 1.0
    q[int(m / 2), int(n / 2)] = mean
    q = q / np.max(q)
    # Save the matrix to a CSV file
    np.savetxt(filepath, q, delimiter=",", fmt="%.6f")


def plot_map(inputfile, outputfile):
    import numpy as np
    import matplotlib.pyplot as plt

    # Load the q_matrix from the CSV file
    q_matrix = np.loadtxt(inputfile, delimiter=",")
    # Create a heatmap using matplotlib
    plt.imshow(q_matrix, cmap='hot', interpolation='nearest', origin='lower', extent=[0, 100, 0, 100])
    plt.colorbar(label='Source strength (neutrons/cm³/s)')

    # Set the plot title and axis labels
    plt.title('Fixed Source Distribution Graphite')
    plt.xlabel('x (cm)')
    plt.ylabel('y (cm)')

    # Save the heatmap to a file
    plt.savefig(outputfile, dpi=600)
    plt.close()


# apply_gaussian(2, 2,   "../analysis/sources/raw/project1_sources_2x2.csv")
# plot_map("../analysis/sources/raw/project1_sources_2x2.csv", "../analysis/sources/plots/project1_sources_2x2.png")
#
# apply_gaussian(4, 4,   "../analysis/sources/raw/project1_sources_4x4.csv")
# plot_map("../analysis/sources/raw/project1_sources_4x4.csv", "../analysis/sources/plots/project1_sources_4x4.png")
#
# apply_gaussian(8, 8,   "../analysis/sources/raw/project1_sources_8x8.csv")
# plot_map("../analysis/sources/raw/project1_sources_8x8.csv", "../analysis/sources/plots/project1_sources_8x8.png")
#
# apply_gaussian(16, 16,   "../analysis/sources/raw/project1_sources_16x16.csv")
# plot_map("../analysis/sources/raw/project1_sources_16x16.csv", "../analysis/sources/plots/project1_sources_16x16.png")
#
# apply_gaussian(32, 32,   "../analysis/sources/raw/project1_sources_32x32.csv")
# plot_map("../analysis/sources/raw/project1_sources_32x32.csv", "../analysis/sources/plots/project1_sources_32x32.png")
#
# apply_gaussian(64, 64,   "../analysis/sources/raw/project1_sources_64x64.csv")
# plot_map("../analysis/sources/raw/project1_sources_64x64.csv", "../analysis/sources/plots/project1_sources_64x64.png")
#
# apply_gaussian(128, 128,   "../analysis/sources/raw/project1_sources_128x128.csv")
# plot_map("../analysis/sources/raw/project1_sources_128x128.csv", "../analysis/sources/plots/project1_sources_128x128.png")
#

# plot_map("../analysis/fluxes/raw/project1_fluxes_2x2.csv", "../analysis/fluxes/plots/project1_fluxes_2x2.png")
# plot_map("../analysis/fluxes/raw/project1_fluxes_4x4.csv", "../analysis/fluxes/plots/project1_fluxes_4x4.png")
# plot_map("../analysis/fluxes/raw/project1_fluxes_8x8.csv", "../analysis/fluxes/plots/project1_fluxes_8x8.png")
# plot_map("../analysis/fluxes/raw/project1_fluxes_16x16.csv", "../analysis/fluxes/plots/project1_fluxes_16x16.png")
plot_map("../analysis/fluxes/raw/project1_fluxes_32x32.csv", "../analysis/fluxes/plots/project1_fluxes_32x32.png")
# plot_map("../analysis/fluxes/raw/project1_fluxes_64x64.csv", "../analysis/fluxes/plots/project1_fluxes_64x64.png")
