
# Introduction
           
This is a Geant4 application based on the ICRP110_HumanPhantoms example (please see the README in the source directory 
for the original information). Here, we've added functionality to:

  - Generate isotropic radiation on a sphere sampled along a lognormal distribution (simulating galactic cosmic rays)
  - Track deposited dose as a function of generated primary particle and its energy
  - Track deposited dose as a function of organID
  - Output secondary particle flux through the human as a function of generated primary particle and energy
  
Below is information on how to access output files for each of these metrics. See also "Useful Geant4 Information" in
the main directory for a detailed explanation of how this was modified from the original example.
        
# Macro Commands

(see build/male_space.in as example)
           
## Select output file type

The user should specify /output/fileName to define the name of the file with output values. Then, the user should specify /output/outputType; options are "basic," "organ," and "secondaries." First, "basic" outputs a .csv file organized with columns "primary particle name, initial kinetic energy (MeV), dose (Gy) imparted by all secondaries."
The "organ" option outputs a dictionary (each entry being one event) with:
    
  - keys: primary particle type, initial kinetic energy
  - values: a dictionary of organID and dose imparted to that organID

The organIDs are as specified in the ICRP110 human phantom. "Secondaries" outputs in the same format as "organ" but with the keys being the name of the secondary particle and the values being the flux of that particle through the human phantom.

Note: For the "secondary" and "organ" output file types, it is recommended that the user manually add a "{" to the beginning of the file, delete the last comma in the file, and add a "}" to the end of the file. This lets python automaticaly read the data in json format.

-----------

Contact: bderieg@deriegfamily.com
