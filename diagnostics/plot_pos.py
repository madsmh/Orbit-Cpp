import numpy as np
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt


data_ref_pos = np.genfromtxt('pos_ref.csv', delimiter=', ')*1e-3
data_sim_pos = np.genfromtxt('pos_sim.csv', delimiter=', ')*1e-3

ref_pos = np.concatenate((np.array([[0,0,0]]), [data_ref_pos[-1,:]]), axis=0)
sim_pos = np.concatenate((np.array([[0,0,0]]), [data_sim_pos[-1,:]]), axis=0)




fig = plt.figure()
ax = fig.gca(projection='3d')

ax.plot(ref_pos[:,0], ref_pos[:, 1], ref_pos[:,2], label="Reference")
ax.plot(sim_pos[:, 0], sim_pos[:, 1], sim_pos[:, 2], label="Simulation")

# plt.title("Position, reference and simulation")
ax.legend()
ax.set_xlabel("x (km)")
ax.set_ylabel("y (km)")
ax.set_zlabel("z (km)")

plt.show()