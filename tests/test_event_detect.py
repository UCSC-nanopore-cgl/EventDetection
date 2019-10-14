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
from event_detect.bindings import add
from event_detect.event_detect import test


class CppTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        super(CppTests, cls).setUpClass()
        cls.HOME = '/'.join(os.path.abspath(__file__).split("/")[:-2])

    def test_cpp(self):
        print("\n\nTesting C++ code...")
        test_path = os.path.join(self.HOME, 'tests/bin', 'test_event_detect_cpp')
        print()
        subprocess.check_call([test_path, self.HOME])
        print()
        self.assertTrue(True)

    def test_pythonbinding(self):
        self.assertEqual(2, add(1, 1))

    def test_test(self):
        self.assertEqual(1, test(1))


if __name__ == '__main__':
    unittest.main()
