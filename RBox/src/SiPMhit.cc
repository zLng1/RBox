#include "SiPMhit.hh"
#include "G4SystemOfUnits.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<SiPMHit> SiPMHitAllocator;

SiPMHit::SiPMHit()
{
  photonCounts = 0; //start our photon count at 0 for every launched particle.
}

SiPMHit::~SiPMHit() = default; //set default destructor for SiPMHit

void SiPMHit::Print()
{
  G4cout<<"     Print:: Pos = "<< position <<G4endl;
  G4cout <<"    Print:: photon counts = " << photonCounts << G4endl; //should never be greater than 1.
}
