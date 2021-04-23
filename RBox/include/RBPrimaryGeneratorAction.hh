#ifndef RBPrimaryGeneratorAction_h
#define RBPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4UIcmdWith3VectorAndUnit;
class RBPrimaryGeneratorMessenger;

class RBPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    RBPrimaryGeneratorAction();    
  virtual ~RBPrimaryGeneratorAction();
  
  virtual void GeneratePrimaries(G4Event*);         
  
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  inline void SetGunPosition(G4ThreeVector aVec){gunPosition = aVec;}
  inline void SetGunParticleType(G4int  nType){particleType = nType;}

private:
  G4ParticleGun*  fParticleGun;
  RBPrimaryGeneratorMessenger* generatorMessenger;
  G4ThreeVector  gunPosition;
  G4int particleType;   // 0: optical photon, 1: muons 
};

#endif
