#ifndef EPAHANDLER_H
#define EPAHANDLER_H

void runSetup(EN_Project& pp, int flowUnit, int headFormula, std::string reportFile, std::string outFile = "", std::string titleOne = "", std::string titleTwo = "", std::string titleThree = "");
void runHydraulics(EN_Project pp, std::string reportFile, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1);
void runQuality(EN_Project pp, std::string reportFile, int runMode = 1, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1);
void runStepwise(EN_Project pp);

void addTank(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage);
void addJunction(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage);
void addReservoir(EN_Project pp, const char *nodeTag, std::map<int, std::map<int, std::string>>& indexStorage);
void addPipe(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::map<int, std::string>>& indexStorage);
void addPump(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::map<int, std::string>>& indexStorage);

void addCurve(EN_Project pp, const char *nodeTag, int ArLen, double xArray[], double yArray[], std::map<int, std::map<int, std::string>>& indexStorage, int curveType = 5);

#endif
