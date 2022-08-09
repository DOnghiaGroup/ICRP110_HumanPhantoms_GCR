import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

###########################
# Define global variables #
###########################

event_filename = "oneevent.csv"
phantom_xy_filename = ""
phantom_xz_filename = "phantom_image_files/malephantom_xz.jpeg"
phantom_yz_filename = "phantom_image_files/malephantom_yz.jpeg"
particleTypes = ['proton', 'neutron', 'e-', 'gamma', 'e+', 'deuteron']
colors = {'neutron' : 'red',  # Must define for all particles on previous line
         'e-' : 'blue',
         'proton' : 'green',
         'gamma' : 'purple',
         'e+' : 'cyan',
         'neutrino' : 'black'}
cm = plt.cm.get_cmap('turbo')

######################
# Read in event data #
######################

data = pd.read_csv(event_filename, names=["particle_name","E_k","x","y","z","E_dep"])

names = []
edep = []
ek = []
xpos = []
ypos = []
zpos = []

for index, row in data.iterrows():
    if (row["E_dep"] != 0) and (row["particle_name"] in particleTypes):
        names.append(row["particle_name"])
        edep.append(row["E_dep"])
        ek.append(row["E_k"])
        xpos.append(row["x"])
        ypos.append(row["y"])
        zpos.append(row["z"])

########
# Plot #
########

plt.rcParams["figure.figsize"] = (5, 8)

# Read in and plot phantom image files
malephantom_yz = plt.imread(phantom_yz_filename)
malephantom_xz = plt.imread(phantom_xz_filename)
malephantom_xz = np.rot90(malephantom_xz, k=2)

# Plot yz projection of phantom and hits
plt.subplot(121)
plt.imshow(malephantom_yz, alpha=0.3, extent=(-350, 370, -920, 920))
plt.scatter(xpos, zpos, c=pd.Series(names).map(colors), s=5)
cb = plt.scatter(ypos, zpos, c=ek, s=1, cmap=cm)
axes = plt.gca()
axes.set_aspect(1)
plt.xticks([])
plt.yticks([])
plt.xlim(-150, 150)
plt.ylim(-1000, 1000)
axes.axis('off')

# Plot xz direction of phantom and hits
plt.subplot(122)
plt.imshow(malephantom_xz, alpha=0.3, extent=(-300, 300, 900, -900))
plt.scatter(xpos, zpos, c=pd.Series(names).map(colors), s=5)
plt.scatter(xpos, zpos, c=ek, s=1, cmap=cm)
axes = plt.gca()
axes.set_aspect(1)
plt.xticks([])
plt.yticks([])
plt.xlim(-270, 270)
plt.ylim(-1000, 1000)
axes.axis('off')

# Add colorbar
plt.subplots_adjust(bottom=0.1, right=0.8, top=0.8)
cax = plt.axes([0.85, 0.15, 0.045, 0.6])
plt.colorbar(cb, label="Kinetic Energy (MeV)", cax=cax)

plt.savefig("one_hit.jpg", dpi=1000, bbox_inches = "tight")
