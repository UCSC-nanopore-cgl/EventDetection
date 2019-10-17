# nanopore-toolkit
## Requirements
* gcc >= 5.5
* cmake
* Boost >=1.69.0 (system date_time filesystem iostreams coroutine context)
  * `wget -O boost_1_69_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.69.0/boost_1_69_0.tar.gz/download`
  * `tar xzf boost_1_69_0.tar.gz >/dev/null`
  * `cd boost_1_69_0/`
  * `./bootstrap.sh --with-libraries=system,date_time,filesystem,iostreams,coroutine,context >/dev/null #--prefix=/usr/`
  * `sudo ./b2 && sudo ./b2 install`
* hdf5 >= 1.10.4 (Needs to be built with --enable-threadsafe for multithreading)
  * `export CFLAGS="$CFLAGS -fPIC"`
  * `export CXXFLAGS="$CXXFLAGS -fPIC"`
  * `wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.4/src/hdf5-1.10.4.tar.gz`
  * `tar -xzf hdf5-1.10.4.tar.gz || exit 255`
  * `cd hdf5-1.10.4 && ./configure --prefix /usr/local --enable-threadsafe --disable-hl && make && sudo make install`

## Installation
* `python setup.py install`
* `python setup.py test`

## [Manual](Manual.md)