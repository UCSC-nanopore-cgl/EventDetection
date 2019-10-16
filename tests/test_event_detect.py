#!/usr/bin/env python3
"""Testing script wrapper for event detection tests """
########################################################################
# File: test_event_detect.py
#  executable: test_event_detect.py
#
# Author: Andrew Bailey
# History: 10/14/19 Created
########################################################################


import unittest
import subprocess
import os
import tempfile
import shutil

from event_detect.bindings import event_detect, multithread_event_detect
from event_detect.event_detect import test


class CppTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        super(CppTests, cls).setUpClass()
        cls.HOME = '/'.join(os.path.abspath(__file__).split("/")[:-2])
        cls.RNA_READ = os.path.join(cls.HOME,
                                    "tests/test_files/rna/DEAMERNANOPORE_20170922_FAH26525_MN16450_sequencing_run_MA_"
                                    "821_R94_NA12878_mRNA_09_22_17_67136_read_36_ch_218_strand.fast5")

    def test_cpp(self):
        print("\n\nTesting C++ code...")
        test_path = os.path.join(self.HOME, 'tests/bin', 'test_event_detect_cpp')
        print()
        subprocess.check_call([test_path, self.HOME])
        print()
        self.assertTrue(True)

    def test_test(self):
        self.assertEqual(1, test(1))

    def test_event_detect(self):
        with tempfile.TemporaryDirectory() as tempdir:
            fast5_file = os.path.join(tempdir, "rna.fast5")
            shutil.copyfile(self.RNA_READ, fast5_file)
            event_detect(fast5_file, embed=True)
            self.assertEqual(1, 1)

    def test_multithread_event_detect(self):
        with tempfile.TemporaryDirectory() as tempdir:
            fast5_file = os.path.join(tempdir, "rna.fast5")
            shutil.copyfile(self.RNA_READ, fast5_file)
            multithread_event_detect(tempdir, embed=True)
            self.assertEqual(1, 1)


if __name__ == '__main__':
    unittest.main()
