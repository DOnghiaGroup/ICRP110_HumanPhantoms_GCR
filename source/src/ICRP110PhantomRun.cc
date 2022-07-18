#include "ICRP110PhantomRun.hh"
#include "G4SystemOfUnits.hh"

// Upon beginning a run, get the specific detector/scorer we will need
ICRP110PhantomRun::ICRP110PhantomRun() : nEvent(0) {
	G4SDManager* SDM = G4SDManager::GetSDMpointer();
	totalDoseID = SDM -> GetCollectionID("phantomDetector/doseCounter");
}

ICRP110PhantomRun::~ICRP110PhantomRun() {
}

void ICRP110PhantomRun::RecordEvent(const G4Event* evt) {
	// Tally the number of events and get the hits collection for this event
	nEvent++;
	G4HCofThisEvent* HCE = evt -> GetHCofThisEvent();

	// Get the desired quantities from this event (note that these were stored
	// when the primaries were generated, in ICRP110PhantomPrimaryGeneratorAction.cc)
	G4String eventPrimaryName = evt -> GetPrimaryVertex() -> GetPrimary() -> GetParticleDefinition() -> GetParticleName();
	G4double eventPrimaryKE = evt -> GetPrimaryVertex() -> GetPrimary() -> GetKineticEnergy();
	eventTotalDose = (G4THitsMap<G4double>*)(HCE -> GetHC(totalDoseID));

	// Extract information about the dose to get the total dose for this event and store it in newDose
	std::map<G4int, G4double*>* eventDoseMap = eventTotalDose->GetMap();
	std::map<G4int, G4double*>::iterator itr;
	G4double newDose = 0.0;
	for (itr = eventDoseMap->begin(); itr != eventDoseMap->end(); itr++) {
		newDose += *(itr->second);	
	}

	// Store this information in class variables depending on what the primary particle was
	std::pair<G4String, G4double> keyPair;
	keyPair.first = eventPrimaryName;
	keyPair.second = eventPrimaryKE;
	totalDoses[keyPair] += newDose;
}

std::map<std::pair<G4String, G4double>, G4double> ICRP110PhantomRun::GetDoseDeposits() {
	return totalDoses;
}
