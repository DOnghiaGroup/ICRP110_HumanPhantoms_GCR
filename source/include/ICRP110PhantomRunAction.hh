#ifndef ICRP110PHANTOMRUNACTION_HH
#define ICRP110PHANTOMRUNACTION_HH

#include "G4UserRunAction.hh"
#include "ICRP110PhantomRun.hh"
#include "G4THitsMap.hh"
#include "G4Run.hh"

class ICRP110PhantomRunAction : public G4UserRunAction {
public:
	ICRP110PhantomRunAction();
	~ICRP110PhantomRunAction();

	virtual G4Run* GenerateRun();
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};

#endif
