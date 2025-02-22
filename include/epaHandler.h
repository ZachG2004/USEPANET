#ifndef EPAHANDLER_H
#define EPAHANDLER_H

void runSetup(EN_Project& pp, int flowUnit, int headFormula, std::string reportFile, std::string outFile = "");
void runHydraulics(EN_Project pp, std::string reportFile, std::string inputFile = "", std::string runType = "noinput", int saveInput = 1);

void addTank(EN_Project pp, const char *nodeTag, std::map<int, std::string> indexStorage);
void addJunction(EN_Project pp, const char *nodeTag, std::map<int, std::string>& indexStorage);
void addReservoir(EN_Project pp, const char *nodeTag, std::map<int, std::string>& indexStorage);
void addPipe(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::string>& indexStorage);
void addPump(EN_Project pp, const char *nodeTag, const char *junctionOne, const char *junctionTwo, std::map<int, std::string>& indexStorage);

#endif
