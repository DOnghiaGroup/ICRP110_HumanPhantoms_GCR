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
	void ExportParticleDiff();
private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	std::map<G4String, G4int> particleDiff;
	std::vector<ICRP110PhantomHit*> hitsCollection;
};

#endif
