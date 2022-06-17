#include "G4UserRunAction.hh"
#include "RunFluxCount.hh"
#include "G4THitsMap.hh"

class RunFluxCountAction : public G4UserRunAction {
public:
	RunFluxCountAction();
	~RunFluxCountAction();
	virtual G4Run* GenerateRun();
	virtual void EndOfRunAction(const G4Run*);
};
