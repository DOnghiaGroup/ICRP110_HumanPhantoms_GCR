#include "ICRP110PhantomDetector.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
	G4Track* track = aStep -> GetTrack();

	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();

	G4cout << "Particle name: " << particleName << G4endl;


}
