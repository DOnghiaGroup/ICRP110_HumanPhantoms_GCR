#include "ICRP110PhantomDetector.hh"
#include "G4SystemOfUnits.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {
	// Define messengers for macro files
	outputMessenger = new G4GenericMessenger(this, "/output/", "Run Action");

	outputMessenger -> DeclareProperty("allFileName", allFileName, "Name of output file for all particles");
	outputMessenger -> DeclareProperty("allDetectorType", allDetectorType, "Type of detector for the sensitive detector");
	allDetectorType = "positions";
	allFileName = "unnamed_output_file.csv";
}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {
	delete outputMessenger;
}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
	// Get information on the particle
	G4Track* track = aStep -> GetTrack();
	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
	G4double initKE = aStep -> GetPreStepPoint() -> GetKineticEnergy();
	G4ThreeVector position = aStep -> GetPreStepPoint() -> GetPosition();
	G4double energyDep = aStep -> GetTotalEnergyDeposit();
	G4String material = aStep -> GetPreStepPoint() -> GetMaterial() -> GetName();

	// Output to file
	if (allDetectorType == "positions") {
		if (material != "Air") {
			std::ofstream ofile;
			ofile.open(allFileName, std::ios_base::app);
			ofile << particleName << "," << initKE << "\n"; // << "," << position[0] << "," << position[1] << "," << position[2] << "," << energyDep << "\n"; 
			ofile.close();
		}
	}
	else if (allDetectorType == "organDoses") {
		if (material != "Air") {
			std::ofstream ofile;
			ofile.open(allFileName, std::ios_base::app);
			// Dose in Gy
			ofile << material << "," << (energyDep/(MeV))*(1.602*pow(10,-13))/70 << "\n"; 
			ofile.close();
		}
	}
	else if (allDetectorType == "secondaryDoses") {
		if (material != "Air") {
			std::ofstream ofile;
			ofile.open(allFileName, std::ios_base::app);
			// Dose in Gy
			ofile << particleName << "," << (energyDep/(MeV))*(1.602*pow(10,-13))/70 << "\n"; 
			ofile.close();
		}
	}

	return true;
}
