# TandemDuplicationFinder
Tool to find tandem duplications in genomic sequence data

## Installation

### Dependencies
`tdf` was implemented with a 64-bit Linux environment in mind. Using other operating systems might break its functionality and is not maintained. The `tdf` repository comes with git submodules by default, i.e. there is no need to install dependencies manually. However, in order to build [htslib](https://github.com/samtools/htslib) make sure you have _libbz2_ and _liblzma_ available on your system.

### Build
```
git clone https://github.com/Krannich479/TandemDuplicationFinder.git --recursive
cd TandemDuplicationFinder
make -C htslib
mkdir build
make
```

If the steps above compile successfully there should be a binary `tdf` in your working directory.

[Advanced build] If you already have a clone of one or multiple submodules available and don't want to download them again then you can omit the `--recursive` flag when cloning the github repository. In this case make sure to adjust the paths in the CXXFLAGS and LDLIBS variables within the Makefile. E.g. for htslib you have to manually set the path to its archive (_libhts.a_).
