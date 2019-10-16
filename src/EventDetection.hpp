//
// Created by Andrew Bailey on 10/14/19.
//

#ifndef EVENTDETECTION_SRC_EVENTDETECTION_HPP_
#define EVENTDETECTION_SRC_EVENTDETECTION_HPP_

// built in
#include <string>
#include <cmath>
// fast5
#include "fast5.hpp"
// c scripts
extern "C" {
#include "event_detection.h"
#include "scrappie_common.h"
}

using namespace std;

 class EventDetection : public fast5::File {
  public:
   fast5::EventDetection_Events_Dataset generate_events();
   static void write_events_to_file(std::vector< fast5::EventDetection_Event > data, const string &output_path);
};

#endif //EVENTDETECTION_SRC_EVENTDETECTION_HPP_
