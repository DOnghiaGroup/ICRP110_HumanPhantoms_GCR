#include "ICRP110PhantomDetector.hh"
#include "ICRP110PhantomHit.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {
}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {
}

void ICRP110PhantomDetector::ExportParticleDiff() {
	G4String firstPrimaryName = hitsCollection[0] -> GetPrimaryName();
	G4cout << "yeeeeeeeeeeeeeeeeeeeeeehaw" << G4endl;
	G4cout << firstPrimaryName << G4endl;
}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
//	G4String outfilename = "secondary_particle_info.csv";
//
//	// Get information on the particle
//	G4Track* track = aStep -> GetTrack();
//	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
//	const std::vector<const G4Track*>* secondaryList = aStep -> GetSecondaryInCurrentStep();
//
//	// Subtract one of these particles from the count
//	if (particleDiff[particleName] != 0) { // Checks if key already in map
//		particleDiff[particleName] = particleDiff[particleName] - 1;
//	} else {
//		particleDiff[particleName] = -1;
//	}
//
//	// Add all secondaries to the particle counts
//	if ((secondaryList -> size()) > 0) {
//		for (int i = 0; i < secondaryList->size(); i++) {
//			G4String curName = secondaryList -> at(i) -> GetParticleDefinition() -> GetParticleName();
//			if (particleDiff[curName] != 0) { // Checks if key already in map
//				particleDiff[curName] = particleDiff[curName] + 1;
//			} else {
//				particleDiff[curName] = 1;
//			}
//		}
//	}
//
//	std::map<G4String, int>::iterator itr;
//	for (itr = particleDiff.begin(); itr != particleDiff.end(); itr++) {
//		G4cout << itr->first << "," << std::to_string(itr->second) << G4endl;
//	}

	G4Track* track = aStep -> GetTrack();
	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
	const std::vector<const G4Track*>* secondaryList = aStep -> GetSecondaryInCurrentStep();

	ICRP110PhantomHit* curHit = new ICRP110PhantomHit();

	curHit -> SetPrimaryName(particleName);
	curHit -> SetSecondaryList(secondaryList);

	hitsCollection.push_back(curHit);

	return true;
}
