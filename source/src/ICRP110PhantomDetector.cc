#include "ICRP110PhantomDetector.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {
	// Define messengers for macro files
	outputMessenger = new G4GenericMessenger(this, "/output/", "Run Action");

	outputMessenger -> DeclareProperty("allFileName", allFileName, "Name of output file for all particles");
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
	G4String material = aStep -> GetTrack() -> GetMaterial() -> GetName();

	// Output to file
	if (material != "Air") {
		std::ofstream ofile;
		ofile.open(allFileName, std::ios_base::app);
		ofile << particleName << "," << initKE << "," << position[0] << "," << position[1] << "," << position[2] << "," << energyDep << "\n"; 
		ofile.close();
	}

	return true;
}
