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
  std::map<int, std::string> nodeTracker; // Use to index numerically all nodes and junctions
  EN_Project proj = nullptr; // Establish the EN_Project project pointer, as a EPANET Project
  std::string outputFile = "homework04Output";
  std::string inputFile = "homework04Input";
  std::string reportFile = "homework04_EPANET_Report";
  int units  = EN_GPM; // Creates enum Units, indicating a base of Gallons/Minute
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  // runs EN_CreateProject & EN_init with given paramaters
  runSetup(proj, units, headMethod, reportFile);

  addJunction(proj, "J1", nodeTracker);
  std::cout << nodeTracker[1] << std::endl;

  EN_deleteproject(proj);
  return 0;

}
