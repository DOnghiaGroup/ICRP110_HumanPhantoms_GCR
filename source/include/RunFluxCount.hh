#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

class RunFluxCount : public G4Run {
public:
	RunFluxCount();
	~RunFluxCount();
	virtual void RecordEvent(const G4Event*);
	G4THitsMap<G4double> GetProtonFlux();
private:
	G4int nEvent;
	G4int protonSurfaceFluxID;
	G4THitsMap<G4double> protonSurfaceFlux;
	G4THitsMap<G4double>* eventProtonSurfaceFlux;
};
