import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import scipy.interpolate as interpolate

###########################
# Define global variables #
###########################

# General parameters
data_filenames = ["geant4_runs/doseDepositByPrimary_Basic_freespace_1e5runs.dat", 
                  "geant4_runs/doseDepositByPrimary_Basic_Al_5gcm2_1e5runs.dat",
                  "geant4_runs/doseDepositByPrimary_Basic_Al_20gcm2_1e5runs.dat",
                  "geant4_runs/doseDepositByPrimary_Basic_Al_40gcm2_1e5runs.dat"]
run_names = ["Free space", "Al 5 g cm$^{-2}$", "Al 20 g cm$^{-2}$", "Al 40 g cm$^{-2}$"]
particlesToPlot = ['proton', 'alpha', 'C12', 'Si28', 'Fe56']
sampling_radius = 180  # In centimeters
oltaris_filename = "hi_flux.dat"

# Plot parameters
mpl.rcParams['figure.figsize'] = [18, 13]
mpl.rcParams['axes.labelsize'] = 20
numbins = 40
plot_filename = "externalFlux_with_dose.jpg"
xlims = [1e1, 1e4]
ylims = [1e2, 1e8]

###########################
# Define global constants #
###########################

# Primary particle atomic numbers and masses
z = {"proton" : 1, 
     "alpha" : 2, 
     "C12" : 6, 
     "Si28" : 14, 
     "Fe56" : 26}
a = {"proton" : 1,
     "alpha" : 4, 
     "C12" : 12, 
     "Si28" : 28, 
     "Fe56" : 56}

############################
# Define various functions #
############################

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

# Purpose: generate a loguniform probability density function
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
def weight(E, n, nbins, xmin, xmax, area=1, time=1, solidang=1, name="proton"):
    w = (nbins/(xmax-xmin)) * (area * time * solidang) / n * functions[name](E) / loguniform_pdf(E, xmin, xmax)
    return w

##########################################
# Create OLTARIS interpolation functions #
##########################################

# Create data frame with points from the input file
energies_axis = []
names = []
energies_values = []
energies_axis = read_array(oltaris_filename, "C\t Energy array dimensions and data", type_conversion=float)
names = read_array(oltaris_filename, "C\t Particle array dimensions and data")
energies_values = read_array(oltaris_filename, "C\t array dimensions and data", type_conversion=float, splitting=125)
oltaris = pd.DataFrame(energies_values, columns=energies_axis, index=names).transpose()

# Create functions from data frame
oltaris_proton_interp = interpolate.interp1d(oltaris['proton'].index.tolist(), oltaris['proton'].tolist())
oltaris_alpha_interp = interpolate.interp1d(oltaris['alpha'].index.tolist(), oltaris['alpha'].tolist())
oltaris_C12_interp = interpolate.interp1d(oltaris['C12'].index.tolist(), oltaris['C12'].tolist())
oltaris_Si28_interp = interpolate.interp1d(oltaris['Si28'].index.tolist(), oltaris['Si28'].tolist())
oltaris_Fe56_interp = interpolate.interp1d(oltaris['Fe56'].index.tolist(), oltaris['Fe56'].tolist())

functions = {"proton" : oltaris_proton_interp,
     "alpha" : oltaris_alpha_interp, 
     "C12" : oltaris_C12_interp, 
     "Si28" : oltaris_Si28_interp, 
     "Fe56" : oltaris_Fe56_interp}

#############################################
# Plot (iterating through 'data_filenames') #
#############################################

# Create grid on which to plot
fig, ax = plt.subplots(len(data_filenames), len(particlesToPlot), sharex='col', sharey='row')

for file_itr in range(len(data_filenames)):  # Iterate through each file
    # Import data
    particles = pd.read_csv(data_filenames[file_itr], names=["primary_name","energy","dose"])

    # Divide energies by nucleon number
    for i, row in particles.iterrows():
        particles.at[i, 'energy'] /= z[row['primary_name']]

    # Count number of each particle and store in dictionary
    particle_num = {}
    for i, row in particles.iterrows():
        if row['primary_name'] not in particle_num:
            particle_num[row['primary_name']] = 0
        particle_num[row['primary_name']] += 1

    # Populate lists of (stored in dictionaries):
    #     1. energies for each particle
    #     2. weights for each particle
    #     3. unweighted doses for each particle
    particleLists = {}
    weightLists = {}
    doseLists = {}
    emin = min(particles['energy'].to_list())
    emax = max(particles['energy'].to_list())
    print(emin)
    print(emax)
    for i, row in particles.iterrows():
        if row['primary_name'] not in particleLists:
            particleLists[row['primary_name']] = []
        if row['primary_name'] not in weightLists:
            weightLists[row['primary_name']] = []
        if row['primary_name'] not in doseLists:
            doseLists[row['primary_name']] = []
        particleLists[row['primary_name']].append(row['energy'])
        weightLists[row['primary_name']].append(
            weight(row['energy'], particle_num[row['primary_name']], numbins, emin, emax, area=4*np.pi*(sampling_radius**2), time=1, solidang=1, name=row['primary_name'])
        )
        particles.at[i, 'weight'] = weightLists[row['primary_name']][-1]
        doseLists[row['primary_name']].append(row['dose'])
    for key in particleLists:  # Convert to Series instead of lists
        particleLists[key] = pd.Series(particleLists[key])
    for key in doseLists:
        doseLists[key] = pd.Series(doseLists[key])
    particleLists = dict(sorted(particleLists.items(), key=lambda i:particlesToPlot.index(i[0])))  # Sort

    # Create lists of the relevant lists to plot (i.e. if in 'particlesToPlot')
    names = []
    listsToPlot = []
    weightsToPlot = []
    dosesToPlot = []
    for key in particleLists:
        if key in particlesToPlot:
            listsToPlot.append(particleLists[key])
            weightsToPlot.append(weightLists[key])
            names.append(key)
            dosesToPlot.append(doseLists[key])

    # Plot histograms
    for i in range(len(particlesToPlot)):
        ax[file_itr, i].hist(listsToPlot[i],
                bins=np.geomspace(xlims[0], xlims[1], numbins),
                weights=weightsToPlot[i],
                histtype='step',
                linewidth=3,
                alpha=0.3,
                label="External Flux")
        ax[file_itr, i].set_xscale('log')
        ax[file_itr, i].set_yscale('log')
        ax[file_itr, i].set_xlim(xlims)
        ax[file_itr, i].set_ylim(ylims)
        ax[file_itr, i].grid(True)
        ax[file_itr, i].set_xticks([1e1, 1e2, 1e3, 1e4])
        ax[file_itr, i].set_xticklabels(["1", "2", "3", "4"])
        ax[file_itr, i].set_yticks([1e2, 1e4, 1e6, 1e8])
        ax[file_itr, i].set_yticklabels(["2", "4", "6", "8"])
        ax[file_itr, i].hist([], color='red', linewidth=3, label="Dose", histtype='step')  # Dose line agent for legend

        ax2 = ax[file_itr, i].twinx()
        ax2.hist(listsToPlot[i],
                bins=np.geomspace(xlims[0], xlims[1], numbins),
                weights=[x * y for x, y in zip(dosesToPlot[i], weightsToPlot[i])],
                histtype='step',
                color='red',
                linewidth=3,
                label="Dose")
        ax2.set_yscale('log')
        ax2.set_ylim([1e-10, 1e-4])
        if i != (len(particlesToPlot)-1):
            ax2.set_yticks([])
        if i == (len(particlesToPlot)-1):
            ax2.set_yticks([1e-10, 1e-8, 1e-6, 1e-4])
            ax2.set_yticklabels(["$-10$", "$-8$", "$-6$", "$-4$"])  

# Set row and column labels
for axis, col in zip(ax[0], particlesToPlot):
    axis.set_title(col, size=15)
for axis, row in zip(ax[:,0], run_names):
    axis.set_ylabel(row, rotation=90, size=15)

# Set overall axis labels by creating an invisible background subplot
ax[0, 0].legend(loc=[0.0, 1.3], fontsize=15)
fig.add_subplot(111, frameon=False)
plt.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)
plt.yticks([])
plt.xlabel("log Kinetic Energy (MeV/n)")
plt.ylabel("log External Flux (particles/day)", labelpad=40.0)
right_y = plt.gca().twinx()
right_y.set_ylabel("log Dose (Gy)", labelpad=40.0)
right_y.set_yticks([])

# Save plot
plt.savefig(plot_filename, dpi=1000)
