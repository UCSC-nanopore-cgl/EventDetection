//
// Created by Andrew Bailey on 10/14/19.
//

// local headers
#include "TestEventDetect.hpp"
// fast5
#include "fast5.hpp"

using namespace boost::filesystem;
using namespace std;

int main(int argc, char **argv) {
  H5Eset_auto(0, nullptr, nullptr);
  testing::InitGoogleMock(&argc, argv);
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new TestPaths(argv[1]));
  return RUN_ALL_TESTS();
}
