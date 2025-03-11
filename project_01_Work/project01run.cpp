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

int main(void){
  // Establishing namespace for variables
  std::map<int, std::map<int, std::string>> nodeTracker; // Use to index numerically all nodes and junctions
  EN_Project proj = nullptr; // Establish the EN_Project project pointer, as a EPANET Project

  // Defining File types and title
  std::string rootDir = "/users/zachfrost/Documents/USEPANET/project_01_Work/builds/";
  std::string outputFile = rootDir + "project01Out";
  std::string inputFile = rootDir + "project01Inp";
  std::string reportFile = rootDir + "project01_EPANET_Report";

  std::string prjT = "Project 01 Hydraulics EPANET Setup";
  std::string author = "[ Zachary A. Gioppo ; Vivienne Foreman ; Zhi Cheng Hai ]";
  std::string org = "MSU ENE422 - Spring 2025";

  int units  = EN_GPM; // Creates enum Units, indicating a base of Gallons/Minute
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  // runs EN_CreateProject & EN_init with given paramaters
  runSetup(proj, units, headMethod, reportFile, "", prjT, org, author);
  EN_setdemandmodel(proj, EN_DDA, 0, 0, 0);

  // Configure Report output
  EN_setreport(proj, "NODES ALL"); // Report on All Nodes
  EN_setreport(proj, "LINKS ALL"); // Report on All Links

  EN_setreport(proj, "DIAMETER PRECISION 2"); // Show Diameter(s) to precision of 0.00
  EN_setreport(proj, "LENGTH PRECISION 2"); // Show Length(s) to precision of 0.00
  EN_setreport(proj, "ELEVATION PRECISION 3"); // Show Elevation to precision of 0.000
  EN_setreport(proj, "PRESSURE PRECISION 3"); // Show Node Pressure to precision 0.000
  EN_setreport(proj, "VELOCITY PRECISION 3"); // Show Link (Pipe) Velocity to precision 0.000

  // Add All Junction Positions (Project, objectTag, mapPointer)
  for (int i = 0; i < 9; i++){
    std::string junctionName = std::format("jnc{}", "ABCDEFGHJ"[i]); // Grab the letter [i] (from 0 = A) and append it to the leader jnc, for a junction ID
    addJunction(proj, junctionName.c_str(), nodeTracker); // Create the junction
    // When naming junctions, for legibility index (I) is not included, instead skipping from H to J

  }

  // the Reservoirs needs to be established using (Project, objectTag, mapPointer) and configured with setnodevalue (Project, nodeIndex, property, value)
  addReservoir(proj, "rsvLow", nodeTracker); // establishing the LOWER resivour, always feeding network with pump, @ 500ft
  EN_setnodevalue(proj, 10, EN_ELEVATION, 500); // The low resivour will have nodeIndex 10, as it is a junction after all use points

  addReservoir(proj, "rsvFire", nodeTracker); // establishing the HIGHER resivour, which provides fire capacity @ 645ft
  EN_setnodevalue(proj, 11, EN_ELEVATION, 645); // the high resivour will have nodeIndex 11, as it was added after low resivour

  // Add a pump using (Project, objectTag, initJunction, finJunction, mapPointer)
  addPump(proj, "lowRsvPump", "rsvLow", "jncA", nodeTracker);
  int pumpIndex = EN_getlinkindex(proj, "lowRsvPump");

  // Add a Valve controller between RsvFire and the output node jncJ
  addPipe(proj, "pipeVal01", "rsvFire", "jncJ", nodeTracker);
  int valveIndex = EN_getlinkindex(proj, "pipeVal01");
  EN_setlinkvalue(proj, valveIndex, EN_MINORLOSS, 0.2); // Minor loss due to a Gate Valve is typically 0.2 when fully opened
  
  // Establishing Properties for Junction using EN_setnodevalue(Project, nodeIndex, property, value)
  // Adding Demands to Junctions using EN_adddemand(Project, nodeIndex, timePattern, demandID)
  EN_setnodevalue(proj, 1, EN_ELEVATION, 515); // jncA (attach to pump), at elevation of 515ft

  EN_setnodevalue(proj, 2, EN_ELEVATION, 515); // jncB, at elevation of 515ft
  EN_adddemand(proj, 2, 45, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 45 gal/min

  EN_setnodevalue(proj, 3, EN_ELEVATION, 525); // jncC, at elevation of 525ft
  EN_adddemand(proj, 3, 75, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 75 gal/min

  EN_setnodevalue(proj, 4, EN_ELEVATION, 530); // jncD, at elevation of 530ft
  EN_adddemand(proj, 4, 40, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 40 gal/min

  EN_setnodevalue(proj, 5, EN_ELEVATION, 530); // jncE, at elevation of 530ft
  EN_adddemand(proj, 5, 102, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 102 gal/min

  EN_setnodevalue(proj, 6, EN_ELEVATION, 535); // jncF, at elevation of 535ft
  EN_adddemand(proj, 6, 98, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 98 gal/min

  EN_setnodevalue(proj, 7, EN_ELEVATION, 540); // jncG, at elevation of 540ft
  EN_adddemand(proj, 7, 80, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 80 gal/min

  EN_setnodevalue(proj, 8, EN_ELEVATION, 550); // jncH, at elevation of 550ft
  EN_adddemand(proj, 8, 50, "", "baseDraw"); // Setting the fundamental demand (non-Fire instance), at 50 gal/min

  EN_setnodevalue(proj, 9, EN_ELEVATION, 550); // jncJ, at elevation of 550ft

  // Save Input File
  std::string saveName;
  const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  saveName = inputFile + "[Ran: " + std::to_string(time) + "].inp";
  EN_saveinpfile(proj, saveName.c_str());

  // Project Cleanup
  EN_deleteproject(proj);
  return 0;

}
