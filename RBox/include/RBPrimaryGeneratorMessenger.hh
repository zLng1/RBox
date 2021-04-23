#ifndef RBPrimaryGeneratorMessenger_h
#define RBPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class RBPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;

class RBPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  RBPrimaryGeneratorMessenger(RBPrimaryGeneratorAction*);
  ~RBPrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  RBPrimaryGeneratorAction*      RBAction;
  G4UIdirectory*                           gunDir; 
  G4UIcmdWithAnInteger*           SetGunParticleType;
  G4UIcmdWith3VectorAndUnit*  SetGunPositionCmd;
  
};

#endif
