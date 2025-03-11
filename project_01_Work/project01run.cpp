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
  std::string rootDir = "/users/zachfrost/Documents/USEPANET/project_01_Work/builds/epanet_Out/";
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

  // Configure Report output
  EN_setreport(proj, "NODES ALL"); // Report on All Nodes
  EN_setreport(proj, "LINKS ALL"); // Report on All Links

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
  addPump(proj, "lowRsvPump", 1, 10, nodeTracker);

  // Establishing Properties for Junctions EN_setnodevalue(Project, nodeIndex, property, value) OR EN_setjuncdata(Project, nodeIndex, elev, demand, demandPattern(time))
  EN_setnodevalue(proj, 1, EN_ELEVATION, 515); // jncA (attach to pump), at elevation of 515ft
  EN_setjuncdata(proj, 2, 515, 35, ""); // jncB, at elevation of 515ft, and has a demand of 35gpm
  EN_setjuncdata(proj, 3, 525, 75, ""); // jncC, at elevation of 525ft, and has a demand of 75gpm
  EN_setjuncdata(proj, 4, 530, 40, ""); // jncD, at elevation of 530ft, and has a demand of 40gpm
  EN_setjuncdata(proj, 5, 530, 102, ""); // jncE, at elevation of 530ft, and has a demand of 102gpm
  EN_setjuncdata(proj, 6, 535, 98, ""); // jncF, at elevation of 535ft, and has a demand of 98gpm
  EN_setjuncdata(proj, 7, 540, 80, ""); // jncG, at elevation of 540ft, and has a demand of 80gpm
  EN_setjuncdata(proj, 8, 550, 50, ""); // jncH, at elevation of 550ft, and has a demand of 50gpm
  EN_setnodevalue(proj, 9, EN_ELEVATION, 550); // jncJ, at elevation of 550ft

  // Project Cleanup
  EN_deleteproject(proj);
  return 0;

}
