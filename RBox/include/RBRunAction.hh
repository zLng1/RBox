#ifndef RBRunAction_h
#define RBRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

/// Run action class

class RBRunAction : public G4UserRunAction
{
  public:
    RBRunAction();
    virtual ~RBRunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void CountPhoton()           { fPhotons += 1; };

private:
    G4Accumulable<G4int>    fPhotons;
};

#endif

