#ifndef ICRP110PHANTOMRUNACTION_HH
#define ICRP110PHANTOMRUNACTION_HH

#include "G4UserRunAction.hh"

class ICRP110PhantomRunAction : public G4UserRunAction {
public:
	ICRP110PhantomRunAction();
	~ICRP110PhantomRunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};

#endif
