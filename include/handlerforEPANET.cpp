/////////////////////////////////////////
////       Zachary A. Gioppo        ////
//// Spring 2025 ENE422 Hydraulics ////
////   Common Operations Parser   ////
/////////////////////////////////////

// namespace and inclusions established
#include <iostream>
#include <epanet2_2.h>
#define ERRCODE(x) (errcode = ((errcode > 100) ? (errcode) : (x)))
using namespace std;


// initalizing project information and return errorcodes
// Mandatory pp,  project name - mandatory flow units - mandatory head calculation method formula - mandatory report file namespace
// Optional output file (binary) - optional inputFile (for running)
void runSetup(EN_Project pp, int flowUnit, int headFormula, std::string reportFile, std::string outFile = "", std::string inputFile = "") {
  // Appending Filetypes
  std::string fullOutFile;
  if (outFile != "") { std::string fullOutFile = outFile + ".out"; }
  else { std::string fullOutFile = outFile; }

  std::string fullReportFile = reportFile + ".rpt";

  // Establish Error number variable and counter/msg
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  // Run these commands to setup EN_Project
  ERRCODE(EN_createproject(&pp));
  ERRCODE(EN_init(pp, fullReportFile.c_str(), fullOutFile.c_str(), flowUnit, headFormula));

  // Search the error message code for its explanation
  EN_geterror(errcode, errmsg, EN_MAXMSG);

  // Return Error Code with message and appropriate information
  if (errcode) { printf("Error Code [%d], with Message: \n%s\n", errcode, errmsg); }
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
          auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
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
