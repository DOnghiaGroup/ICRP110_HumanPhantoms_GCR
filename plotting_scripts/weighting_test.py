from scipy.stats import loguniform
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import scipy.integrate as integrate
import pandas as pd
import scipy.interpolate as interpolate

###########################
# Define various funtions #
###########################

# Purpose: read .dat file provided by OLTARIS
# Parameters:
#     filename: self-explanatory
#     starting_line: entire text of line where data starts (e.g. "C\t Energy array dimensions ...")
#     skip: number of lines to skip after starting line before reading data
#     type_conversion: convert strings to what datatype (e.g. float, int, etc.)?
#     splitting: make data 2-dimensional, splitting at this interval
def read_array(filename, starting_line, skip=2, type_conversion=lambda x:x, splitting=0):
    value_list = []
    
    with open(filename) as f:
        file_lines = f.readlines()

    itr = 0
    for line in file_lines:
        jtr = itr + skip
        if starting_line == line.strip():
            while file_lines[jtr] != "\n":
                cur_line = list(map(type_conversion, file_lines[jtr].strip().split()))
                value_list += cur_line
                jtr += 1
        itr += 1
        
    if splitting != 0:
        value_list = [value_list[i:i+splitting] for i in range(0,len(value_list), splitting)]
    
    return value_list

# Purpose: generate a loguniform probability density function which represents the generated distribution (this should of course be changed if the generated distribution is not loguniform)
# Parameters:
#     x: the point at which to return the function's value
#     xmin: lower bound for the distribution
#     xmax: upper bound for the distribution
# WARNING: Right now, the integer 3330 is hard-coded and really is a normalization which depends on bounds
def loguniform_pdf(x, xmin, xmax):
    return  x * np.log(10) * (1 / (x * np.log(xmax/xmin))) / 3330

# Purpose: return a weight value based on various factors
# Parameters:
#     E: energy per nucleon of the particle which we're weighting
#     n: number of particles of that type which were generated
#     nbins: number of bins being plotted in the histogram
#     xmin: minimum energy sampled
#     xmax: maximum energy sampled
#     area: surface area sampled
#     time: arbitrary -- in same units as physical spectrum
#     solidang: ratio of solid angle sampled to solid angle assumed in physical spectrum
#     name: particle name to determine which physical spectrum to use
def weight(E, n, nbins, xmin, xmax, area=1, time=1, solidang=1):
    w = (nbins/(xmax-xmin)) * (area * time * solidang) / n * oltaris_proton_interp(E) / loguniform_pdf(E, xmin, xmax)
    return w

def mag_shielding(R, coef=1):
    t = (coef/3) * np.log10(R)
#    if (t < 1):
    return t
#    else:
#        return 1

####################################
# Read in GCR data and interpolate #
####################################

filename = "hi_flux.dat"
energies_axis = []
names = []
energies_values = []
energies_axis = read_array(filename, "C\t Energy array dimensions and data", type_conversion=float)
names = read_array(filename, "C\t Particle array dimensions and data")
energies_values = read_array(filename, "C\t array dimensions and data", type_conversion=float, splitting=125)
                             
oltaris = pd.DataFrame(energies_values, columns=energies_axis, index=names).transpose()
                             
oltaris_proton_interp = interpolate.interp1d(oltaris['proton'].index.tolist(), oltaris['proton'].tolist())
oltaris_alpha_interp = interpolate.interp1d(oltaris['alpha'].index.tolist(), oltaris['alpha'].tolist())
oltaris_C12_interp = interpolate.interp1d(oltaris['C12'].index.tolist(), oltaris['C12'].tolist())
oltaris_Si28_interp = interpolate.interp1d(oltaris['Si28'].index.tolist(), oltaris['Si28'].tolist())
oltaris_Fe56_interp = interpolate.interp1d(oltaris['Fe56'].index.tolist(), oltaris['Fe56'].tolist())
         

#########################################
# Create test data and populate weights #
#########################################

emin = 1e1
emax = 1e4
num_runs = 100000
numbins = 50
testData = loguniform.rvs(emin, emax, size=num_runs).tolist()
weights = []
for energy in testData:
    weights.append(
        mag_shielding(energy, coef=0.75) * weight(E=energy, n=num_runs, nbins=numbins, xmin=emin, xmax=emax, area=1, time=1, solidang=1)
    )
    
###########################################
# Plot to make sure everything looks good #
###########################################

mpl.rcParams['figure.figsize'] = [10, 5]
xlims = [emin, emax]
ylims = [1e-5, 1e5]

# Verify test data was created correctly
plt.subplot(131)
plt.hist(testData,
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins))
plt.xscale('log')
plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.title("test sample data")

# Verify 'loguniform_pdf' matches
plt.subplot(132)
dist_x = np.arange(xlims[0], xlims[1], 10)
dist_y = loguniform_pdf(dist_x, xmin=emin, xmax=emax)
plt.plot(dist_x, dist_y)
plt.xscale('log')
plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.yticks([])
plt.title("sample PDF")

# Weighted test data with physical spectrum for comparison
plt.subplot(133)
plt.hist(testData,
        histtype='step',
        bins=np.geomspace(xlims[0], xlims[1], numbins),
        weights=weights)
phys_x = np.arange(xlims[0], xlims[1], 10)
phys_y = oltaris_proton_interp(phys_x)
plt.plot(phys_x, phys_y)
plt.xscale('log')
plt.yscale('log')
plt.xlim(xlims)
plt.ylim(ylims)
plt.yticks([])
plt.title("weighted data over physical spectrum")

plt.show()

###############################################

plt.clf()
ax = plt.gca()
ax.set_aspect(1)
plt.plot(phys_x, phys_y, label="without magnetic shielding")
phys_y = oltaris_proton_interp(phys_x) * mag_shielding(phys_x, coef=0.75)
plt.plot(phys_x, phys_y, label="with magnetic shielding")
plt.yscale('log')
plt.xscale('log')
plt.yticks(fontsize=15)
plt.xticks(fontsize=15)
plt.legend(fontsize=20)
plt.xlabel("Kinetic Energy (MeV/n))", fontsize=20)
plt.ylabel("Flux (particles day$^{-1}$ cm$^{-2}$)", fontsize=20)
plt.tight_layout()
plt.show()

###############################################

# Verify area under 'loguniform_pdf' is ~1.0
print(integrate.quad(lambda energy: loguniform_pdf(energy, xmin=xlims[0], xmax=xlims[1]), a=xlims[0], b=xlims[1])[0])
