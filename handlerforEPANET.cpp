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
void runSetup(EN_Project pp, int *flowUnit, int *headFormula, char *reportFile, char *outFile = "", char *inputFile = "") {
  // Establish Error number varioable and counter/msg
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  // Run these commands to setup EN_Project
  ERRCODE(EN_createproject(pp));
  ERRCODE();

}

// Establish Error Handler to generate Hydraulics Report, while parsing errors along the way
void runHydraulics(EN_Project pp, char *reportFile, char *inputFile = "", char runType = "noinput", int saveInput = 1) {
  if (saveInput == 1 && inputFile != "") { continue; }
    else if (saveInput == 1 && inputFile == "") { printf("In order to save Input, please specify an input file name \n"); return 1; }
    else if (saveInput == 0) { printf("Continuing without saving InputFile"); continue; }
    else return ("saveInput must be '0' or '1' (inputted: %d) | runType must be 'input' or 'noinput' (inputted: %s)", saveInput, runType);

  // Establish code number variable and counter/msg
  int errcode = 0;  
  char errmsg[EN_MAXMSG + 1];

  // If running an InputFile; otherwise assume project was initalized
  if (runType == "input") { ERRCODE(EN_open(pp, inputFile.append(".inp"), reportFile.append(".rpt"), "")); continue; }
  else { printf("Assuming EN_init function was ran to initalize Project. \n"); continue; }
  // Hydraulic Analysis
  ERRCODE(EN_solveH(pp));
  ERRCODE(EN_saveH(pp));
  ERRCODE(EN_report(pp));

    // If a saveName is specified for input file, run savefile
  if (saveInput == 1 && runType == "noinput") { ERRCODE(EN_saveinpfile(pp, inpSaveName.append(".inp"))); }
  elif (saveInput == 1 && runType == "input") { auto time = std::chrono::system_clock::now(); ERRCODE(EN_saveinpfile(pp, inpSaveName.append("[Ran: %s]", time))); }
  else continue;
  // Searcj the error message code for its explanation
  EN_geterror(errcode, errmsg, EN_MAXMSG);

  // Return Error Code with message and appropriate information
  if (errode) { printf("Error Code [%d], with Message: \n%s\n", errcode, errmsg); }
}
