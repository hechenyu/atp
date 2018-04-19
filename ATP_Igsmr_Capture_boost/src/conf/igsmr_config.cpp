#include <iostream>
#include "igsmr_config.h"

IgsmrConfig::IgsmrConfig(): parser_("igsmr")
{
    parser_.add_option("help,h")
          .add_string_option("ConfigFile,F", "config file")
          .add_string_option("LogDir", "logging file directory")
          .add_string_option("MT1DTESerial")
          .add_string_option("MT1DCESerial")
          .add_string_option("MT2DTESerial")
          .add_string_option("MT2DCESerial")
          .add_string_option("FilePrefix", "file name prefix")
          .add_int_option("FileSliceSize", "file size per slice, by bytes")
          .add_string_option("IPAddress", "udp receiver ip")
          .add_int_option("Port", "udp receiver port")
          .add_int_option("PollTimeout", "poll timeout(milliseconds)")
          .add_int_option("DaemonMode", "1 for daemon mode, 0 for front end mode");
}

std::string IgsmrConfig::getLogDir()
{
    return parser_.get_string_variables("LogDir", ".");
}

std::string IgsmrConfig::getConfigFile()
{
    return parser_.get_string_variables("ConfigFile", "IgsmrCaptureSet.ini");
}

std::string IgsmrConfig::getMT1DTESerial()
{
    return parser_.get_string_variables("MT1DTESerial", "/dev/null");
}

std::string IgsmrConfig::getMT1DCESerial()
{
    return parser_.get_string_variables("MT1DCESerial", "/dev/null");
}

std::string IgsmrConfig::getMT2DTESerial()
{
    return parser_.get_string_variables("MT2DTESerial", "/dev/null");
}

std::string IgsmrConfig::getMT2DCESerial()
{
    return parser_.get_string_variables("MT2DCESerial", "/dev/null");
}

int IgsmrConfig::getFileSliceSize()
{
    const int _10M = 10000000;
    return parser_.get_int_variables("FileSliceSize", _10M);
}

std::string IgsmrConfig::getFilePrefix()
{
    return parser_.get_string_variables("FilePrefix", "/IgsmrRecord/ATP_Igsmr_MT");
}

std::string IgsmrConfig::getIPAddress()
{
    return parser_.get_string_variables("IPAddress", "0.0.0.0");
}

int IgsmrConfig::getPort()
{
    return parser_.get_int_variables("Port", 0);
}

int IgsmrConfig::getPollTimeout()
{
    return parser_.get_int_variables("PollTimeout", 300);
}

int IgsmrConfig::getDaemonMode()
{
    return parser_.get_int_variables("DaemonMode", 0);
}

void IgsmrConfig::init(int argc, char *argv[])
{
    parser_.parse_command_line(argc, argv);
    if (parser_.has_parsed_option("help")) {
        parser_.print_options_description(std::cout);
        exit(1);
    }

    parser_.parse_config_file(getConfigFile());
}

void IgsmrConfig::initialize(int argc, char *argv[])
{
    IgsmrConfig &config = Singleton<IgsmrConfig>::getInstance();
    config.init(argc, argv);
}

IgsmrConfig &IgsmrConfig::getInstance()
{
    return Singleton<IgsmrConfig>::getInstance();
}

void IgsmrConfig::print(std::ostream &out)
{
    out << "ConfigFile: " << getConfigFile() << '\n';
    out << "LogDir: " << getLogDir() << '\n';
    out << "MT1DTESerial: " << getMT1DTESerial() << '\n';
    out << "MT1DCESerial: " << getMT1DCESerial() << '\n';
    out << "MT2DTESerial: " << getMT2DTESerial() << '\n';
    out << "MT2DCESerial: " << getMT2DCESerial() << '\n';
    out << "IPAddress: " << getIPAddress() << '\n';
    out << "Port: " << getPort() << '\n';
    out << "FilePrefix: " << getFilePrefix() << '\n';
    out << "FileSliceSize: " << getFileSliceSize() << '\n';
    out << "PollTimeout: " << getPollTimeout() << '\n';
    out << "DaemonMode: " << getDaemonMode() << '\n';
}
