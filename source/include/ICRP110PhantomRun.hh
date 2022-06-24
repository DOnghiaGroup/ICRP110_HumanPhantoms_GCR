// This class gets all the information from a run and stores maps where the key values are
// the primary particle, and the values are either the kinetic energy of that particle or
// the total dose delivered by all secondaries.

#ifndef ICRP110PhantomRun_HH
#define ICRP110PhantomRun_HH

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4PrimaryParticle.hh"
#include <map>
#include <utility>

class ICRP110PhantomRun : public G4Run {
public:
	ICRP110PhantomRun();
	~ICRP110PhantomRun();
	virtual void RecordEvent(const G4Event*);
	std::map<std::pair<G4String, G4double>, G4double> GetDoseDeposits();
private:
	G4int nEvent;
	G4int totalDoseID;
	std::map<std::pair<G4String, G4double>, G4double> totalDoses;
	G4THitsMap<G4double>* eventTotalDose;
};

#endif
