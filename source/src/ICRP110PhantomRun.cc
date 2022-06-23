#include "ICRP110PhantomRun.hh"

ICRP110PhantomRun::ICRP110PhantomRun() : nEvent(0) {
	G4SDManager* SDM = G4SDManager::GetSDMpointer();
	totalDoseID = SDM -> GetCollectionID("phantomDetector/doseCounter");
}

ICRP110PhantomRun::~ICRP110PhantomRun() {
}

void ICRP110PhantomRun::RecordEvent(const G4Event* evt) {
	nEvent++;
	G4HCofThisEvent* HCE = evt -> GetHCofThisEvent();

	G4String eventPrimaryName = evt -> GetPrimaryVertex() -> GetPrimary() -> GetParticleDefinition() -> GetParticleName();
	G4double eventPrimaryKE = evt -> GetPrimaryVertex() -> GetPrimary() -> GetKineticEnergy();
	eventTotalDose = (G4THitsMap<G4double>*)(HCE -> GetHC(totalDoseID));

	std::map<G4int, G4double*>* eventDoseMap = eventTotalDose->GetMap();
	std::map<G4int, G4double*>::iterator itr;
	G4double newDose = 0.0;
	for (itr = eventDoseMap->begin(); itr != eventDoseMap->end(); itr++) {
		newDose += *(itr->second);	
	}

	totalDoses[eventPrimaryName] += newDose;
	primaryKEs[eventPrimaryName] = eventPrimaryKE;
}

std::map<G4String, G4double> ICRP110PhantomRun::GetDoseDeposits() {
	return totalDoses;
}

std::map<G4String, G4double> ICRP110PhantomRun::GetPrimaryKEs() {
	return primaryKEs;
}
