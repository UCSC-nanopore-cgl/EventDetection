//
// Created by Andrew Bailey on 10/14/19.
//

// local lib
#include "EventDetectMain.hpp"
#include "Utils.hpp"
// boost
#include <boost/filesystem.hpp>
// openmp
#include "omp.h"
// std libs
#include <string>
#include <getopt.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace boost::filesystem;
using namespace utils;

//
#define SUBPROGRAM "event_detect"
#define EVENT_DETECT_VERSION "0.0.1"
#define THIS_NAME "nanopore-cli"
#define PACKAGE_BUGREPORT2 "https://github.com/UCSC-nanopore-cgl/EventDetection/issues"

static const char *EVENT_DETECT_VERSION_MESSAGE =
    SUBPROGRAM " Version " EVENT_DETECT_VERSION "\n";

static const char *EVENT_DETECT_USAGE_MESSAGE =
    "Usage: " THIS_NAME " " SUBPROGRAM " [OPTIONS] --fast5 FAST5_PATH --embed\n"
    "Event detection for fast5 nanopore reads.\n"
    "\n"
    "  -v, --verbose                        display verbose output\n"
    "      --version                        display version\n"
    "      --help                           display this help and exit\n"
    "  -f, --fast5=PATH                     directory of fast5 files or an individual fast5 file\n"
    "  -o, --output_dir=PATH                path to output directory\n"
    "  -t, --threads=NUMBER                 number of threads\n"
    "  -e, --embed                          option to embed event table into fast5s\n"
    "  -r, --rna                            option to force all reads to be interpreted as RNA\n"
    "  -d, --dna                            option to force all reads to be interpreted as DNA\n"

    "\nReport bugs to " PACKAGE_BUGREPORT2 "\n\n";

namespace opt
{
static unsigned int verbose;
static std::string fast5_dir;
static std::string output;
static bool embed;
static bool rna;
static bool dna;
static unsigned int threads = 1;
}

static const char* shortopts = "t:o:i:vdreh";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
    { "fast5",            required_argument, nullptr, 'f' },
    { "embed",            no_argument,       nullptr, 'e' },
    { "output_dir",       optional_argument, nullptr, 'o' },
    { "threads",          optional_argument, nullptr, 't' },
    { "rna",              no_argument,       nullptr, 'r' },
    { "dna",              no_argument,       nullptr, 'd' },
    { "verbose",          no_argument,       nullptr, 'v' },
    { "help",             no_argument,       nullptr, OPT_HELP },
    { "version",          no_argument,       nullptr, OPT_VERSION },
    { nullptr, 0, nullptr, 0 }
};

void parse_event_detect_main_options(int argc, char** argv)
{
  bool die = false;
  for (char c; (c = getopt_long(argc, argv, shortopts, longopts, nullptr)) != -1;) {
    std::istringstream arg(optarg != nullptr ? optarg : "");
    switch (c) {
      case 'f': arg >> opt::fast5_dir; break;
      case 'e': opt::embed = true; break;
      case 'r': opt::rna = true; break;
      case 'd': opt::dna = true; break;
      case 'o': arg >> opt::output; break;
      case 't': arg >> opt::threads; break;

      case 'v': opt::verbose++; break;
      case OPT_HELP:
        std::cout << EVENT_DETECT_USAGE_MESSAGE;
        exit(EXIT_SUCCESS);
      case OPT_VERSION:
        std::cout << EVENT_DETECT_VERSION_MESSAGE;
        exit(EXIT_SUCCESS);
      default:
        string error = ": unreconized argument -";
        error += c;
        error += " \n";
        std::cerr << SUBPROGRAM + error;
        exit(EXIT_FAILURE);
    }
  }

  if (argc - optind > 0) {
    std::cerr << SUBPROGRAM ": too many arguments\n";
    die = true;
  }

  if(opt::rna && opt::dna) {
    std::cerr << SUBPROGRAM ": must select rna or dna but not both \n";
    die = true;
  }

  if (die)
  {
    std::cout << "\n" << EVENT_DETECT_USAGE_MESSAGE;
    exit(EXIT_FAILURE);
  }
}

void multithread_event_detect(string assignment_dir, string const &output_dir, bool embed, string const &type){
  path p(assignment_dir);
  throw_assert(output_dir.empty() || embed, "Must specify output directory or set embed to True");
  path output_path;
  if (!output_dir.empty()){
    output_path = make_dir(output_dir);
  }

  directory_iterator end_itr;
//    Get all tsvs to process
  vector<path> all_files;
  for (directory_iterator itr(p); itr != end_itr; ++itr) {
//        filter for files that are regular, end with fast5 and are not empty
    if (is_regular_file(itr->path()) and itr->path().extension().string() == ".fast5" and getFilesize(itr->path().string()) > 0 and fast5::File::is_valid_file(itr->path().string())) {
      all_files.push_back(itr->path());
    }
  }
  int64_t number_of_files = all_files.size();
  path* array_of_files = &all_files[0];

#pragma omp parallel for shared(array_of_files, number_of_files, embed, type, output_path) default(none)
  for(int64_t i=0; i < number_of_files; i++) {
    path current_file = array_of_files[i];
    event_detect(current_file.string(), output_path.string(), embed, type);
  }
}


void event_detect(string const &fast5_path, string const &output_dir, bool embed, string const &type) {
  EventDetection file_handler;
  file_handler.open(fast5_path, true, type);
  fast5::EventDetection_Events_Dataset event_data = file_handler.generate_events();
  if (embed){
    file_handler.add_eventdetection_events_dataset(event_data);
  }
  if (!output_dir.empty()){
    fast5::Raw_Samples_Params params = file_handler.get_raw_samples_params();
    string read_name =  params.read_id+".tsv";
    path output_file = ((path) output_dir)/ ((path) read_name);
    file_handler.write_events_to_file(event_data.first, output_file.string());
  }
}


int detect_main(int argc, char** argv)
{
  parse_event_detect_main_options(argc, argv);
#ifndef H5_HAVE_THREADSAFE
  if(opt::num_threads > 1) {
    fprintf(stderr, "You enabled multi-threading but you do not have a threadsafe HDF5\n");
    fprintf(stderr, "Please recompile built-in libhdf5 or run with -t 1\n");
    exit(1);
  }
#endif
  omp_set_num_threads(opt::threads);

  string type;
  throw_assert(!(opt::rna && opt::dna),
      "Cannot force both rna and dna event detection parameters")
  if (opt::rna){
    type = "rna";
  }
  else if (opt::dna){
    type = "dna";
  }

  path input_path(opt::fast5_dir);
  path output_path(opt::output);


  if (is_directory(input_path)) {
    multithread_event_detect(input_path.string(), output_path.string(), opt::embed, type);
  } else if (exists(input_path)) {
    if (!output_path.empty()){
      output_path = make_dir(output_path);
    }
    event_detect(input_path.string(), output_path.string(), opt::embed, type);
  }
  return EXIT_SUCCESS;
}