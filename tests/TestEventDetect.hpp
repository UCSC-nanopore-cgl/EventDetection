//
// Created by Andrew Bailey on 10/14/19.
//

#ifndef EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_
#define EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_

// local libs
#include "EventDetection.hpp"
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
path DNA_WITH_EVENTS;
}

class TestPaths : public testing::Environment {
 public:
  explicit TestPaths(const path &home) {
    DNA_FAST5 = home / (path) "tests/test_files/dna/LomanLabz_PC_20161025_FNFAB42699_MN17633_sequencing_run_20161025_E_coli_native_450bps_82361_ch6_read347_strand.fast5";
    RNA_FAST5 = home / (path) "tests/test_files/rna/DEAMERNANOPORE_20170922_FAH26525_MN16450_sequencing_run_MA_821_R94_NA12878_mRNA_09_22_17_67136_read_36_ch_218_strand.fast5";
    DNA_WITH_EVENTS = home / (path) "tests/test_files/dna/LomanLabz_PC_20161025_FNFAB42699_MN17633_sequencing_run_20161025_E_coli_native_450bps_82361_ch92_read1108_strand.fast5";
  }
};

TEST (Fast5AccessTest, isValidFile) {
  EXPECT_TRUE(fast5::File::is_valid_file(RNA_FAST5.string()));
  EXPECT_TRUE(fast5::File::is_valid_file(DNA_FAST5.string()));
}

TEST (EventDetectTest, generate_events) {
  path tmp = temp_directory_path();
  path TMP_RNA = tmp/"rna.fast5";
  path TMP_DNA = tmp/"dna.fast5";
  if (exists(TMP_DNA)){
    remove(TMP_DNA);
  }
  if (exists(TMP_RNA)){
    remove(TMP_RNA);
  }
  copy_file(RNA_FAST5, TMP_RNA);
  copy_file(DNA_FAST5, TMP_DNA);

//  RNA
  EventDetection file;
  file.open(TMP_RNA.string());
  fast5::EventDetection_Events_Dataset data = file.generate_events();
  EXPECT_TRUE(!data.first.empty());
  file.close();
//  DNA
  EventDetection file2;
  file2.open(TMP_DNA.string(), true);
  fast5::EventDetection_Events_Dataset data2 = file2.generate_events();
//  confirm embedding works
  file2.add_eventdetection_events_dataset(data2);
  std::vector<fast5::EventDetection_Event> data3 = file2.get_eventdetection_events();
  EXPECT_TRUE(!data2.first.empty());
  EXPECT_EQ(data3, data2.first);
  file2.close();
}

TEST (EventDetectTest, new_group_info) {
  path tmp = temp_directory_path();
  path TMP_DNA = tmp/"dna.fast5";
  path TMP_RNA = tmp/"rna.fast5";

  if (exists(TMP_DNA)){
    remove(TMP_DNA);
  }
  if (exists(TMP_RNA)){
    remove(TMP_RNA);
  }

  copy_file(DNA_WITH_EVENTS, TMP_DNA);
  copy_file(RNA_FAST5, TMP_RNA);


  EventDetection file;
  file.open(TMP_DNA.string());
  fast5::Group_Read_ID data2 = file.new_group_info();

  EXPECT_EQ(data2.gr, "002");
  EXPECT_EQ(data2.rn, "Read_1108");
  EventDetection file2;
  file2.open(TMP_RNA.string());
  fast5::Group_Read_ID data3 = file2.new_group_info();
  EXPECT_EQ(data3.gr, "000");
  EXPECT_EQ(data3.rn, "Read_36");
}

TEST (EventDetectTest, is_read_rna) {
  path tmp = temp_directory_path();
  path TMP_DNA = tmp/"dna.fast5";
  path TMP_RNA = tmp/"rna.fast5";

  if (exists(TMP_DNA)){
    remove(TMP_DNA);
  }
  if (exists(TMP_RNA)){
    remove(TMP_RNA);
  }

  copy_file(DNA_WITH_EVENTS, TMP_DNA);
  copy_file(RNA_FAST5, TMP_RNA);

  EventDetection file;
  file.open(TMP_RNA.string());
  bool rna = file.is_read_rna();
  EXPECT_TRUE(rna);

  EventDetection file2;
  file2.open(TMP_DNA.string());
  bool dna = file2.is_read_rna();
  EXPECT_FALSE(dna);

  EventDetection file3;
  file3.open(TMP_RNA.string(),false, "dna");
  EXPECT_FALSE(file3.is_rna);

}

#endif //EVENTDETECTION_TESTS_TESTEVENTDETECT_HPP_
