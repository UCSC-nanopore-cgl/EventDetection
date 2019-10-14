//
// Created by Andrew Bailey on 03/15/19.
//

// local lib
#include "EventDetectMain.hpp"
// boost
#include <boost/filesystem.hpp>
// fast5
#include <fast5.hpp>
// std libs
#include <map>
#include <cassert>
#include <iostream>
#include <string>


using namespace std;
using namespace boost::filesystem;



int print_usage(int argc, char **argv);

static std::map< std::string, std::function<int(int, char**)> > programs = {
    {"help", print_usage},
    {"--help", print_usage},
    {"detect", detect_main}
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
  H5Eset_auto(0, NULL, NULL);
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
}


// get channel parameters and scale raw ADC counts to get pA raw current
//fast5_raw_scaling channel_params = fast5_get_channel_params(hdf5_file);
//raw_table rt = fast5_get_raw_samples(hdf5_file, channel_params);
//
//// trim using scrappie's internal method
//// parameters taken directly from scrappie defaults
//int trim_start = 200;
//int trim_end = 10;
//int varseg_chunk = 100;
//float varseg_thresh = 0.0;
//trim_and_segment_raw(rt, trim_start, trim_end, varseg_chunk, varseg_thresh);
//
//// get events
//event_table et = detect_events(rt, *ed_params);