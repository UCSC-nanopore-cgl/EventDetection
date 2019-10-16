//
// Created by Andrew Bailey on 2019-10-14.
//

#ifndef EVENT_DETECT_PYBIND_API_H
#define EVENT_DETECT_PYBIND_API_H

// local libs
#include "EventDetectMain.hpp"
#include "EventDetection.hpp"
// pybind
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

int add1(int i, int j) {
    return i + j;
}

using namespace pybind11;


PYBIND11_MODULE(bindings, module) {

//  class_<EventDetection>(module, "EventDetection")
//      .def(init<const std::string &, bool, const std::string & >(),
//           "Event detection class for handling individual Fast5 files ",
//           pybind11::arg("file_name"),
//           pybind11::arg("rw") = false,
//           pybind11::arg("type") = "")
//      .def("generate_events",
//           &EventDetection::generate_events,
//           R"pbdoc(
//           Create events from a fast5 file\n"
//           "@return event table)pbdoc")
//      .def("write_events_to_file",
//           &EventDetection::write_events_to_file,
//           R"pbdoc(
//           Write the vector of events to a tsv file
//           @param data: vector of EventDetection_Events from the fast5.hpp data structures
//           @param output_path: string of the path to write data)pbdoc",
//           pybind11::arg("output_path"));

  module.doc() = R"pbdoc(
        Event Detection c++ Wrapper
        -----------------------
           add
           event_detect
    )pbdoc";

  module.def("event_detect", &event_detect, R"pbdoc(
         Event detection for just a single fast5 read
         @param fast5_path : path to fast5 file
         @param output_dir : optional path to output directory to write tsv files
         @param embed : optional argument to embed fast5 files with event detections
         @param type : optional argument to force "rna" or "dna" event detection parameters
    )pbdoc", pybind11::arg("fast5_path"), pybind11::arg("output_dir") = "", pybind11::arg("embed") = false, pybind11::arg("type") = "");

  module.def("multithread_event_detect", &multithread_event_detect, R"pbdoc(
             Multithread event detect individual nanopore reads
             @param fast5_dir : path to directory of fast5s
             @param output_dir : optional path to output directory to write tsv files
             @param embed : optional argument to embed fast5 files with event detections
             @param type : optional argument to force "rna" or "dna" event detection parameters
    )pbdoc", pybind11::arg("fast5_dir"), pybind11::arg("output_dir") = "", pybind11::arg("embed") = false, pybind11::arg("type") = "");

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  module.attr("__version__") = "dev";
#endif
}

#endif
