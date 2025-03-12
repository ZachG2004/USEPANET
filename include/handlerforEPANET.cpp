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
  if (errcode) { printf("Error Code when running Setup [%d], with Message: \n%s\n", errcode, errmsg); }
}


// Establish Error Handler when generating Water Quality Report(s), parsing errors as they arise
// Requires definition of project, name and reportFIle
// Optionally define inputFile
void runQuality(EN_Project pp, std::string reportFile, int runMode = 1, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1) {
  // Establish error code and message buffer
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];

  long t, tStep; // Timestep storage
  // Validate saveInput and runType
  if (saveInput != 0 && saveInput != 1) { printf("saveInput must be '0' or '1' (inputted: %d)\n", saveInput); return; }
  if (runType != "input" && runType != "noinput") { printf("runType must be 'input' or 'noinput' (inputted: %s)\n", runType.c_str()); return; }

  if (saveInput == 1 && inputFile.empty()) { printf("In order to save Input, please specify an input file name.\n"); return; }
  else if (saveInput == 0) { printf("Continuing without saving InputFile.\n"); }

  // Open file if running from input
  if (runType == "input") {
      std::string fullInputFile = inputFile + ".inp";
      std::string fullReportFile = reportFile + ".rpt";
      ERRCODE(EN_open(pp, fullInputFile.c_str(), fullReportFile.c_str(), ""));
  }
  else { printf("Continuing to run Water Quality. \n"); }

  // Check runMode
  if (runMode == 1) {
    // Run Water Quality analysis continuously
    ERRCODE(EN_solveH(pp));
    ERRCODE(EN_saveH(pp));

    ERRCODE(EN_solveQ(pp));
    ERRCODE(EN_report(pp));

  } else if (runMode == 2) {
    // Run Water Quality Analysis Sequentially

    ERRCODE(EN_openQ(pp));
    ERRCODE(EN_initQ(pp, EN_SAVE));
    do {
      ERRCODE(EN_runQ(pp, &t));
      ERRCODE(EN_nextQ(pp, &tStep));

    } while (tStep > 0);
    EN_clearreport(pp);
    ERRCODE(EN_report(pp));
    ERRCODE(EN_closeQ(pp));

  }  else {
    printf("Indicated mode {%i} is NOT a valid option. Enter either: \n 1 : Continuous \n 2 : Step-wise \n\n", runMode);
    return;
  }  // Otherwise do not run


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
        ERRCODE(EN_saveinpfile(pp, saveName.c_str()));
    }

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code when running Quality [%d], Message: \n%s\n", errcode, errmsg);
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
      ERRCODE(EN_open(pp, fullInputFile.c_str(), fullReportFile.c_str(), ""));
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
      ERRCODE(EN_saveinpfile(pp, saveName.c_str()));
  }

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code when Running Hydraulics [%d], Message: \n%s\n", errcode, errmsg);
  }
}


void runStepwise(EN_Project pp) {
  // Establish error code and message buffer
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  long tStep, t;

  printf("Initiating a joint analysis, running hydraulics and Quality simultaneously. \n\n");

  // DEBUG
  // long qualStUsed, hydStUsed, repStUsed;
  // EN_gettimeparam(pp, EN_HYDSTEP, &hydStUsed);
  // EN_gettimeparam(pp, EN_QUALSTEP, &qualStUsed);
  // EN_gettimeparam(pp, EN_REPORTSTEP, &repStUsed);
  // printf("Using timeStep: [Quality: %ld , Hydraulics: %ld , Report: %ld] \n\n", qualStUsed/(60*60), hydStUsed/(60*60), repStUsed/(60*60));


  ERRCODE(EN_openH(pp));
  ERRCODE(EN_initH(pp, EN_SAVE));

  ERRCODE(EN_openQ(pp));
  ERRCODE(EN_initQ(pp, EN_SAVE));

  do {
    ERRCODE(EN_runH(pp, &t));
    ERRCODE(EN_runQ(pp, &t));

    ERRCODE(EN_nextH(pp, &tStep));
    ERRCODE(EN_nextQ(pp, &tStep));
    // printf("Current time : [%ld] \n Current Remaining: [%ld] \n -- \n", t/(60*60), tStep/(60*60)); // DEBUG
  } while (tStep > 0);

  ERRCODE(EN_report(pp));

  ERRCODE(EN_closeH(pp));
  ERRCODE(EN_closeQ(pp));

  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code when running Joint Quality, Hydraulic Analysis [%d], Message: \n%s\n", errcode, errmsg);
  }
}

// Establishing Error Handler and indexer for various Network components. More to be added as needed
// Junction Tank and Reservoir are EN_addnode() calls, Pipe and Pump are EN_addlink() calls
// inputs are all required, and represent the project - a tag for the node, and a storage map to cirrelate EPANET's ID and its Tag
// Only addJunction is annotated, as these functions are variations on the same.

// Junctions
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
      printf("Error Code generating Node '%s'  [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
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
      printf("Error Code generating Tank '%s' [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
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
      printf("Error Code generating Reservoir '%s'  [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
  }
}

// Common Links
void addPipe(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::map<int, std::string>>& indexStorage) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addlink(pp, nodeTag, EN_PIPE, junctionOne, junctionTwo, &index));

  indexStorage[2][index] = std::format("nID[{}] : tag[{}] : type[Pipe]", index, nodeTag);

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code when generating Pipe '%s' [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
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
      printf("Error Code generating Pump '%s'  [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
  }
}


// Establishing Error Handler and Indexer for Curves. All Types of curves accounted for
// Completed using an EN_addcurve(), EN_setcurve(), and EN_setcurvetype() calls
// inputs are all required; except for curveType integer, which will default to '5' - Generic Curve. Can be set later with another call
void addCurve(EN_Project pp, const char *nodeTag, int arLen, double xArray[], double yArray[], std::map<int, std::map<int, std::string>>& indexStorage, int curveType = 5) {
  int errcode = 0;
  char errmsg[EN_MAXMSG + 1];
  int index;

  ERRCODE(EN_addcurve(pp, nodeTag));
  EN_getcurveindex(pp, nodeTag, &index);
  ERRCODE(EN_setcurve(pp, index, xArray, yArray, arLen));

  if (curveType == 1) {
    ERRCODE(EN_setcurvetype(pp, index, EN_VOLUME_CURVE));
    indexStorage[3][index] = std::format("nID[{}] : tag[{}] : type[Tank Vol/Dep Curve]", index, nodeTag);

  } else if (curveType == 2) {
    ERRCODE(EN_setcurvetype(pp, index, EN_PUMP_CURVE));
    indexStorage[3][index] = std::format("nID[{}] : tag[{}] : type[Pump H/Q Curve]", index, nodeTag);

  } else if (curveType == 3) {
    ERRCODE(EN_setcurvetype(pp, index, EN_EFFIC_CURVE));
    indexStorage[3][index] = std::format("nID[{}] : tag[{}] : type[Pump Eff/Q Curve]", index, nodeTag);

  } else if (curveType == 4) {
    ERRCODE(EN_setcurvetype(pp, index, EN_HLOSS_CURVE));
    indexStorage[3][index] = std::format("nID[{}] : tag[{}] : type[Valve Hd/Q Curve]", index, nodeTag);

  } else if (curveType == 5) {
    ERRCODE(EN_setcurvetype(pp, index, EN_GENERIC_CURVE));
    indexStorage[3][index] = std::format("nID[{}] : tag[{}] : type[Generic Curve]", index, nodeTag);

  } else { printf("Curve Type must be designated between 1 -- 5 \n ----------------- \n -  1  :  TnkV   - \n -  2  :  PmpH   - \n -  3  :  PmpE   - \n -  4  :  ValH   - \n -  5  :  Gnrc   - \n ----------------- \n\n Curve could not be established."); return; }

  // Retrieve error message if any
  if (errcode) {
      EN_geterror(errcode, errmsg, EN_MAXMSG);
      printf("Error Code generating Curve '%s'  [%d], Message: \n%s\n", nodeTag, errcode, errmsg);
  }
}
