#ifndef PHANTOM_DETECTOR_HH
#define PHANTOM_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include <fstream>
#include <vector>
#include <map>
#include "ICRP110PhantomHit.hh"

// This class implements some LogicalVolume as a sensitive detector so we can process events inside of it

class ICRP110PhantomDetector : public G4VSensitiveDetector {
public:
	ICRP110PhantomDetector(G4String);
	~ICRP110PhantomDetector();
	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);
private:
	ICRP110PhantomHitsCollection* hitsCollection;
	G4int collectionID;
};

#endif
