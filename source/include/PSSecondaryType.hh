#ifndef PSSecondaryType_h
#define PSSecondaryType_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>

class PSSecondaryType : public G4VPrimitiveScorer
{
 public: // with description
      PSSecondaryType(G4String name, G4int depth=0);
      PSSecondaryType(G4String name, const G4String& unit, G4int depth=0);
      virtual ~PSSecondaryType();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();

  private:
      G4int HCID;
      G4THitsMap<G4String>* EvtMap;
};
#endif
