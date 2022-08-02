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
#include "ICRP110PhantomNestedParameterisation.hh"
#include <cmath>

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
    outputMessenger = new G4GenericMessenger(this, "/output/", "Run Action");
    outputMessenger -> DeclareProperty("weight", weight, "Weight particles to provide equivalent dose (bool)?");
    weight = false;
}

G4PSDoseDepositMod::G4PSDoseDepositMod(G4String name, const G4String& unit,
                                 G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{
    SetUnit(unit);
}

G4PSDoseDepositMod::~G4PSDoseDepositMod() {
	delete outputMessenger;
}

G4bool G4PSDoseDepositMod::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
if (aStep->GetPreStepPoint()->GetMaterial()->GetName() != "Air") {
  G4double edep = aStep->GetTotalEnergyDeposit();

  if ( edep == 0. ) return FALSE;

  // G4double dose = edep / (70*kg);  // G4double dose    = edep / ( density * cubicVolume );
  G4double orgID = std::stoi(aStep->GetPreStepPoint()->GetMaterial()->GetName());
  G4double organMass = IDToMass(orgID);
  if (organMass == 0.) return FALSE;
  G4double dose = 0.0;
  if (weight) {
  	dose = GetRBE(aStep) * edep / (organMass*g);
  } else {
  	dose = edep / (organMass*g);
  }
  G4int  index = GetIndex(aStep);
  EvtMap->add(index,dose);  

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

// FIXME: Currently assumes the phantom is male
G4double G4PSDoseDepositMod::IDToMass(G4int id) {
	if (id==0) { return 0; }
	if (id==1) { return 7; }
	if (id==2) { return 7; }
	if (id==3) { return 11.03; }
	if (id==4) { return 28.41; }
	if (id==5) { return 30.73; }
	if (id==6) { return 5.1; }
	if (id==7) { return 10.01; }
	if (id==8) { return 66.31; }
	if (id==9) { return 0.85; }
	if (id==10) { return 271.93; }
	if (id==11) { return 15.72; }
	if (id==12) { return 83.04; }
	if (id==13) { return 135.26; }
	if (id==14) { return 184.86; }
	if (id==15) { return 33.48; }
	if (id==16) { return 128.03; }
	if (id==17) { return 59.73; }
	if (id==18) { return 37.13; }
	if (id==19) { return 270.8; }
	if (id==20) { return 181.91; }
	if (id==21) { return 22.67; }
	if (id==22) { return 179.74; }
	if (id==23) { return 139.59; }
	if (id==24) { return 47.78; }
	if (id==25) { return 53.06; }
	if (id==26) { return 562.85; }
	if (id==27) { return 451.06; }
	if (id==28) { return 261.68; }
	if (id==29) { return 472.05; }
	if (id==30) { return 25.78; }
	if (id==31) { return 294.09; }
	if (id==32) { return 438.57; }
	if (id==33) { return 80.89; }
	if (id==34) { return 531.35; }
	if (id==35) { return 729.38; }
	if (id==36) { return 78.67; }
	if (id==37) { return 232.56; }
	if (id==38) { return 507.78; }
	if (id==39) { return 76.12; }
	if (id==40) { return 73.9; }
	if (id==41) { return 398.62; }
	if (id==42) { return 681.18; }
	if (id==43) { return 365.16; }
	if (id==44) { return 520.06; }
	if (id==45) { return 221.13; }
	if (id==46) { return 192.21; }
	if (id==47) { return 102.92; }
	if (id==48) { return 73.55; }
	if (id==49) { return 286.58; }
	if (id==50) { return 335.34; }
	if (id==51) { return 186.19; }
	if (id==52) { return 302.07; }
	if (id==53) { return 109.23; }
	if (id==54) { return 173.51; }
	if (id==55) { return 9.89; }
	if (id==56) { return 56.31; }
	if (id==57) { return 15.96; }
	if (id==58) { return 88.67; }
	if (id==59) { return 5.91; }
	if (id==60) { return 34.73; }
	if (id==61) { return 1450; }
	if (id==62) { return 7.5; }
	if (id==63) { return 4.99; }
	if (id==64) { return 7.5; }
	if (id==65) { return 4.99; }
	if (id==66) { return 0.19; }
	if (id==67) { return 7.29; }
	if (id==68) { return 0.19; }
	if (id==69) { return 7.33; }
	if (id==70) { return 13.92; }
	if (id==71) { return 54.08; }
	if (id==72) { return 149.99; }
	if (id==73) { return 250.01; }
	if (id==74) { return 650; }
	if (id==75) { return 350.02; }
	if (id==76) { return 89.99; }
	if (id==77) { return 55.02; }
	if (id==78) { return 60; }
	if (id==79) { return 95; }
	if (id==80) { return 60; }
	if (id==81) { return 40.01; }
	if (id==82) { return 89.99; }
	if (id==83) { return 35; }
	if (id==84) { return 40.01; }
	if (id==85) { return 74.97; }
	if (id==86) { return 29.98; }
	if (id==87) { return 329.98; }
	if (id==88) { return 510.01; }
	if (id==89) { return 107.12; }
	if (id==90) { return 38.25; }
	if (id==91) { return 7.63; }
	if (id==92) { return 109.92; }
	if (id==93) { return 39.25; }
	if (id==94) { return 7.87; }
	if (id==95) { return 1800.01; }
	if (id==96) { return 79.01; }
	if (id==97) { return 477.74; }
	if (id==98) { return 71.54; }
	if (id==99) { return 580.08; }
	if (id==100) { return 2.26; }
	if (id==101) { return 6.4; }
	if (id==102) { return 5.98; }
	if (id==103) { return 104.4; }
	if (id==104) { return 7.83; }
	if (id==105) { return 11.1; }
	if (id==106) { return 1217.81; }
	if (id==107) { return 15006.82; }
	if (id==108) { return 2750.53; }
	if (id==109) { return 10024.97; }
	if (id==110) { return 40.01; }
	if (id==111) { return 0; }
	if (id==112) { return 0; }
	if (id==113) { return 140; }
	if (id==114) { return 0.6; }
	if (id==115) { return 17.01; }
	if (id==116) { return 1048.51; }
	if (id==117) { return 12370.04; }
	if (id==118) { return 1643.47; }
	if (id==119) { return 5396.47; }
	if (id==120) { return 42.49; }
	if (id==121) { return 42.49; }
	if (id==122) { return 291.34; }
	if (id==123) { return 1465.21; }
	if (id==124) { return 639.03; }
	if (id==125) { return 1332.43; }
	if (id==126) { return 36.62; }
	if (id==127) { return 149.99; }
	if (id==128) { return 50.04; }
	if (id==129) { return 17.48; }
	if (id==130) { return 17.52; }
	if (id==131) { return 24.99; }
	if (id==132) { return 19.99; }
	if (id==133) { return 42.28; }
	if (id==134) { return 3.01; }
	if (id==135) { return 8.51; }
	if (id==136) { return 7.49; }
	if (id==137) { return 50.01; }
	if (id==138) { return 200; }
	if (id==139) { return 0; }
	if (id==140) { return 0.2; }
	if (id==141) { return 154.3; }
}

G4double G4PSDoseDepositMod::GetRBE(G4Step* aStep) {
	G4cout << "I am weighting!" << G4endl;

	G4double e = 2.718281828459045;

	G4String particleName = aStep -> GetTrack() -> GetParticleDefinition() -> GetParticleName();
	G4double KE = aStep -> GetPreStepPoint() -> GetKineticEnergy();
	G4int baryonNumber = aStep -> GetTrack() -> GetParticleDefinition() -> GetBaryonNumber();

	if (particleName == "neutron") {
		if (KE < (1*MeV)) {
			return ( 2.5 + ( 18.2*pow(e,((-pow(std::log(KE),2))/6)) ) );
		} else if (KE < (50*MeV)) {
			return ( 5.0 + ( 17.0*pow(e,((-pow(std::log(2*KE),2))/6)) ) );
		} else {
			return ( 2.5 + ( 3.25*pow(e,((-pow(std::log(0.04*KE),2))/6)) ) );
		}
	} else if ((particleName == "proton") 
			|| (particleName == "pi+") 
			|| (particleName == "pi-")) {
		return 2.0;
	} else if (baryonNumber > 1) {
		return 20.0;
	} else {
		return 1.0;
	}
}
