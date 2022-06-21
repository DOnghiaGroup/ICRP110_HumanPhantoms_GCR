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
	eventTotalDose = (G4THitsMap<G4double>*)(HCE -> GetHC(totalDoseID));
	totalDose += *eventTotalDose;
}

G4THitsMap<G4double> ICRP110PhantomRun::GetDoseDeposit() {
	return totalDose;
}
