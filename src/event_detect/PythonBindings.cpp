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
using namespace fast5;

PYBIND11_MODULE(bindings, module) {

  class_<EventDetection>(module, "EventDetection")
      .def(init<const std::string &, bool, const std::string & >(),
           "Event detection class for handling individual Fast5 files ",
           pybind11::arg("file_name"),
           pybind11::arg("rw") = false,
           pybind11::arg("type") = "")
      .def("generate_events",
           &EventDetection::generate_events,
           R"pbdoc(
           Create events from a fast5 file\n"
           "@return event table)pbdoc");

  class_<EventDetection_Event>(module, "EventDetection_Event")
      .def(init<>())
      .def_readwrite("mean", &EventDetection_Event::mean)
      .def_readwrite("stdv", &EventDetection_Event::stdv)
      .def_readwrite("start", &EventDetection_Event::start)
      .def_readwrite("length", &EventDetection_Event::length);

  class_<fast5::EventDetection_Events_Params>(module, "EventDetection_Events_Params")
      .def(init<>())
      .def_readwrite("read_id", &EventDetection_Events_Params::read_id)
      .def_readwrite("scaling_used", &EventDetection_Events_Params::scaling_used)
      .def_readwrite("start_mux", &EventDetection_Events_Params::start_mux)
      .def_readwrite("start_time", &EventDetection_Events_Params::start_time)
      .def_readwrite("duration", &EventDetection_Events_Params::duration)
      .def_readwrite("median_before", &EventDetection_Events_Params::median_before)
      .def_readwrite("abasic_found", &EventDetection_Events_Params::abasic_found);

  module.doc() = R"pbdoc(
        Event Detection c++ Wrapper
        -----------------------
           add
           event_detect)pbdoc";

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
