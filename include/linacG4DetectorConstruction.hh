#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
private:
    /* data */
    G4LogicalVolume* fScoringVolume = nullptr;
public:
    DetectorConstruction(/* args */);
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;}
};

#endif
