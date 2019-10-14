//
// Created by Andrew Bailey on 10/14/19.
//

#include <string>
#include <getopt.h>
#include <iostream>
#include <sstream>

using namespace std;

//
#define SUBPROGRAM "event_detect"
#define EVENT_DETECT_VERSION "0.0.1"
#define THIS_NAME "detect"
#define PACKAGE_BUGREPORT2 "https://github.com/UCSC-nanopore-cgl/EventDetection/issues"

static const char *EVENT_DETECT_VERSION_MESSAGE =
    SUBPROGRAM " Version " EVENT_DETECT_VERSION "\n";

static const char *EVENT_DETECT_USAGE_MESSAGE =
    "Usage: " THIS_NAME " " SUBPROGRAM " [OPTIONS] --alignment_files ALIGNMENT_FILES --positions_file POSITIONS_FILE --output_dir OUTPUT_DIR\n"
    "Converts alignment files into assignment files with kmers covering certain positions.\n"
    "\n"
    "  -v, --verbose                        display verbose output\n"
    "      --version                        display version\n"
    "      --help                           display this help and exit\n"
    "  -a, --alignment_files=DIR            directory of signalalign alignment files\n"
    "  -o, --output=PATH                    path and name of output bed file\n"
    "  -c, --ambig_chars=NUCLEOTIDES        a string containing all ambiguous characters (default is P which corresponds to cytosine and 5methylcytosine \n"
    "  -t, --threads=NUMBER                 number of threads\n"
    "  -l, --locks=NUMBER                   number of locks for multithreading\n"

    "\nReport bugs to " PACKAGE_BUGREPORT2 "\n\n";

namespace opt
{
static unsigned int verbose;
static uint64_t num_locks = 10000;
static std::string input_path;
static std::string output;
static bool embed;

static unsigned int threads;
}

static const char* shortopts = "t:e:o:i:vh";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
    { "verbose",          no_argument,       nullptr, 'v' },
    { "input_path",       required_argument, nullptr, 'i' },
    { "output",           optional_argument, nullptr, 'o' },
    { "embed",            no_argument,       nullptr, 'e' },
    { "threads",          optional_argument, nullptr, 't' },
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
      case 'i': arg >> opt::input_path; break;
      case 'e': opt::embed = true; break;
      case 'o': arg >> opt::output; break;
      case 't': arg >> opt::threads; break;
      case 'l': arg >> opt::num_locks; break;

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

  if(opt::output.empty()) {
    std::cerr << SUBPROGRAM ": a --output file must be provided\n";
    die = true;
  }

  if (die)
  {
    std::cout << "\n" << EVENT_DETECT_USAGE_MESSAGE;
    exit(EXIT_FAILURE);
  }
}


int detect_main(int argc, char** argv)
{
  parse_event_detect_main_options(argc, argv);
  return EXIT_SUCCESS;
}