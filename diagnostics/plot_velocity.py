import numpy as np
import matplotlib.pyplot as plt

data_ref_vel = np.genfromtxt('ref_velocity.csv', delimiter=', ')
data_sim_vel = np.genfromtxt('sim_velocity.csv', delimiter=', ')

#plt.plot(data_sim_vel[0:300], label="Simulation")
#plt.plot(data_ref_vel[0:300], label="Reference")

#plt.axhline(data_ref_vel.mean(), color='r')
#plt.axhline(data_sim_vel.mean(), color='g')

plt.plot(data_ref_vel[0:]-data_sim_vel[0:], label="Abs. dfference in speed")

plt.ylabel("m/s")
plt.xlabel("Earth hours")

plt.legend()
plt.show()

# print("Avg. reference velocity: " + str(data_ref_vel.mean()))
# print("Avg. simulated velocity: " + str(data_sim_vel.mean()))

