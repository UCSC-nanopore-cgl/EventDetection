//
// Created by Andrew Bailey on 10/14/19.
//
// local lib
#include "EventDetection.hpp"


#ifndef EVENTDETECTION_SRC_EVENTDETECTMAIN_HPP_
#define EVENTDETECTION_SRC_EVENTDETECTMAIN_HPP_

int detect_main(int argc, char** argv);

void event_detect(string const &fast5_path, string const &output_dir = string(), bool embed = false, string const &type = string());
void multithread_event_detect(string assignment_dir, string const &output_dir = string(), bool embed = false, string const &type = string());

#endif //EVENTDETECTION_SRC_EVENTDETECTMAIN_HPP_
