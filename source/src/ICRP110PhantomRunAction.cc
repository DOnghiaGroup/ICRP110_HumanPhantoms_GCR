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
	// Get information about the run from the run class
	ICRP110PhantomRun* theRun = (ICRP110PhantomRun*)aRun;
	std::map<G4String, G4double> totalDoses = theRun -> GetDoseDeposits();
	std::map<G4String, G4double> primaryKEs = theRun -> GetPrimaryKEs();

	// For all of the primary particles, store this information
	for (auto itr = primaryKEs.begin(); itr != primaryKEs.end(); itr++) {
		G4String eventPrimaryName = itr->first;
		G4double eventPrimaryKE = itr->second;
		G4double eventDose = totalDoses[eventPrimaryName];

		std::ofstream ofile;
		ofile.open(fileName, std::ios_base::app);
		ofile << eventPrimaryName << "," << eventPrimaryKE << "," << eventDose << "\n";
		ofile.close();
	}
}
