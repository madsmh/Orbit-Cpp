import numpy as np
import matplotlib.pyplot as plt

data_dist = np.genfromtxt('dist.csv', delimiter=', ')
data_error = np.genfromtxt('error_pos.csv', delimiter=', ')

max_error = data_error[:,1].max()

# Source wikipedia
ref_max_orbit = 676938
ref_min_orbit = 664862
ref_mean_orbit = 670900

sim_mean_orbit = data_dist[:,1].mean()

plt.plot(data_dist[:, 1])
plt.axhline(y=ref_max_orbit, color='r')
plt.axhline(y=ref_min_orbit, color='r')
plt.axhline(y=ref_mean_orbit, color='r')
plt.axhline(y=sim_mean_orbit, color='y')
# plt.axhline(y=max_error/2, color='y')

plt.xlabel("Days")
plt.ylabel("Distance to Jupiter")

plt.show()