/////////////////////////////////////////
////       Zachary A. Gioppo        ////
//// Spring 2025 ENE422 Hydraulics ////
////        Project 01 Setup      ////
/////////////////////////////////////

// namespace and inclusions established
#include <iostream>
#include <map>
#include <epanet2_2.h>
#include "../include/epaHandler.h"
using namespace std;

void projSetupConfig(EN_Project& pp, int runTime, int runStep, float repRatio = 1) {
  if (repRatio < 1) {
    printf("Reporting Ratio must be 1 or greater. \n This Indicates how many Time steps occur between report logging. \n Your entry {%f}", repRatio);
    return;
  }
  if (runStep > runTime) { printf("Indicated runStep {%i} CANNOT exceed the runtime {%i} ", runStep, runTime); return; }
  else if (runStep == runTime) { printf("WARNING: \n Only ONE timeStep will be simulated."); }

  // Unit Conversion(s)
  long runTimeSecs = runTime * 60 * 60;
  long runStepSecs = runStep * 60 * 60;
  long repStepSecs = runStepSecs * repRatio;

  // Set the demand modeling method
  EN_setdemandmodel(pp, EN_DDA, 0, 0, 0);

  // Setup Water Quality Monitoring
  EN_setqualtype(pp, EN_AGE, "", "", "");

  // Setup time parameters
  EN_settimeparam(pp, EN_DURATION, runTimeSecs);
  EN_settimeparam(pp, EN_HYDSTEP, runStepSecs);
  EN_settimeparam(pp, EN_QUALSTEP, runStepSecs/2);
  EN_settimeparam(pp, EN_REPORTSTEP, repStepSecs);

  // Configure Report output
  EN_setreport(pp, "NODES ALL"); // Report on All Nodes
  EN_setreport(pp, "LINKS ALL"); // Report on All Links
  EN_setreport(pp, "ENERGY YES");

  EN_setreport(pp, "DIAMETER PRECISION 2"); // Show Diameter(s) to precision of 0.00
  EN_setreport(pp, "LENGTH PRECISION 2"); // Show Length(s) to precision of 0.00
  EN_setreport(pp, "ELEVATION PRECISION 3"); // Show Elevation to precision of 0.000
  EN_setreport(pp, "PRESSURE PRECISION 3"); // Show Node Pressure to precision 0.000
  EN_setreport(pp, "VELOCITY PRECISION 3"); // Show Link (Pipe) Velocity to precision 0.000
}

void projNodeSetup(EN_Project& pp, std::map<int, std::map<int, std::string>>& indexStorage){
  // Add All Junction Positions (Project, objectTag, mapPointer)
  for (int i = 0; i < 10; i++){
    std::string junctionName = std::format("jnc{}", "ABCDEFGHJK"[i]); // Grab the letter [i] (from 0 = A) and append it to the leader jnc, for a junction ID
    addJunction(pp, junctionName.c_str(), indexStorage); // Create the junction
    // When naming junctions, for legibility index (I) is not included, instead skipping from H to J

  }

  // the Reservoirs needs to be established using (Project, objectTag, mapPointer) and configured with setnodevalue (Project, nodeIndex, property, value)
  addReservoir(pp, "rsvLow", indexStorage); // establishing the LOWER resivour, always feeding network with pump, @ 500ft
  EN_setnodevalue(pp, 11, EN_ELEVATION, 500); // The low resivour will have nodeIndex 10, as it is a junction after all use points

  addReservoir(pp, "rsvFire", indexStorage); // establishing the HIGHER resivour, which provides fire capacity @ 645ft
  EN_setnodevalue(pp, 12, EN_ELEVATION, 645); // the high resivour will have nodeIndex 11, as it was added after low resivour

  // Establishing Properties for Junction using EN_setnodevalue(Project, nodeIndex, property, value)
  // Adding Demands to Junctions using EN_adddemand(Project, nodeIndex, timePattern, demandID)

  EN_setnodevalue(pp, 10, EN_ELEVATION, 500); // jncK (attach to pump), at elevation of 500ft
  EN_setnodevalue(pp, 1, EN_ELEVATION, 515); // JncA (First Network node), at elevation 515ft

  EN_setnodevalue(pp, 2, EN_ELEVATION, 515); // jncB, at elevation of 515ft
  EN_adddemand(pp, 2, 35, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 35 gal/min

  EN_setnodevalue(pp, 3, EN_ELEVATION, 525); // jncC, at elevation of 525ft
  EN_adddemand(pp, 3, 75, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 75 gal/min

  EN_setnodevalue(pp, 4, EN_ELEVATION, 530); // jncD, at elevation of 530ft
  EN_adddemand(pp, 4, 40, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 40 gal/min

  EN_setnodevalue(pp, 5, EN_ELEVATION, 530); // jncE, at elevation of 530ft
  EN_adddemand(pp, 5, 102, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 102 gal/min

  EN_setnodevalue(pp, 6, EN_ELEVATION, 535); // jncF, at elevation of 535ft
  EN_adddemand(pp, 6, 98, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 98 gal/min

  EN_setnodevalue(pp, 7, EN_ELEVATION, 540); // jncG, at elevation of 540ft
  EN_adddemand(pp, 7, 80, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 80 gal/min

  EN_setnodevalue(pp, 8, EN_ELEVATION, 550); // jncH, at elevation of 550ft
  EN_adddemand(pp, 8, 50, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 50 gal/min

  EN_setnodevalue(pp, 9, EN_ELEVATION, 550); // jncJ, at elevation of 550ft
}

int main(void){
  // Establishing namespace for variables
  std::map<int, std::map<int, std::string>> nodeTracker; // Use to index numerically all nodes and junctions
  EN_Project proj = nullptr; // Establish the EN_Project project pointer, as a EPANET Project

  // Defining File types and title
  std::string rootDir = "/users/zachfrost/Documents/USEPANET/project_01_Work/builds/";
  std::string outputFile = rootDir + "outProj01";
  std::string inputFile = rootDir + "inpProj01";
  std::string reportFile = rootDir + "project01_EPANET_Report";

  std::string prjT = "Project 01 Hydraulics EPANET Setup";
  std::string author = "  Zachary A. Gioppo \n  Vivienne Foreman \n  Zhi Cheng Hai";
  std::string org = "MSU ENE422 - Spring 2025";

  int units  = EN_GPM; // Creates enum Units, indicating a base of Gallons/Minute
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  double pipeTraits [3] = {24, 0.0085, 0.3}; // Traits: Pipe Diameter (24in), Pipe Roughness (0.0085 for Iron), Minor Loss (0.3 for TWO check valves, fully open);
  int pipeLenTable [12] = {550, 250, 1000, 750, 750, 750, 875, 1050, 750, 750, 1000, 838}; // Pipe Lengths
  double curveXVals [] = {0, 200, 400, 600, 800, 1000};
  double curveYVals [] = {380, 360, 310, 240, 150, 50};



  // runs EN_CreateProject & EN_init with given paramaters
  runSetup(proj, units, headMethod, reportFile, "", prjT, org, author);
  projSetupConfig(proj, 72, 8, 3); // Setup configuration of report file for project
    // Run simulation for 72hrs, with timestep 8hr and reporting every 2nd timeStep


  // Adding all Nodes using the projNodeSetup() call
  projNodeSetup(proj, nodeTracker);
  // Add a pump using (Project, objectTag, initJunction, finJunction, mapPointer)
  int rsvPumpID;
  addPump(proj, "lowRsvPump", "rsvLow", "jncK", nodeTracker);
  EN_getlinkindex(proj, "lowRsvPump",&rsvPumpID);


  // Add a Valve controller between RsvFire and the output node jncJ
  int fireValID;
  addPipe(proj, "pipeVal01", "rsvFire", "jncJ", nodeTracker);
  EN_getlinkindex(proj, "pipeVal01", &fireValID);
  EN_setlinkvalue(proj, fireValID, EN_MINORLOSS, 0.2); // Minor loss due to a Gate Valve is typically 0.2 when fully opened
  EN_setlinkvalue(proj, fireValID, EN_INITSTATUS, EN_CLOSED); // Set the "Status" of the fire pipe to CLOSED by default
  EN_setlinkvalue(proj, fireValID, EN_LENGTH, pipeLenTable[1]); // Select Pipe Length
  EN_setlinkvalue(proj, fireValID, EN_DIAMETER, pipeTraits[0]); // Select Diameter
  EN_setlinkvalue(proj, fireValID, EN_ROUGHNESS, pipeTraits[1]); // Select Roughness


  // Network Design itself
  addPipe(proj, "pipeAB", "jncA", "jncB", nodeTracker); // PipeAB - Connect Node A to B over length 1000ft
  addPipe(proj, "pipeAE", "jncA", "jncE", nodeTracker); // PipeAE - Connect Node A to E over length 0750ft
  addPipe(proj, "pipeBD", "jncB", "jncD", nodeTracker); // PipeBD - Connect Node B to D over length 0750ft
  addPipe(proj, "pipeDG", "jncD", "jncG", nodeTracker); // PipeDG - Connect Node D to G over length 0750ft
  addPipe(proj, "pipeGE", "jncG", "jncE", nodeTracker); // PipeGE - Connect Node G to E over length 0875ft
  addPipe(proj, "pipeEC", "jncE", "jncC", nodeTracker); // PipeEC - Connect Node E to C over length 1050ft
  addPipe(proj, "pipeCF", "jncC", "jncF", nodeTracker); // PipeCF - Connect Node C to F over length 0750ft
  addPipe(proj, "pipeHF", "jncH", "jncF", nodeTracker); // PipeFH - Connect Node H to F over length 0750ft
  addPipe(proj, "pipeJH", "jncJ", "jncH", nodeTracker); // PipeJH - Connect Node H to J over length 1000ft
  addPipe(proj, "pipeEJ", "jncE", "jncJ", nodeTracker); // PipeJE - Connect Node E to J over length 0838ft
  addPipe(proj, "pipeKA", "jncK", "jncA", nodeTracker); // PipeKA - Connect Node K to A over length 550ft
  // Store information for 'special' Pipes
  int pipeFirstID; int pipeLastID; int pipePumpID;
  EN_getlinkindex(proj, "pipeAB", &pipeFirstID); EN_getlinkindex(proj, "pipeJE", &pipeLastID); EN_getlinkindex(proj, "pipeKA", &pipePumpID); // Save the link index of first and last pipe pipe (not the Fire Valve or Reservoir Pump)
  // Configure General Pipenetwork options
  for (int i = pipeFirstID; i < pipeLastID + 1; i++){
    // For Each Pipe (starting from the index of first mapped Pipe), set Length, Diameter Roughness, Minorloss, and status (Open/Closed)
    // "i" is the linkIndex, and will iterate between the first and last pipe in the network (after the pump and "valve")
    // Traits and Lengths are fetched from appropriate lookup tables
    EN_setlinkvalue(proj, i, EN_LENGTH, pipeLenTable[i - 1]);
    EN_setlinkvalue(proj, i, EN_DIAMETER, pipeTraits[0]);
    EN_setlinkvalue(proj, i, EN_ROUGHNESS, pipeTraits[1]);
    EN_setlinkvalue(proj, i, EN_MINORLOSS, pipeTraits[2]);
    EN_setlinkvalue(proj, i, EN_STATUS, EN_OPEN);
  }
  EN_setpipedata(proj, pipePumpID, pipeLenTable[0], pipeTraits[0], pipeTraits[1], pipeTraits[2]);
  EN_setlinkid(proj, pipePumpID, "pipePump2Net");


  // Setup the Pump Curve
  addCurve(proj, "pumpCurve01", 6, curveXVals, curveYVals, nodeTracker, 2);
  int pumpCurveID; EN_getcurveindex(proj, "pumpCurve01", &pumpCurveID);
  EN_setheadcurveindex(proj, rsvPumpID, pumpCurveID);


  // Run the hydraulics for the system
  // runHydraulics(proj, reportFile, inputFile);
  // runQuality(proj, reportFile,  2, inputFile);
  runStepwise(proj);
  string saveName = inputFile + ".inp";
  EN_saveinpfile(proj, saveName.c_str());

  // Project Cleanup
  EN_deleteproject(proj);
  return 0;
}
