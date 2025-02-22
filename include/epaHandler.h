#ifndef EPAHANDLER_H
#define EPAHANDLER_H

void runSetup(EN_Project pp, int flowUnit, int headFormula, std::string reportFile, std::string outFile = "");
void runHydraulics(EN_Project pp, std::string reportFile, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1);

#endif
