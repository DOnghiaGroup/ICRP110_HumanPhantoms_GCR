// This class defines what to do at the beginning and end of a given run. For example,
// it can store information which was generated during the run in the run class.

#ifndef ICRP110PHANTOMRUNACTION_HH
#define ICRP110PHANTOMRUNACTION_HH

#include "G4UserRunAction.hh"
#include "ICRP110PhantomRun.hh"
#include "G4THitsMap.hh"
#include "G4Run.hh"
#include <map>
#include "G4VTrajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4PrimaryParticle.hh"
#include <fstream>
#include "G4GenericMessenger.hh"
#include <utility>

class ICRP110PhantomRunAction : public G4UserRunAction {
public:
	ICRP110PhantomRunAction();
	~ICRP110PhantomRunAction();

	virtual G4Run* GenerateRun();
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
private:
	G4String primariesFileName;
	G4String primariesFileType;
	G4GenericMessenger* outputMessenger;
};

#endif
