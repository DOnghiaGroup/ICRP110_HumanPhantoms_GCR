#include "RunFluxCount.hh"

RunFluxCount::RunFluxCount() : nEvent(0) {
	G4SDManager* SDM = G4SDManager::GetSDMpointer();
	protonSurfaceFluxID = SDM -> GetCollectionID("fluxDetectorInsideD/protonSurfaceFlux");
}

RunFluxCount::~RunFluxCount() {}

void RunFluxCount::RecordEvent(const G4Event* evt) {
	nEvent++;
	G4HCofThisEvent* HCE = evt -> GetHCofThisEvent();
	eventProtonSurfaceFlux = (G4THitsMap<G4double>*)(HCE -> GetHC(protonSurfaceFluxID));
	protonSurfaceFlux += *eventProtonSurfaceFlux;
}
