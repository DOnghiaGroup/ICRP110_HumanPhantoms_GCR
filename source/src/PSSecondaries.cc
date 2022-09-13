#include "PSSecondaries.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

PSSecondaries::PSSecondaries(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{
}

PSSecondaries::~PSSecondaries()
{;}

G4bool PSSecondaries::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
if (aStep->GetPreStepPoint()->GetMaterial()->GetName() != "Air") {
  G4int index = aStep -> GetTrack() -> GetTrackID();
  if (trackIDsFound.find(index) == trackIDsFound.end()) {
	trackIDsFound.insert(index);
  	EvtMap -> set(index, aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  }

  return TRUE;
} else { return FALSE; }
}

void PSSecondaries::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4String>(GetMultiFunctionalDetector()->GetName(),
                                    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSSecondaries::EndOfEvent(G4HCofThisEvent*)
{;}

void PSSecondaries::clear()
{
  EvtMap->clear();
}
