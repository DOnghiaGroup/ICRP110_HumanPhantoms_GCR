#include "ICRP110PhantomRun.hh"

ICRP110PhantomRun::ICRP110PhantomRun() : nEvent(0), primaryKE(-1), primaryName("err_name_not_found") {
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
	primaryKE = evt -> GetPrimaryVertex() -> GetPrimary() -> GetKineticEnergy();
	primaryName = evt -> GetPrimaryVertex() -> GetPrimary() -> GetParticleDefinition() -> GetParticleName();
}

G4THitsMap<G4double> ICRP110PhantomRun::GetDoseDeposit() {
	return totalDose;
}

G4double ICRP110PhantomRun::GetPrimaryKE() {
	return primaryKE;
}

G4String ICRP110PhantomRun::GetPrimaryName() {
	return primaryName;
}
