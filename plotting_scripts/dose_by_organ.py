import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import scipy.interpolate as interpolate
import json

###########################
# Define global variables #
###########################

organs_to_show = ["Bladder", "Brain", "Breast", "Colon", "Esophagus", "Heart", "Liver", "Lungs", "Testes", "Stomach", "Thyroid"]
files_to_use = ["geant4_runs/doseDepositByPrimary_byOrgan_freespace_1e5runs.dat",
                "geant4_runs/doseDepositByPrimary_byOrgan_Al_5gcm2_1e5runs.dat",
                "geant4_runs/doseDepositByPrimary_byOrgan_Al_20gcm2_1e5runs.dat",
                "geant4_runs/doseDepositByPrimary_byOrgan_Al_40gcm2_1e5runs.dat"]

labels = ["Free space", "5 g cm$^{-2}$", "20 g cm$^{-2}$", "40 g cm$^{-2}$"]
emin = 10
emax = 10000

##########################
# Define other constants #
##########################

organ_names = {"Bladder" : ["137", "138"],
               "Brain" : ["61"],
               "Breast" : ["62", "63", "64", "65"],
               "Colon" : ["76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86"],
               "Esophagus" : ["110"],
               "Heart" : ["87", "88"],
               "Liver" : ["95"],
               "Lungs" : ["96", "97", "98", "99"],
               "Testes" : ["129", "130"],
               "Stomach" : ["72", "73"],
               "Thyroid" : ["132"]}
numbers_to_show = []
for key in organ_names:
    for num in organ_names[key]:
        if (num not in numbers_to_show) and (key in organs_to_show):
            numbers_to_show.append(num)

#########################
# Define some functions #
#########################

# Function to read the .dat file provided by OLTARIS
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

# Function to read json from Geant4 generated data
def file_read_geant4(filename):
    f = open(filename)
    data_init = json.load(f)

    data_final = {}
    for key in data_init:
        data_final[eval(key)] = data_init[key]
    return data_final

####################################################
# Create interpolation functions from OLTARIS data #
####################################################

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

###############################
# Define some other functions #
###############################

# Define weighting functions
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

###################
# Weight the data #
###################

runs = []
for name in files_to_use:
    runs.append(file_read_geant4(name))

# Count the number of each particle
particle_counts = []
for run in runs:
    particle_counts.append({})
    for key in run:
        if key[0] not in particle_counts[-1]:
            particle_counts[-1][key[0]] = 0
        particle_counts[-1][key[0]] += 1

for k in range(len(runs)):
    for key in runs[k]:
        cur_weight = weight(
            E=key[1], n=particle_counts[k][key[0]], nbins=1, xmin=emin, xmax=emax, area=4*np.pi*(180**2), time=365, name=key[0]
        )
        for tissue in runs[k][key]:
            runs[k][key][tissue] *= cur_weight

#################################################
# Add up the doses by tissue (if to be plotted) #
#################################################

dose_by_tissue = []
for run in runs:
    dose_by_tissue.append({})
    for key in run:
        for tissue in run[key]:
            if tissue in numbers_to_show:
                if tissue not in dose_by_tissue[-1]:
                    dose_by_tissue[-1][tissue] = 0.0
                dose_by_tissue[-1][tissue] += run[key][tissue]

##################################
# Rename tissues to actual names #
##################################

for run_num in range(len(runs)):
    for key in organs_to_show:
        for orgID in organ_names[key]:
            if key not in dose_by_tissue[run_num]:
                dose_by_tissue[run_num][key] = 0.0
            dose_by_tissue[run_num][key] += dose_by_tissue[run_num][orgID]
            del dose_by_tissue[run_num][orgID]

########
# Plot #
########

mpl.rcParams['figure.figsize'] = [15,10]

for k in range(len(dose_by_tissue)):
    keys, values = dose_by_tissue[k].keys(), dose_by_tissue[k].values()
    plt.plot(range(len(values)), values, 
             marker='.', 
             markersize=15, 
             label=labels[k])

plt.xticks(range(len(values)), keys, size=15, rotation=90)
plt.yticks(size=15)
plt.ylabel("Dose (Gy/year)", size=20)
plt.legend(fontsize=15)

plt.savefig("dose_by_organ.jpg", dpi=1000)
