/////////////////////////////////////////
////       Zachary A. Gioppo        ////
//// Spring 2025 ENE422 Hydraulics ////
////        Project 01 Test       ////
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
  std::string outputFile = "homework04Output";
  std::string inputFile = "homework04Input";
  std::string reportFile = "homework04_EPANET_Report";

  std::string prjT = "Homework 04 EPANET";
  std::string author = "Zachary A. Gioppo";
  std::string org = "MSU ENE422 - Spring 2025";

  int units  = EN_LPS; // Creates enum Units, indicating a base of Litres/Second
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  // runs EN_CreateProject & EN_init with given paramaters
  runSetup(proj, units, headMethod, reportFile, "", prjT, org, author);

  // Configure Report output
  EN_setreport(proj, "NODES ALL");
  EN_setreport(proj, "LINKS ALL");

  EN_setreport(proj, "ELEVATION PRECISION 3");

  // Add All Junction Positions (Project, objectTag, mapPointer)
  for (int i = 0; i < 4; i++){
    std::string junctionName = std::format("jnc{}", "ABCD"[i]); // Grab the letter [i] (from 0 = A) and append it to the leader jnc, for a junction ID
    addJunction(proj, junctionName.c_str(), nodeTracker); // Create the junction
  }
  addReservoir(proj, "rsv1", nodeTracker);
  // Establishing Properties for Junctions (Project, nodeIndex, elev, demand, demandPattern(time))
  EN_setjuncdata(proj, 1, 10, 0, ""); // jncA - outflow 0L/s at 10m
  EN_setjuncdata(proj, 2, 20, 50, ""); // jncB - outflow 50L/s at 20m
  EN_setjuncdata(proj, 3, 5, 200, ""); // jncC - outflow 200L/s at 5m
  EN_setjuncdata(proj, 4, 0, 200, ""); // jncD - outflow 200L/s at 0m
  // the Reservoir needs to be configured with setnodevalue (Project, nodeIndex, property, value)
  EN_setnodevalue(proj, 5, EN_ELEVATION, 100); // rsv1 - outflow unknown at 100m

  // Debug to show junction nodes were set
  std::cout << "\nJunctions created! Identified as: \n" << nodeTracker[1][1] << " | " << nodeTracker [1][2] << " | " << nodeTracker[1][3] << " | " << nodeTracker[1][4] << "\n\n" << std::endl;
  std::cout << "Reservoir Created! Identified as: \n" << nodeTracker[1][5] << "\n\n" << std::endl;

  // Add Pipes between junctions (Project, objectTag, fromLoc, toLoc, mapPointer)
  addPipe(proj, "pipe1", "rsv1", "jncA", nodeTracker);
  addPipe(proj, "pipe2", "jncA", "jncB", nodeTracker);
  addPipe(proj, "pipe3", "jncA", "jncC", nodeTracker);
  addPipe(proj, "pipe4", "jncB", "jncD", nodeTracker);
  addPipe(proj, "pipe5", "jncB", "jncD", nodeTracker);
  addPipe(proj, "pipe6", "jncC", "jncD", nodeTracker);

  // Configure pipe's properties (Project, objectTag, length, diameter, roughness, minLoss)
  EN_setpipedata(proj, 1, 300, 0.5, 0.2, 0); // Pipe 1, length 0300m -- Diameter 0.5m, roughness 0.2mm
  EN_setpipedata(proj, 2, 600, 0.5, 0.2, 0); // Pipe 2, length 0600m -- Diameter 0.5m, roughness 0.2mm
  EN_setpipedata(proj, 3, 1200, 0.4, 0.2, 0); // Pipe 3, length 1200m -- Diameter 0.4m, roughness 0.2mm
  EN_setpipedata(proj, 4, 1000, 0.4, 0.2, 0); // Pipe 4, length 1000m -- Diameter 0.4m, roughness 0.2mm
  EN_setpipedata(proj, 5, 1200, 0.4, 0.2, 0); // Pipe 5, length 1200m -- Diameter 0.4m, roughness 0.2mm
  EN_setpipedata(proj, 6, 600, 0.5, 0.2, 0); // Pipe 6, length 0600m -- Diameter 0.4m, roughness 0.2mm

  // Debug to show links are set
  std::cout << "Junctions created! Identified as: \n" << nodeTracker[2][1] << " | " << nodeTracker [2][2] << " | " << nodeTracker[2][3] << "\n" << nodeTracker[2][4] << " | " << nodeTracker[2][5] << " | " << nodeTracker[2][6] << "\n\n" << std::endl;


  // Run the model
  runHydraulics(proj, reportFile, inputFile); // Run the hydraulic model, saving to reportFile and inputFIle. By default it will run without a input (reading the project layout described above), and save an inputfile for future use

  // Project Cleanup
  EN_deleteproject(proj);
  return 0;

}
