//
// Created by Andrew Bailey on 10/14/19.
//

// local lib
#include "EventDetection.hpp"
#include "Utils.hpp"

using namespace fast5;
using namespace utils;

/**
 * Create events from a fast5 file
 * @return event table
 */
EventDetection_Events_Dataset EventDetection::generate_events() {
  Channel_Id_Params samples_params = get_channel_id_params();
// Read the actual samples
  Raw_Samples_Dataset raw_sample = get_raw_samples_dataset();
// trim using scrappie's internal method
  raw_table rt;
  rt.raw = &raw_sample.first[0];
  rt.n = raw_sample.first.size();
  rt.start = 0;
  rt.end = rt.n;
// parameters taken directly from scrappie defaults
  int trim_start = 200;
  int trim_end = 10;
  int varseg_chunk = 100;
  float varseg_thresh = 0.0;
  trim_and_segment_raw(rt, trim_start, trim_end, varseg_chunk, varseg_thresh);
//  get correct event detection parameters
  const detector_param *ed_params;
  if (is_rna){
    ed_params = &event_detection_rna;
  } else {
    ed_params = &event_detection_defaults;
  }
  event_table et = detect_events(rt, *ed_params);

  vector<EventDetection_Event> all_events;
  all_events.reserve(et.n);
  for (uint64_t i = et.start; i < et.n; i++){
    event_t event_i = et.event[i];
    EventDetection_Event event_data = {event_i.mean, event_i.stdv, event_i.start, event_i.length};
    all_events.push_back(event_data);
  }
//  EventDetection_Event
  Raw_Samples_Params raw_samples_params = get_raw_samples_params();
  EventDetection_Events_Params event_params;
  event_params.read_id = raw_samples_params.read_id;
  event_params.read_number = raw_samples_params.read_number;
  event_params.scaling_used = -1;
  event_params.start_mux = raw_samples_params.start_mux;
  event_params.start_time = raw_samples_params.start_time;
  event_params.duration = raw_samples_params.duration;
  event_params.median_before = -1;
  event_params.abasic_found = -1;
  EventDetection_Events_Dataset ede_ds;
  ede_ds.first = all_events;
  ede_ds.second = event_params;

  assert(rt.n > 0);
  assert(et.n > 0);
  return ede_ds;
}

/**
 *
 */
void EventDetection::write_events_to_file(std::vector< EventDetection_Event > data, std::string output_path){
  std::ofstream out_file;
  out_file.open(output_path);
  out_file << "start" << '\t' <<  "length" << "\t" << "mean" << '\t' << "stdv" << '\n';
  for (auto &event: data){
    out_file << event.start << '\t' <<  event.length << "\t" << event.mean << '\t' << event.stdv << '\n';
  }
  out_file.close();
}