# CAS - The Coverage Anomaly Scanner 
Tool to find, interpret and convert unexpected changes in read coverage in genomic sequence data

## Installation

### Dependencies
`cas` was implemented with a 64-bit Linux environment in mind. Using other operating systems might break its functionality and is not maintained. The `cas` repository comes with git submodules by default, i.e. there is no need to install dependencies manually. However, in order to build [htslib](https://github.com/samtools/htslib) make sure you have _libbz2_ and _liblzma_ available on your system. To build the `cas` binary from source you need a C++ compiler that supports the C++17 standard (tested with gcc v9.4.0, but should require only gcc >=v8.3.0).

### Build
```
git clone https://github.com/Krannich479/CoverageAnomalyScanner.git --recursive
cd CoverageAnomalyScanner 
make -C htslib
mkdir build
make
```

If the steps above compile successfully there should be a binary `cas` in your working directory.

[Advanced build] If you already have a clone of one or multiple submodules available and don't want to download them again then you can omit the `--recursive` flag when cloning the github repository. In this case make sure to adjust the paths in the CXXFLAGS and LDLIBS variables within the Makefile. E.g. for htslib you have to manually set the path to its archive (_libhts.a_).

## Program execution

```
Usage: cas [-h] --bam FILE [--chr INT] [--start INT] [--end INT]

CAS - The Coverage Anomaly Scanner

Optional arguments:
  -h, --help            shows help message and exits 
  -v, --version         prints version information and exits 
  -b, --bam FILE        Read alignment in BAM file format. [required]
  -c, --chr INT         A 0-based chromosome index from the BAM file. 
  -s, --start INT       Start position in the chromosome. 
  -e, --end INT         End position in the chromosome. 

by T. Krannich (2022)
```
