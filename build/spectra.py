import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# ***Import data***
print("Importing data . . . ")
data = pd.read_csv("secondary_particle_info.csv", names=["particle","counter","E_k"], nrows=20000000)

# ***Get total particle counts (at each energy)***
# Initialize with beam configuration
print("Populating data structures part 1 . . . ")
particleTracker = {('alpha', 400) : 333333,  # Keys are a tuple: (particle, E_k)
                  ('alpha', 1200) : 333333,  # Values are the count
                  ('alpha', 4000) : 333333,
                  ('proton', 100) : 333333,
                  ('proton', 300) : 333333,
                  ('proton', 1000) : 333333,
                  ('Fe56', 2600) : 333333,
                  ('Fe56', 7800) : 333333,
                  ('Fe56', 26000) : 333333}
# Populate with new values from the run
for i, row in data.iterrows():
    # Create key if it doesn't exist
    if (row['particle'], row['E_k']) not in particleTracker:
        particleTracker[(row['particle'], row['E_k'])] = 0
    # Update key value
    particleTracker[(row['particle'], row['E_k'])] += row['counter']
    # Log for progress update
    if i%1000000 == 0:
        print(". . . " + str(i) + " rows completed")

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
originalBeamSpectra = {('alpha', 400) : 333333,  # Keys are a tuple: (particle, E_k)
                  ('alpha', 1200) : 333333,  # Values are the count
                  ('alpha', 4000) : 333333,
                  ('proton', 100) : 333333,
                  ('proton', 300) : 333333,
                  ('proton', 1000) : 333333,
                  ('Fe56', 2600) : 333333,
                  ('Fe56', 7800) : 333333,
                  ('Fe56', 26000) : 333333}

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

xlims = [1e-1, 3e5]
ylims = [1e-1, 1e8]
beamParticles = ['proton', 'alpha', 'Fe56']
secondaryParticles = ['proton', 'alpha', 'Fe56', 'neutron', 'e-']
numbins = 100

originalLists = []
for name in beamParticles:
    originalLists.append(pd.Series(originalEnergies[name]))
plt.subplot(121)
plt.hist(originalLists, 
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins),
        label=beamParticles)
plt.xscale('log')
plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.legend(loc='lower left')
plt.title("Beam Spectra")
plt.ylabel("n")
plt.xlabel("Kinetic Energy (MeV)")

secondariesLists = []
for name in secondaryParticles:
    secondariesLists.append(pd.Series(particleEnergies[name]))
plt.subplot(122)
plt.hist(secondariesLists, 
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins),
        label=secondaryParticles)
plt.xscale('log')
plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.legend(bbox_to_anchor=(1.2,0), loc='lower right')
plt.yticks([])
plt.title("Final Spectra")

plt.savefig('spectra.jpg', dpi=1000)
