#include "ICRP110PhantomRunAction.hh"

ICRP110PhantomRunAction::ICRP110PhantomRunAction() : fileName("doseDepositByPrimary.csv") {

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
	G4double runDoseTotal = 0.0;
	G4THitsMap doseDeposit = theRun -> GetDoseDeposit();
	std::map<G4int, G4double*>* dosesMap = doseDeposit.GetMap();
	std::map<G4int, G4double*>::iterator itr;
	if (theRun->GetPrimaryKE() > 0) {
		for (itr = dosesMap->begin(); itr != dosesMap->end(); itr++) {
			runDoseTotal += *(itr->second);
		}
		std::ofstream ofile;
		ofile.open(fileName, std::ios_base::app);
		ofile << theRun->GetPrimaryName() << "," << theRun->GetPrimaryKE() << "," << runDoseTotal << "\n";
		ofile.close();
	}
}
