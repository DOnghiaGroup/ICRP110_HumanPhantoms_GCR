#include "ICRP110PhantomDetector.hh"
#include "ICRP110PhantomHit.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name),
								collectionID(-1) {
	collectionName.insert("secondaries");
}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {
	delete hitsCollection;
}

// The following function defines what to do at the beginning of an event
void ICRP110PhantomDetector::Initialize(G4HCofThisEvent* HCE) {
	G4cout << "Initializing detector event . . . " << G4endl;

	if(collectionID<0) collectionID = GetCollectionID(0);

	hitsCollection = new ICRP110PhantomHitsCollection("ShieldDetector", collectionName[0]);

	HCE -> AddHitsCollection(collectionID, hitsCollection);
}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
	// Get info about the particle
	G4Track* track = aStep -> GetTrack();
	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
	const std::vector<const G4Track*>* secondaryList = aStep -> GetSecondaryInCurrentStep();

	// Create a hit object for this hit
	ICRP110PhantomHit* curHit = new ICRP110PhantomHit();

	// Fill info about the hit into the hit object
	curHit -> SetPrimaryName(particleName);
	curHit -> SetSecondaryList(secondaryList);

	// Store the hit object
	hitsCollection -> insert(curHit);

	return true; // The return statement here is obsolete, according to Geant4 documentation
}

// The following function defines what to do at the end of an event
void ICRP110PhantomDetector::EndOfEvent(G4HCofThisEvent* HCE) {
	G4cout << "End of detector event . . . " << G4endl;
}
