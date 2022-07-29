// This is just the G4DoseDeposit.hh file copied directly, but the .cc file has a few modifications
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

#ifndef G4PSDoseDepositMod_h
#define G4PSDoseDepositMod_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>

// (Description)
//   Primitive scorer class for scoring dose deposit in the geometry volume.
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 2010-07-22   Introduce Unit specification.
// 

class G4PSDoseDepositMod : public G4VPrimitiveScorer
{
 public: // with description
      G4PSDoseDepositMod(G4String name, G4int depth=0);
      G4PSDoseDepositMod(G4String name, const G4String& unit, G4int depth=0);
      virtual ~G4PSDoseDepositMod();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

      virtual G4double ComputeVolume(G4Step*, G4int idx);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      virtual void SetUnit(const G4String& unit);

      std::vector<std::pair<G4String, G4double>> GetParticlesEnergies();

  private:
      G4int HCID;
      G4THitsMap<G4double>* EvtMap;
      std::vector<std::pair<G4String, G4double>> particlesEnergies;
      G4double IDToMass(G4int);
};
#endif
