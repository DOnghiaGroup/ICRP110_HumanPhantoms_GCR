#ifndef ICRP110PhantomHit_H
#define ICRP110PhantomHit_H

#include "G4VHit.hh"
#include <vector>
#include "G4Track.hh"
#include "G4THitsCollection.hh"

class ICRP110PhantomHit : public G4VHit {
public:
	ICRP110PhantomHit();
	virtual ~ICRP110PhantomHit();
	inline void SetPrimaryName(G4String prim) { primaryName = prim; }
	inline void SetSecondaryList(const std::vector<const G4Track*>* sec) { secondaryList = sec; }
	inline G4String GetPrimaryName() { return primaryName; }
private:
	G4String primaryName;
	const std::vector<const G4Track*>* secondaryList;
};

// Define the containers for these hits
typedef G4THitsCollection<ICRP110PhantomHit> ICRP110PhantomHitsCollection;

#endif
