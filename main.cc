// Основные Manager
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
// Вспмогательные
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QBBC.hh"
#include "G4UserStackingAction.hh"
// Пользовательские include
#include "include/linacG4DetectorConstruction.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;

    G4RunManager* runManager = new G4RunManager;

    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new QBBC);

    runManager->Initialize();

    ui = new G4UIExecutive(argc, argv);

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialise();

    G4UImanager *UI = G4UImanager::GetUIpointer();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // UI->ApplyCommand("/vis/open OGL");
    UI->ApplyCommand("/vis/drawVolume");
    // UI->ApplyCommand("/vis/scene/add/axes");
    UI->ApplyCommand("/vis/viewer/set/auxiliaryEdge true");
    // UI->ApplyCommand("/vis/viewer/set/lineSegmentsPerCircle 100");
    UI->ApplyCommand("/vis/open VRML2FILE");


    ui->SessionStart();

    delete runManager;
    delete visManager;
    return 0;
}
