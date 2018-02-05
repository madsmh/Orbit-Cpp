import numpy as np
import matplotlib.pyplot as plt

data_angles = np.genfromtxt('delta.csv', delimiter=', ')
data_angles = data_angles*1e-3

plt.figure(num=None, figsize=(12, 8), dpi=150, facecolor='w', edgecolor='k')

plt.plot(data_angles[:,0], label="x")
plt.plot(data_angles[:,1], label="y")
plt.plot(data_angles[:,2], label="z")

plt.title("Error between x, y, and z components")
plt.grid(b=True, which='both')
plt.legend()
plt.xlabel("Days")
plt.ylabel("km")


plt.show()