#ifndef PHANTOM_DETECTOR_HH
#define PHANTOM_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include <fstream>
#include <set>

// This class implements the phantom as a sensitive detector to get information on events that happen in it

class ICRP110PhantomDetector : public G4VSensitiveDetector {
public:
	ICRP110PhantomDetector(G4String);
	~ICRP110PhantomDetector();
private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	G4bool ParticleFilter(G4ThreeVector, G4ThreeVector);
};

#endif
