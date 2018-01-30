import numpy as np
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt


data_ref_pos = np.genfromtxt('pos_ref.csv', delimiter=', ')
data_sim_pos = np.genfromtxt('pos_sim.csv', delimiter=', ')

fig = plt.figure()
ax = fig.gca(projection='3d')

ax.plot(data_ref_pos[0:50,0], data_ref_pos[0:50, 1], data_ref_pos[0:50,2], label="Reference")
ax.plot(data_sim_pos[0:50,0], data_sim_pos[0:50, 1], data_sim_pos[0:50,2], label="Simulation")

# plt.title("Position, reference and simulation")
ax.legend()


plt.show()