           ====================================
                        Introduction
           ====================================
           
This is a Geant4 application based on the ICRP110_HumanPhantoms example (please see the README in the source directory 
for the original information). Here, we've added functionality to:

  - Generate isotropic radiation on a sphere sampled along a lognormal distribution (simulating galactic cosmic rays)
  - Track deposited dose as a function of: 
        - generated primary particle and its energy
        - organID
        - secondary particle and location
  
Below is information on how to access output files for each of these metrics. See also "Useful Geant4 Information" in
the main directory for a detailed explanation of how this was modified from the original example.
        
           ======================================
                        Macro Commands
           ======================================
            (see build/male_space.in as example)
           
       ===== Select output file type =====
At the beginning of the macro file, the user should specify /output/detectorType. Options are "primaries" or 
"all." First, "primaries" specifies that the multifunctional detector should be used to output a file with one 
line of code for each event (exact information depends on further specifications). Second, "all" specifies that
one line will be output to a file for every event inside of a sensitive detector attached to the phantom. This takes a
while to run, but is useful for short simulations for outputting specific location information of every hit. Additionally,
the user can also pass "none" to not output anything.

The file name should also be specified with either /output/allFileName or /output/primariesFileName depending on the chosen
options above.

       === Select primaries file type ===
Before running, the user should specify /output/primariesFileType. Options are "basic" and "organ." First, "basic" outputs
a .csv file organized with columns "primary particle name, initial kinetic energy (MeV), dose (Gy) imparted by all secondaries."
The "organ" option outputs a dictionary (each entry being one event) with:
    
    - keys: primary particle type, initial kinetic energy
    - values: a dictionary of organID and dose imparted to that organID

The organIDs are as specified in the ICRP110 human phantom.

===================================================================

Contact: bderieg@deriegfamily.com
