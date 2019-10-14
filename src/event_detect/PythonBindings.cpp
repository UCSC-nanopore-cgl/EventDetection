//
// Created by Andrew Bailey on 2019-10-14.
//

#ifndef EMBED_PYBIND_API_H
#define EMBED_PYBIND_API_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

int add1(int i, int j) {
    return i + j;
}

using namespace pybind11;


PYBIND11_MODULE(bindings, module) {

  module.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: cmake_example
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

  module.def("add", &add1, R"pbdoc(
        Add two numbers
        Some other explanation about the add function.
    )pbdoc");

  module.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers
        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  module.attr("__version__") = "dev";
#endif
}

#endif
