import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# ***Import data***
print("Importing data . . . ")
data = pd.read_csv("secondary_particle_info.csv", names=["particle","counter","E_k"], nrows=100000)

# ***Get total particle counts (at each energy)***
# Initialize with beam configuration
print("Populating data structures part 1 . . . ")
particleTracker = {('alpha', 400) : 1e6,  # Keys are a tuple: (particle, E_k)
                  ('proton', 100) : 1e6,  # Values are the count
                  ('Be9', 900) : 1e6}
# Populate with new values from the run
for i, row in data.iterrows():
    # Create key if it doesn't exist
    if (row['particle'], row['E_k']) not in particleTracker:
        particleTracker[(row['particle'], row['E_k'])] = 0
    # Update key value
    particleTracker[(row['particle'], row['E_k'])] += row['counter']

# ***Separate into a list of energies for each particle***
# That is, here we have a dictionary where the keys are particles. The values
# are lists of the energies, with duplicates
print("Populating data structures part 2 . . . ")
particleEnergies = {}
for key in particleTracker:
    if key[1] != 0:
        # Create key if it doesn't exist
        if key[0] not in particleEnergies:
            particleEnergies[key[0]] = []
        # Add the energy for as many particles as exist
        for i in range(int(particleTracker[key])):
            particleEnergies[key[0]].append(key[1])

# ***Create original spectra***
print("Creating beam spectra . . . ")
originalBeamSpectra = {('alpha', 400) : 1e6,  # Keys are a tuple: (particle, E_k)
                  ('proton', 100) : 1e6,  # Values are the count
                  ('Be9', 900) : 1e6}

originalEnergies = {}
for key in originalBeamSpectra:
    if key[1] != 0:
        # Create key if it doesn't exist
        if key[0] not in originalEnergies:
            originalEnergies[key[0]] = []
        # Add the energy for as many particles as exist
        for i in range(int(originalBeamSpectra[key])):
            originalEnergies[key[0]].append(key[1])

# ***Create histograms***
print("Creating histograms . . . ")

xlims = [1e-3, 1e3]
ylims = [0.1, 5.0]
beamParticles = ['proton', 'alpha', 'Be9']
secondaryParticles = ['neutron', 'e-']
numbins = 20

originalLists = []
for name in beamParticles:
    originalLists.append(pd.Series(originalEnergies[name]))
plt.subplot(121)
plt.hist(originalLists, 
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins),
        label=beamParticles)
plt.xscale('log')
# plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.legend()
plt.title("Beam Spectra")

secondariesLists = []
for name in secondaryParticles:
    secondariesLists.append(pd.Series(particleEnergies[name]))
plt.subplot(122)
plt.hist(secondariesLists, 
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins),
        label=secondaryParticles)
plt.xscale('log')
# plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.legend()
plt.xticks([])
plt.title("Final Spectra")

plt.savefig('spectra.jpg')
