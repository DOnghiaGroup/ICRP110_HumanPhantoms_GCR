#ifndef ICRP110PhantomRun_HH
#define ICRP110PhantomRun_HH

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

class ICRP110PhantomRun : public G4Run {
public:
	ICRP110PhantomRun();
	~ICRP110PhantomRun();
	virtual void RecordEvent(const G4Event*);
private:
	G4int nEvent;
	G4int totalDoseID;
	G4THitsMap<G4double> totalDose;
	G4THitsMap<G4double>* eventTotalDose;
};

#endif
