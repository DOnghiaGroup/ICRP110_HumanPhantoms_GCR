#include "ICRP110PhantomDoseDeposit.hh"

G4bool ICRP110PhantomDoseDeposit::ProcessHits(G4Step* aStep, G4TouchableHistory* ROHist) {
	G4double edep = aStep->GetTotalEnergyDeposit();
	if ( edep == 0. ) return FALSE;
	
	G4int idx = ((G4TouchableHistory*)
	             (aStep->GetPreStepPoint()->GetTouchable()))
	             ->GetReplicaNumber(indexDepth);
	G4double cubicVolume = ComputeVolume(aStep, idx);
	
	
	G4double density = aStep->GetTrack()->GetStep()->GetPreStepPoint()->GetMaterial()->GetDensity();
	G4double dose    = edep / ( density * cubicVolume );
	dose *= aStep->GetPreStepPoint()->GetWeight(); 
	G4int  index = GetIndex(aStep);
	//EvtMap->add(index,dose);  
	return TRUE;	
}
