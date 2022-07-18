// This is just the G4DoseDeposit.cc file with a few modifications
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
// GEANT4 tag $Name: geant4-09-04 $
//
// G4PSDoseDeposit
#include "G4PSDoseDepositMod.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// (Description)
//   This is a primitive scorer class for scoring only energy deposit.
// 
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 2010-07-22   Introduce Unit specification.
// 

G4PSDoseDepositMod::G4PSDoseDepositMod(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{
    SetUnit("Gy");
}

G4PSDoseDepositMod::G4PSDoseDepositMod(G4String name, const G4String& unit,
                                 G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{
    SetUnit(unit);
}

G4PSDoseDepositMod::~G4PSDoseDepositMod()
{;}

G4bool G4PSDoseDepositMod::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
if (aStep->GetPreStepPoint()->GetMaterial()->GetName() != "Air") {
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4int idx = ((G4TouchableHistory*)
               (aStep->GetPreStepPoint()->GetTouchable()))
               ->GetReplicaNumber(indexDepth);
  G4double cubicVolume = ComputeVolume(aStep, idx);


  G4double density = aStep->GetTrack()->GetStep()->GetPreStepPoint()->GetMaterial()->GetDensity();
  G4double dose = edep / (70*kg);  // G4double dose    = edep / ( density * cubicVolume );
  // dose *= aStep->GetPreStepPoint()->GetWeight(); 
  G4int  index = GetIndex(aStep);
  EvtMap->add(index,dose);  

  // G4cout << "E_dep: " << edep/(MeV) << ", Density: " << density/(g/mm3) << ", Volume: " << cubicVolume/(mm3) << ", Dose: " << dose/(MeV/g) << G4endl;

  return TRUE;
} else { return FALSE; }
}

void G4PSDoseDepositMod::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
                                    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void G4PSDoseDepositMod::EndOfEvent(G4HCofThisEvent*)
{;}

void G4PSDoseDepositMod::clear()
{
  EvtMap->clear();
}

void G4PSDoseDepositMod::DrawAll()
{;}

void G4PSDoseDepositMod::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
           << "  dose deposit: " 
           << *(itr->second)/GetUnitValue()
           << " ["<<GetUnit() <<"]"
           << G4endl;
  }
}

void G4PSDoseDepositMod::SetUnit(const G4String& unit)
{
        CheckAndSetUnit(unit,"Dose");
}

G4double G4PSDoseDepositMod::ComputeVolume(G4Step* aStep, G4int idx){

  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VPVParameterisation* physParam = physVol->GetParameterisation();
  G4VSolid* solid = 0;
  if(physParam)
  { // for parameterized volume
    if(idx<0)
    {
      G4ExceptionDescription ED;
      ED << "Incorrect replica number --- GetReplicaNumber : " << idx << G4endl;
      G4Exception("G4PSDoseDepositMod::ComputeVolume","DetPS0004",JustWarning,ED);
    }
    solid = physParam->ComputeSolid(idx, physVol);
    solid->ComputeDimensions(physParam,idx,physVol);
  }
  else
  { // for ordinary volume
    solid = physVol->GetLogicalVolume()->GetSolid();
  }
  
  return solid->GetCubicVolume();
}

std::vector<std::pair<G4String, G4double>> G4PSDoseDepositMod::GetParticlesEnergies() {
	return particlesEnergies;
}
