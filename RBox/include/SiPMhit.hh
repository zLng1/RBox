#ifndef SiPMhit_h
#define SiPMhit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class SiPMHit : public G4VHit
{
public:
  SiPMHit();
  ~SiPMHit();
  // print on screen a Hit
  void Print();

public:
  inline void *operator  new(size_t);
  inline void operator delete(void *aHit);

public:
  void AddPhotonCount()  { photonCounts += 1;}
  void SetPosition(const G4ThreeVector & pos) {position = pos;}

private:
  G4int   photonCounts;
  //  G4double eDep;
  G4ThreeVector position;
};

/// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<SiPMHit> SiPMHitCollection;

//  -- new and delete overloaded operators:

extern G4Allocator<SiPMHit>   SiPMHitAllocator;

inline void* SiPMHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiPMHitAllocator.MallocSingle();
  return aHit;
}

inline void SiPMHit::operator delete(void *aHit)
{
  SiPMHitAllocator.FreeSingle((SiPMHit*) aHit);
}

#endif
