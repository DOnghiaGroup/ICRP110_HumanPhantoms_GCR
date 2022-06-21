#include "ICRP110PhantomRunAction.hh"

ICRP110PhantomRunAction::ICRP110PhantomRunAction() {

}

ICRP110PhantomRunAction::~ICRP110PhantomRunAction() {

}

G4Run* ICRP110PhantomRunAction::GenerateRun() {
	return (new ICRP110PhantomRun());
}

void ICRP110PhantomRunAction::BeginOfRunAction(const G4Run* aRun) {
}

void ICRP110PhantomRunAction::EndOfRunAction(const G4Run* aRun) {
	ICRP110PhantomRun* theRun = (ICRP110PhantomRun*)aRun;
	G4THitsMap doseDeposit = theRun -> GetDoseDeposit();
}
