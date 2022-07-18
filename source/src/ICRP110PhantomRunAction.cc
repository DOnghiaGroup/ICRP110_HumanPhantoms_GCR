#include "ICRP110PhantomRunAction.hh"
#include "G4SystemOfUnits.hh"

ICRP110PhantomRunAction::ICRP110PhantomRunAction() {
	// Define messengers for macro files
	outputMessenger = new G4GenericMessenger(this, "/output/", "Run Action");

	outputMessenger -> DeclareProperty("primariesFileName", primariesFileName, "Name of output file for primaries");
	primariesFileName = "unnamed_output_file.csv";
}

ICRP110PhantomRunAction::~ICRP110PhantomRunAction() {
	delete outputMessenger;
}

G4Run* ICRP110PhantomRunAction::GenerateRun() {
	return (new ICRP110PhantomRun());
}

void ICRP110PhantomRunAction::BeginOfRunAction(const G4Run* aRun) {
}

void ICRP110PhantomRunAction::EndOfRunAction(const G4Run* aRun) {
	// Get information about the run from the run class
	ICRP110PhantomRun* theRun = (ICRP110PhantomRun*)aRun;
	std::map<std::pair<G4String, G4double>, G4double> totalDoses = theRun -> GetDoseDeposits();

	// For all of the primary particles, store this information
	for (auto itr = totalDoses.begin(); itr != totalDoses.end(); itr++) {
		G4String eventPrimaryName = itr->first.first;
		G4double eventPrimaryKE = itr->first.second;
		G4double eventDose = totalDoses[itr->first];

		std::ofstream ofile;
		ofile.open(primariesFileName, std::ios_base::app);
		// Notice: convert dose to gray on next line
		ofile << eventPrimaryName << "," << eventPrimaryKE << "," << (eventDose/(MeV/g))*(1.602*pow(10,-10)) << "\n";
		ofile.close();
	}
}
