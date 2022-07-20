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
	std::map<std::pair<G4String, G4double>, std::map<G4String, G4double>> GetTotalDosesByTissue();
	std::map<std::pair<G4String, G4double>, std::map<G4String, G4double>> GetTotalDosesBySecondary();
private:
	G4int nEvent;
	G4int totalDoseID;
	G4int organNameID;
	G4int secondaryTypeID;
	G4THitsMap<G4double>* eventTotalDose;
	G4THitsMap<G4String>* eventOrganNames;
	G4THitsMap<G4String>* eventSecondaryNames;
	std::map<std::pair<G4String, G4double>, G4double> totalDoses;
	std::map<std::pair<G4String, G4double>, std::map<G4String, G4double>> totalDosesByTissue;
	std::map<std::pair<G4String, G4double>, std::map<G4String, G4double>> totalDosesBySecondary;
};

#endif
