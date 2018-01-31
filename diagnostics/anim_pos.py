import numpy as np
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import animation

data_ref_pos = np.genfromtxt('pos_ref.csv', delimiter=', ')*1e-3
data_sim_pos = np.genfromtxt('pos_sim.csv', delimiter=', ')*1e-3


def animate(i, data, lines):
    for dat, line in zip(data, lines):
        line[0].set_data(dat[:i,0], dat[:i, 1])
        line[0].set_3d_properties(dat[:i,2])


fig = plt.figure()
ax = fig.gca(projection='3d')

data = [data_ref_pos, data_sim_pos]
lines = [ ax.plot(data_ref_pos[0:,0], data_ref_pos[0:,1], data_ref_pos[0:,2], label="Reference"),
          ax.plot(data_sim_pos[0:,0], data_sim_pos[0:,1], data_sim_pos[0:,2], label="Simulation")]

ax.legend()
# ax.relim()
# ax.autoscale_view(True, True, True)

anim = animation.FuncAnimation(fig, animate, fargs=(data, lines),frames=300, interval=900)



anim.save('test.mp4', fps=30, extra_args=['-vcodec', 'libx264'])

plt.show()