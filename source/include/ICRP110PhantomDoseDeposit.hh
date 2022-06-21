#ifndef ICRP110PhantomDoseDeposit_HH
#define ICRP110PhantomDoseDeposit_HH

#include "G4PSDoseDeposit.hh"

class ICRP110PhantomDoseDeposit : public G4PSDoseDeposit {
private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
};

#endif
