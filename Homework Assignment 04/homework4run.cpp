/////////////////////////////////////////
////       Zachary A. Gioppo        ////
//// Spring 2025 ENE422 Hydraulics ////
////        Project 01 Test       ////
/////////////////////////////////////

// namespace and inclusions established
#include <iostream>
#include <epanet2_2.h>
#include "../include/epaHandler.h"
using namespace std;

int main(void){
  // Establishing namespace for variables
  int index; // Use to index numerically all nodes and junctions
  EN_Project proj; // Establish the project pointer, as a EPANET Project
  std::string outputFile = "homework04Output";
  std::string inputFile = "homework04Input";
  std::string reportFile = "homework04_EPANET_Report";
  int units  = EN_GPM; // Creates enum Units, indicating a base of Gallons/Minute
  int headMethod = EN_DW; // Create an enum headMethod, which indicates the Darcy Wisebach equation

  runSetup(proj, units, headMethod, reportFile);

  // Final Project completion and cleanup
}
