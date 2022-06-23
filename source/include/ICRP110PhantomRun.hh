#ifndef ICRP110PhantomRun_HH
#define ICRP110PhantomRun_HH

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4PrimaryParticle.hh"
#include <map>

class ICRP110PhantomRun : public G4Run {
public:
	ICRP110PhantomRun();
	~ICRP110PhantomRun();
	virtual void RecordEvent(const G4Event*);
	std::map<G4String, G4double> GetDoseDeposits();
	std::map<G4String, G4double> GetPrimaryKEs();
private:
	G4int nEvent;
	G4int totalDoseID;
	std::map<G4String, G4double> totalDoses;
	G4THitsMap<G4double>* eventTotalDose;
	std::map<G4String, G4double> primaryKEs;
};

#endif
