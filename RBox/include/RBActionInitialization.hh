#ifndef RBActionInitialization_h
#define RBActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class RBActionInitialization : public G4VUserActionInitialization
{
  public:
    RBActionInitialization();
    virtual ~RBActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};

#endif

    
