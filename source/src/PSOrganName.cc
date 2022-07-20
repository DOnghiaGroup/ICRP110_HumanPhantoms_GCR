#include "PSOrganName.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

PSOrganName::PSOrganName(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{
}

PSOrganName::~PSOrganName()
{;}

G4bool PSOrganName::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
if (aStep->GetPreStepPoint()->GetMaterial()->GetName() != "Air") {
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4int  index = GetIndex(aStep);
  G4String organName = aStep -> GetTrack() -> GetStep() -> GetPreStepPoint() -> GetMaterial() -> GetName();
  EvtMap->set(index, organName);  

  return TRUE;
} else { return FALSE; }
}

void PSOrganName::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4String>(GetMultiFunctionalDetector()->GetName(),
                                    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSOrganName::EndOfEvent(G4HCofThisEvent*)
{;}

void PSOrganName::clear()
{
  EvtMap->clear();
}
