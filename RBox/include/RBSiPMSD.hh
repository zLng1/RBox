#ifndef RBSiPMSD_h
#define RBSiPMSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "SiPMhit.hh"

class G4HCofThisEvent;      // "H(it) C(ollection) of This Event

/// Run action class

class RBSiPMSD : public G4VSensitiveDetector
{
public:
  RBSiPMSD(G4String SDname);
  virtual ~RBSiPMSD();

  /// Mandatory base class method: it must to be overloaded
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

  void Initialize(G4HCofThisEvent* HCE);
  void EndOfEvent(G4HCofThisEvent* HCE);
  
private:
  SiPMHitCollection*       photonHitCollection;  
};

#endif

