#include "ICRP110PhantomDetector.hh"

ICRP110PhantomDetector::ICRP110PhantomDetector(G4String name) : G4VSensitiveDetector(name) {}

ICRP110PhantomDetector::~ICRP110PhantomDetector() {
}

// The following function defines what to do on a hit
G4bool ICRP110PhantomDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
	G4String outfilename = "secondary_particle_info.csv";

	// Get information on the particle
	G4Track* track = aStep -> GetTrack();
	G4String particleName = track -> GetParticleDefinition() -> GetParticleName();
	G4ThreeVector primaryMomentumInit = aStep -> GetPreStepPoint() -> GetMomentumDirection();
	G4ThreeVector primaryPositionInit = aStep -> GetPreStepPoint() -> GetPosition();
	G4ThreeVector primaryMomentumFinal = aStep -> GetPostStepPoint() -> GetMomentumDirection();
	G4ThreeVector primaryPositionFinal = aStep -> GetPostStepPoint() -> GetPosition();
	G4double primaryEnergyInit = aStep -> GetPreStepPoint() -> GetKineticEnergy();
	G4double primaryEnergyFinal = aStep -> GetPostStepPoint() -> GetKineticEnergy();
	const std::vector<const G4Track*>* secondaryList = aStep -> GetSecondaryInCurrentStep();

	if (ParticleFilter(primaryMomentumFinal, primaryPositionFinal) && ParticleFilter(primaryMomentumInit, primaryPositionInit)) {
		std::ofstream ofile;
		ofile.open(outfilename, std::ios_base::app);
		ofile << particleName << ",-1," << primaryEnergyInit << "\n";
		ofile << particleName << ",+1," << primaryEnergyFinal << "\n";
		ofile.close();
	} else if (ParticleFilter(primaryMomentumFinal, primaryPositionFinal) && !ParticleFilter(primaryMomentumInit, primaryPositionInit)) {
		std::ofstream ofile;
		ofile.open(outfilename, std::ios_base::app);
		ofile << particleName << ",+1," << primaryEnergyFinal << "\n";
		ofile.close();
	} else {
		std::ofstream ofile;
		ofile.open(outfilename, std::ios_base::app);
		ofile << particleName << ",-1," << primaryEnergyInit << "\n";
		ofile.close();
	}

	// Write a string containing a list of the primary particle followed by its secondaries
	G4String secondaryNameList = particleName;
	if ((secondaryList -> size()) > 0) {
		for (int i = 0; i < secondaryList->size(); i++) {
			G4String curName = secondaryList -> at(i) -> GetParticleDefinition() -> GetParticleName();
			G4ThreeVector curMomentum = secondaryList -> at(i) -> GetMomentumDirection();
			G4ThreeVector curPosition = secondaryList -> at(i) -> GetPosition();
			G4double curEnergy = secondaryList -> at(i) -> GetKineticEnergy();

			if (ParticleFilter(curMomentum, curPosition)) {
				std::ofstream ofile;
				ofile.open(outfilename, std::ios_base::app);
				ofile << curName << ",+1," << curEnergy << "\n";
				ofile.close();
			}
		}
	}

	return true;
}

// Determines if a particle is moving inwards or outwards
G4bool ICRP110PhantomDetector::ParticleFilter(G4ThreeVector momentum, G4ThreeVector position) {
	G4double oldPosLength = std::sqrt(position[0]*position[0] 
				+ position[1]*position[1] 
				+ position[2]*position[2]);
	G4double newPosLength = std::sqrt((position[0]+momentum[0])*(position[0]+momentum[0]) 
				+ (position[1]+momentum[1])*(position[1]+momentum[1]) 
				+ (position[2]+momentum[2])*(position[2]+momentum[2]));

	if (newPosLength < oldPosLength) {
		return true;
	} else {
		return false;
	}
}
