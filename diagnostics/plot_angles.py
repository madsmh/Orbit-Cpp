import numpy as np
import matplotlib.pyplot as plt

data_angles = np.genfromtxt('angles.csv', delimiter=', ')
data_error = np.genfromtxt('error_pos.csv', delimiter=', ')

max_error = data_error[:,1].max()

# Source wikipedia
ref_max_orbit = 676938
ref_min_orbit = 664862
ref_mean_orbit = 670900



plt.plot(data_angles[:, 1])

plt.title("Angle between simulation and reference")
plt.xlabel("Days")
plt.ylabel("Degrees")
plt.grid(b=True, which='both')

plt.show()