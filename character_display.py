import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
fs = open("C:/Dev/MLC/data/character_dim.dat")

f = open("C:/Dev/MLC/data/character.dat", "r")
a = np.fromfile(f, dtype=np.uint8)
b = np.fromfile(fs, dtype=np.uint32)
path = 'C:/Dev/MLC/output/'+str(b[1]) + '.png'
plt.imsave(path, np.array(a).reshape(b[0],b[0]), cmap=cm.gray)
#plt.imshow(np.array(a).reshape(b[0],b[0]))
