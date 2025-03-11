/////////////////////////////////////////
////       Zachary A. Gioppo        ////
//// Spring 2025 ENE422 Hydraulics ////
////   Common Operations Parser   ////
/////////////////////////////////////

// namespace and inclusions established
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <epanet2_2.h>
#define ERRCODE(x) (errcode = ((errcode > 100) ? (errcode) : (x)))
using namespace std;


// initalizing project information and return errorcodes
// Mandatory pp,  project name - mandatory flow units - mandatory head calculation method formula - mandatory report file namespace
// Optional output file (binary) - optional inputFile (for running)
void runSetup(EN_Project& pp, int flowUnit, int headFormula, std::string reportFile, std::string outFile = "", std::string titleOne = "", std::string titleTwo = "", std::string titleThree = "") {
  // Appending Filetypes
  std::string fullOutFile;
  if (outFile != "") { std::string fullOutFile = outFile + ".out"; } // If a output file was named, make it to have .out
  std::string fullReportFile = reportFile + ".rpt"; // add .rpt file extension to ReportFile

  // Establish Error number variable and counter/msg
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  // Run these commands to setup EN_Project
  ERRCODE(EN_createproject(&pp));
  ERRCODE(EN_init(pp, fullReportFile.c_str(), fullOutFile.c_str(), flowUnit, headFormula));
  if (titleOne != "") { EN_settitle(pp, titleOne.c_str(), titleTwo.c_str(), titleThree.c_str()); }
  // Search the error message code for its explanation
  EN_geterror(errcode, errmsg, EN_MAXMSG);

  // Return Error Code with message and appropriate information
  if (errcode) { printf("Error Code [%d], with Message: \n%s\n", errcode, errmsg); }
}


// Establish Error Handler when generating Water Quality Report(s), parsing errors as they arise
// Requires definition of project, name and reportFIle
// Optionally define inputFile
void runQuality(EN_Project pp) {
  // Establish error code and message buffer
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  // Run Water Quality analysis
  ERRCODE(EN_solveQ(pp));
  ERRCODE(EN_report(pp));

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}

// Establish Error Handler to generate Hydraulics Report, while parsing errors along the way
// Will REQUIRE definition of project name and a reportFIle name.
// Optionally define a inputFIle, if runSetup() isn't used; and
void runHydraulics(EN_Project pp, std::string reportFile, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1) {
  // Validate saveInput and runType
  if (saveInput != 0 && saveInput != 1) { printf("saveInput must be '0' or '1' (inputted: %d)\n", saveInput); return; }
  if (runType != "input" && runType != "noinput") { printf("runType must be 'input' or 'noinput' (inputted: %s)\n", runType.c_str()); return; }

  if (saveInput == 1 && inputFile.empty()) { printf("In order to save Input, please specify an input file name.\n"); return; }
  else if (saveInput == 0) { printf("Continuing without saving InputFile.\n"); }

  // Establish error code and message buffer
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  // Open file if running from input
  if (runType == "input") {
      std::string fullInputFile = inputFile + ".inp";
      std::string fullReportFile = reportFile + ".rpt";
      errcode = EN_open(pp, fullInputFile.c_str(), fullReportFile.c_str(), "");
  }
  else { printf("Assuming EN_init function was run to initialize Project.\n"); }

  // Run hydraulic analysis
  ERRCODE(EN_solveH(pp));
  ERRCODE(EN_saveH(pp));
  ERRCODE(EN_report(pp));

  // Save input file if needed
  if (saveInput == 1) {
      std::string saveName;
      if (runType == "noinput") {
          saveName = inputFile + ".inp";
      }
      else {
          const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
          saveName = inputFile + "[Ran: " + std::to_string(time) + "].inp";
      }
      errcode = EN_saveinpfile(pp, saveName.c_str());
  }

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}


// Establishing Error Handler and indexer for various Network components. More to be added as needed
// Junction Tank and Reservoir are EN_addnode() calls, Pipe and Pump are EN_addlink() calls
// inputs are all required, and represent the project - a tag for the node, and a storage map to cirrelate EPANET's ID and its Tag
// Only addJunction is annotated, as these functions are variations on the same.
void addJunction(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage) {
  // Establish errorcode and a blank index value (temporary)
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  // Add the node, with type EN_Junction, and save its output integer to the index pointer
  ERRCODE(EN_addnode(pp, nodeTag, EN_JUNCTION, &index));

  // Append to the indexStorage map a key:value pair for this Junction
  indexStorage[1][index] = std::format("nID[{}] : tag[{}] : type[Junction]", index, nodeTag); // use std::format (C++20) to create a value with the NodeType and its tag denoted. Store this with its key
  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}

void addTank(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addnode(pp, nodeTag, EN_TANK, &index));

  indexStorage[1][index] = std::format("nID[{}] : tag[{}] : type[Tank]", index, nodeTag);

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}

void addReservoir(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addnode(pp, nodeTag, EN_RESERVOIR, &index));

  indexStorage[1][index] = std::format("nID[{}] : tag[{}] : type[Reservoir]", index, nodeTag);

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}


void addPipe(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::map<int, std::string>>& indexStorage) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addlink(pp, nodeTag, EN_PIPE, junctionOne, junctionTwo, &index));

  indexStorage[2][index] = std::format("nID[{}] : tag[{}] : type[Pipe]", index, nodeTag);

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}

void addPump(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::map<int, std::string>>& indexStorage) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addlink(pp, nodeTag, EN_PUMP, junctionOne, junctionTwo, &index));

  indexStorage[2][index] = std::format("nID[{}] : tag[{}] : type[Pump]", index, nodeTag);

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code [%d], Message: \n%s\n", errcode, errmsg);
  }
}
