#ifndef PSSecondaries_h
#define PSSecondaries_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include "G4Step.hh"
#include <vector>
#include <set>

class PSSecondaries : public G4VPrimitiveScorer
{
 public: // with description
      PSSecondaries(G4String name, G4int depth=0);
      PSSecondaries(G4String name, const G4String& unit, G4int depth=0);
      virtual ~PSSecondaries();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();

  private:
      G4int HCID;
      G4THitsMap<G4String>* EvtMap;
      std::set<G4int> trackIDsFound;
};
#endif
