#include "ICRP110PhantomRunAction.hh"
#include "G4SystemOfUnits.hh"

ICRP110PhantomRunAction::ICRP110PhantomRunAction() {
	// Define messengers for macro files
	outputMessenger = new G4GenericMessenger(this, "/output/", "Run Action");

	outputMessenger -> DeclareProperty("primariesFileName", primariesFileName, "Name of output file for primaries");
	outputMessenger -> DeclareProperty("primariesFileType", primariesFileType, "What to output (basic, organ)?");
	primariesFileName = "unnamed_output_file.csv";
	primariesFileType = "basic";
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
	std::map<std::pair<G4String, G4double>, std::map<G4String, G4double>> totalDosesByTissue = theRun -> GetTotalDosesByTissue();

	// For all of the primary particles, store this information
	if (primariesFileType == "basic") { for (auto itr = totalDoses.begin(); itr != totalDoses.end(); itr++) {
		G4String eventPrimaryName = itr->first.first;
		G4double eventPrimaryKE = itr->first.second;
		G4double eventDose = totalDoses[itr->first];

		std::ofstream ofile;
		ofile.open(primariesFileName, std::ios_base::app);
		// Notice: convert dose to gray on next line
		ofile << eventPrimaryName << "," << eventPrimaryKE << "," << (eventDose/(MeV/g))*(1.602*pow(10,-10)) << "\n";
		ofile.close();
	}}

	else if (primariesFileType == "organ") { for (auto itr = totalDosesByTissue.begin(); itr != totalDosesByTissue.end(); itr++) {
		G4String eventPrimaryName = itr->first.first;
		G4double eventPrimaryKE = itr->first.second;

		std::ofstream ofile;
		ofile.open(primariesFileName, std::ios_base::app);
		ofile << "{ " << eventPrimaryName << ", " << eventPrimaryKE << " } : { ";

		G4int formatItr = 0;
		for (auto tissueItr = itr->second.begin(); tissueItr != itr->second.end(); tissueItr++) {
			G4String tissueName = tissueItr->first;
			// Notice: convert dose to gray on next line
			G4double tissueDose = (tissueItr->second/(MeV/g))*(1.602*pow(10,-10));

			if (formatItr != 0) { ofile << ", "; } 
			ofile << "\"" << tissueName << "\" : " << tissueDose;

			formatItr += 1;
		}

		ofile << " }\n";
		ofile.close();
	}}
}
