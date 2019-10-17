# Nanopore-Toolkit Manual
## nanopore-cli
After installation `nanopore-cli` should be in your path.  

### event_detect
This will perform default event detection for a given set of nanopore reads. You can specify either one 
fast5 file or a path to a directory of fast5 files.
The event detection table can be embedded into the fast5 and/or written to files. 
By default, all reads will be checked to see if they are RNA or DNA but if you want to force a set of
 default parameters 
you can select `--rna` or `--dna`.
  

```
Usage: nanopore-cli event_detect [OPTIONS] --fast5 FAST5_PATH --embed 
   Event detection for fast5 nanopore reads.
   
     -v, --verbose                        display verbose output
         --version                        display version
         --help                           display this help and exit
     -f, --fast5=PATH                     directory of fast5 files or an individual fast5 file
     -o, --output_dir=PATH                path to output directory
     -t, --threads=NUMBER                 number of threads
     -e, --embed                          option to embed event table into fast5s
     -r, --rna                            option to force all reads to be interpreted as RNA
     -d, --dna                            option to force all reads to be interpreted as DNA
```


## Python Modules
### event_detect
There are 3 modules associated with event detection. 
* `from ntk* bindings import event_detect`
    * ```
        Event detection for just a single fast5 read
         @param fast5_path : path to fast5 file
         @param output_dir : optional path to output directory to write tsv files
         @param embed : optional argument to embed fast5 files with event detections
         @param type : optional argument to force "rna" or "dna" event detection parameters ("rna", "dna")
        ```
* `from ntk* bindings import multithread_event_detect`
    * ```
         Multithread event detect individual nanopore reads
         @param fast5_dir : path to directory of fast5s
         @param output_dir : optional path to output directory to write tsv files
         @param embed : optional argument to embed fast5 files with event detections
         @param type : optional argument to force "rna" or "dna" event detection parameters ("rna", "dna")
        ```
* `from ntk* bindings import EventDetection`
    * ```
        Event detection class for handling individual Fast5 files
         @param file_name : path to fast5 file
         @param rw : boolean option to write
         @param type : optional argument to force "rna" or "dna" event detection parameters ("rna", "dna")
        ```
    * `events, params = EventDetection.generate_events()`
        * This returns a list of events and parameters from the event detection