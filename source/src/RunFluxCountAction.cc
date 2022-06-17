#include "RunFluxCountAction.hh"

RunFluxCountAction::RunFluxCountAction() {}

RunFluxCountAction::~RunFluxCountAction() {}

G4Run* RunFluxCountAction::GenerateRun() {
	return (new RunFluxCount());
}

void RunFluxCountAction::EndOfRunAction(const G4Run* aRun) {
	RunFluxCount* theRun = (RunFluxCount*)aRun;
}
