//
// Created by Andrew Bailey on 03/15/19.
//

// local lib
#include "EventDetectMain.hpp"
// fast5
#include <fast5.hpp>
// std libs
#include <map>
#include <iostream>
#include <string>


using namespace std;



int print_usage(int argc, char **argv);

static std::map< std::string, std::function<int(int, char**)> > programs = {
    {"event_detect", detect_main},
    {"help", print_usage},
    {"--help", print_usage}
};

int print_usage(int, char **)
{
  std::cout << "usage: nanopore-cli [options]" << std::endl;
  std::cout << "  valid commands: " << std::endl;
  for (const auto &item : programs){
    std::cout << "    " << item.first << std::endl;
  }
  std::cout << "  for help on given command, type main command --help" << std::endl;
  return 0;
}


int main(int argc, char** argv) {
  H5Eset_auto(0, nullptr, nullptr);
  int ret = 0;
  if(argc <= 1) {
    printf("error: no command provided\n");
    print_usage(argc - 1 , argv + 1);
    return 0;
  } else {
    std::string command(argv[1]);
    auto iter = programs.find(command);
    if (iter != programs.end())
      ret = iter->second( argc - 1, argv + 1);
    else
      ret = print_usage( argc - 1, argv + 1);
  }
  return ret;
}
