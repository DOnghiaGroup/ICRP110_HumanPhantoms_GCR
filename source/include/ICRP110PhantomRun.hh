#ifndef ICRP110PhantomRun_HH
#define ICRP110PhantomRun_HH

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4PrimaryParticle.hh"

class ICRP110PhantomRun : public G4Run {
public:
	ICRP110PhantomRun();
	~ICRP110PhantomRun();
	virtual void RecordEvent(const G4Event*);
	G4THitsMap<G4double> GetDoseDeposit();
	G4double GetPrimaryKE();
	G4String GetPrimaryName();
private:
	G4int nEvent;
	G4int totalDoseID;
	G4THitsMap<G4double> totalDose;
	G4THitsMap<G4double>* eventTotalDose;
	G4double primaryKE;
	G4String primaryName;
};

#endif
