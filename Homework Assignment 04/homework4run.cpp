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
  std::string outputFile = "homework04Output";
  std::string inputFile = "homework04Input";
  std::string reportFile = "homework04_EPANET_Report";
  int units  = EN_GPM; // Creates enum Units, indicating a base of Gallons/Minute
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  // runs EN_CreateProject & EN_init with given paramaters
  runSetup(proj, units, headMethod, reportFile);


  // Add All Junction Positions
  for (int i = 0; i < 4; i++){
    std::string junctionName = std::format("jnc{}", "ABCD"[i]); // Grab the letter [i] (from 0 = A) and append it to the leader jnc, for a junction ID
    addJunction(proj, junctionName.c_str(), nodeTracker); // Create the junction
  }

  addReservoir(proj, "rsv1", nodeTracker);
  std::cout << "\nJunctions created! Identified as: \n" << nodeTracker[1][1] << " | " << nodeTracker [1][2] << " | " << nodeTracker[1][3] << " | " << nodeTracker[1][4] << "\n\n" << std::endl;
  std::cout << "Reservoir Created! Identified as: \n" << nodeTracker[1][5] << "\n\n" << std::endl;

  // Add Pipes between junctions
  addPipe(proj, "pipe1", "rsv1", "jncA", nodeTracker);
  addPipe(proj, "pipe2", "jncA", "jncB", nodeTracker);
  addPipe(proj, "pipe3", "jncA", "jncC", nodeTracker);
  addPipe(proj, "pipe4", "jncB", "jncD", nodeTracker);
  addPipe(proj, "pipe5", "jncB", "jncD", nodeTracker);
  addPipe(proj, "pipe6", "jncC", "jncD", nodeTracker);
  std::cout << "Junctions created! Identified as: \n" << nodeTracker[2][1] << " | " << nodeTracker [2][2] << " | " << nodeTracker[2][3] << "\n" << nodeTracker[2][4] << " | " << nodeTracker[2][5] << " | " << nodeTracker[2][6] << "\n\n" << std::endl;

  // Project Cleanup
  EN_deleteproject(proj);
  proj = nullptr;
  return 0;

}
