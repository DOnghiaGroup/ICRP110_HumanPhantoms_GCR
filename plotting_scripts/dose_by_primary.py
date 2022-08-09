import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import scipy.interpolate as interpolate

###########################
# Define global variables #
###########################

data_filenames = ["geant4_runs/doseDepositByPrimary_Basic_freespace_1e5runs.dat",
                  "geant4_runs/doseDepositByPrimary_Basic_Al_5gcm2_1e5runs.dat",
                  "geant4_runs/doseDepositByPrimary_Basic_Al_20gcm2_1e5runs.dat",
                  "geant4_runs/doseDepositByPrimary_Basic_Al_40gcm2_1e5runs.dat"]
labels = ["Free Space", "5 g/cm$^2$", "20 g/cm$^2$", "40 g/cm$^2$"]
xlims = [10, 10000]
numbins = 40

####################################################
# Create interpolation functions from OLTARIS data #
####################################################

def read_array_oltaris(filename, starting_line, skip=2, type_conversion=lambda x:x, splitting=0):
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

flux_data_filename = "hi_flux.dat"
energies_axis = []
names = []
energies_values = []
energies_axis = read_array_oltaris(flux_data_filename, "C\t Energy array dimensions and data", type_conversion=float)
names = read_array_oltaris(flux_data_filename, "C\t Particle array dimensions and data")
energies_values = read_array_oltaris(flux_data_filename, "C\t array dimensions and data", type_conversion=float, splitting=125)
oltaris = pd.DataFrame(energies_values, columns=energies_axis, index=names).transpose()

oltaris_proton_interp = interpolate.interp1d(oltaris['proton'].index.tolist(), oltaris['proton'].tolist())
oltaris_alpha_interp = interpolate.interp1d(oltaris['alpha'].index.tolist(), oltaris['alpha'].tolist())
oltaris_C12_interp = interpolate.interp1d(oltaris['C12'].index.tolist(), oltaris['C12'].tolist())
oltaris_Si28_interp = interpolate.interp1d(oltaris['Si28'].index.tolist(), oltaris['Si28'].tolist())
oltaris_Fe56_interp = interpolate.interp1d(oltaris['Fe56'].index.tolist(), oltaris['Fe56'].tolist())

#############################################
# Define some other constants and functions #
#############################################

# Weighting parameters
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

# Weighting functions
def loguniform_pdf(x, xmin, xmax):
    return  x * np.log(10) * (1 / (x * np.log(xmax/xmin))) / 3330

def weight(E, n, nbins, xmin, xmax, area=1, time=1, solidang=1, name="proton"):
    w = (nbins/(xmax-xmin)) * (area * time * solidang) / n * functions[name](E) / loguniform_pdf(E, xmin, xmax)
    return w

functions = {"proton" : oltaris_proton_interp,
     "alpha" : oltaris_alpha_interp,
     "C12" : oltaris_C12_interp,
     "Si28" : oltaris_Si28_interp,
     "Fe56" : oltaris_Fe56_interp}

##################################
# Import run data and manipulate #
##################################

# Make an array of dataframes for each run
runs = []
for item in data_filenames:
    runs.append(pd.read_csv(item, names=["primary_name","energy","dose"]))

# For heavier particles, divide energies
for run in runs:
    for i, row in run.iterrows():
        run.at[i, 'energy'] /= z[row['primary_name']]

# Count number of each particle
particle_nums = []
for j in range(len(runs)):
    particle_nums.append({})
    for i, row in runs[j].iterrows():
        if row['primary_name'] not in particle_nums[j]:
            particle_nums[j][row['primary_name']] = 0
        particle_nums[j][row['primary_name']] += 1

# Add weight column
for j in range(len(runs)):
    for i, row in runs[j].iterrows():
        runs[j].at[i, 'weight'] = weight(row['energy'], particle_nums[j][row['primary_name']], numbins, 10, 10000, area=4*np.pi*(153**2), time=1, solidang=1, name=row['primary_name'])

# Sum total doses for each particle type
total_doses = []
for run in runs:
    total_doses.append({})
    for i, row in run.iterrows():
        if row['primary_name'] not in total_doses[-1]:
            total_doses[-1][row['primary_name']] = 0.0
        total_doses[-1][row['primary_name']] += (row['dose']*row['weight'])

# Make list of relative contributions
relative = []
for run in total_doses:
    relative.append({})
    for key in run:
        relative[-1][key] = (run[key]/sum(run.values()))*100

########
# Plot #
########

mpl.rcParams['figure.figsize'] = [10,5]
dict_order = ["proton", "alpha", "C12", "Si28", "Fe56"]

for k in range(len(relative)):
    relative[k] = dict(sorted(relative[k].items(), key=lambda i:dict_order.index(i[0])))

    keys, values = relative[k].keys(), relative[k].values()
    plt.plot(range(len(values)), values, 
             marker='.', 
             markersize=15, 
             label=labels[k])
plt.yscale('log')
plt.xticks(range(len(values)), keys, size=20)
plt.yticks(size=20)
plt.xlabel("Primary particle type", size=20)
plt.ylabel("Relative dose contribution (%)", size=20)
plt.legend()

plt.savefig("relative_dose_contribution.jpg", dpi=1000)
