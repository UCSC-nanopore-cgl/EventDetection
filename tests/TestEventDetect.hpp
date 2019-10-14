//
// Created by Andrew Bailey on 10/14/19.
//

#ifndef EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_
#define EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_

// gtest
#include <gtest/gtest.h>
#include <gmock/gmock.h>
// boost
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
// fast5
#include "fast5.hpp"

using namespace boost::filesystem;
using namespace std;

namespace {
path DNA_FAST5;
path RNA_FAST5;
}

 class TestPaths : public testing::Environment {
 public:
  explicit TestPaths(const path &home) {
    DNA_FAST5 = home / (path) "tests/test_files/dna/LomanLabz_PC_20161025_FNFAB42699_MN17633_sequencing_run_20161025_E_coli_native_450bps_82361_ch6_read347_strand.fast5";
    RNA_FAST5 = home / (path) "tests/test_files/rna/DEAMERNANOPORE_20170922_FAH26525_MN16450_sequencing_run_MA_821_R94_NA12878_mRNA_09_22_17_67136_read_36_ch_218_strand.fast5";
  }
};

TEST (Fast5AccessTest, isValidFile) {
  EXPECT_TRUE(fast5::File::is_valid_file(RNA_FAST5.string()));
  EXPECT_TRUE(fast5::File::is_valid_file(DNA_FAST5.string()));
}


#endif //EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_
