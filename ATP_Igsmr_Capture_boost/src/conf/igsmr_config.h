#ifndef __igsmr_config_h
#define __igsmr_config_h

#include <string>
#include "singleton.h"
#include "config_parser.h"

class IgsmrConfig {
private:
    ConfigParser parser_;

private:
    friend class Singleton<IgsmrConfig>;

    IgsmrConfig();
    void init(int argc, char *argv[]);

public:
    std::string getLogDir();
    std::string getConfigFile();
    std::string getMT1DTESerial();
    std::string getMT1DCESerial();
    std::string getMT2DTESerial();
    std::string getMT2DCESerial();
    std::string getFilePrefix();
    int getFileSliceSize();
    std::string getIPAddress();
    int getPort();
    int getPollTimeout();
    int getDaemonMode();
    void print(std::ostream &out);

    static void initialize(int argc, char *argv[]);
    static IgsmrConfig &getInstance();

private:
    ConfigParser &get_config_parser(); 
};

#endif
