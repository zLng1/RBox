#ifndef RBDetectorConstruction_h
#define RBDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class RBDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  RBDetectorConstruction();
  virtual ~RBDetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  
private:
  void DefineMaterials();
  
  G4double BoxXYZ;

  G4Material *default_mat, *Box_mat;

  G4LogicalVolume* sipmLV;
  
  G4bool  fCheckOverlaps;
};

#endif
