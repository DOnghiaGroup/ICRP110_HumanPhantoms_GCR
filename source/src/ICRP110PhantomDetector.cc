#include "ICRP110PhantomDetector.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
	G4String outfilename = "secondary_particle_info.csv";

	// Get information on the particle
	G4Track* track = aStep -> GetTrack();
	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
	const std::vector<const G4Track*>* secondaryList = aStep -> GetSecondaryInCurrentStep();

	// Test
	//if ((secondaryList -> size()) > 0) {
	//	G4String secondaryFirstName = secondaryList -> at(0) -> GetParticleDefinition() -> GetParticleName();
	//}
	G4String secondaryNameList = "";
	if ((secondaryList -> size()) > 0) {
		for (int i = 0; i < secondaryList->size(); i++) {
			G4String curName = secondaryList -> at(i) -> GetParticleDefinition() -> GetParticleName();
			if (i == 0) {
				secondaryNameList = secondaryNameList + " -> " + curName;
			} else {
				secondaryNameList = secondaryNameList + ", " + curName;
			}
		}
		G4cout << particleName << secondaryNameList << G4endl;
	}

	return true;
}
