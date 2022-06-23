#ifndef PHANTOM_DETECTOR_HH
#define PHANTOM_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include <fstream>
#include <vector>
#include "G4GenericMessenger.hh"

class ICRP110PhantomDetector : public G4VSensitiveDetector {
public:
	ICRP110PhantomDetector(G4String);
	~ICRP110PhantomDetector();
private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	G4GenericMessenger* outputMessenger;
	G4String allFileName;
};

#endif
