import numpy as np
import matplotlib.pyplot as plt
import matplotlib.tri as tri
a=np.genfromtxt("C:/Users/filip/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/NewCell0Ds.csv");
b=np.genfromtxt("C:/Users/filip/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/NewCell2Ds.csv",skip_header=1);
x=a[1:,1]
y=a[1:,2]
triangles=b[:,1:4]
triang=tri.Triangulation(x,y,triangles)
plt.triplot(triang)
plt.plot(range(1))
plt.xlim(0,1)
plt.ylim(0,1)
ax = plt.gca()
ax.set_aspect('equal',adjustable='box')
plt.savefig("NewMesh.png")
plt.close()
