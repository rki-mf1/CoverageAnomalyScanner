# CAS - The Coverage Anomaly Scanner 
Tool to find, interpret and convert unexpected changes in read coverage in genomic sequence data

## Installation

### Dependencies
`cas` was implemented with a 64-bit Linux environment in mind. Using other operating systems might break its functionality and is not maintained. The `cas` repository comes with git submodules by default, i.e. there is no need to install dependencies manually. However, in order to build [htslib](https://github.com/samtools/htslib) make sure you have _libbz2_ and _liblzma_ available on your system.

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

## Program execution (CLI will get reworked until the final release!)

```
./cas <BAM> <CHROM_ID> <START> <END>
```

where \<BAM\> is a set of mapped reads in SAM/BAM alignment format, \<CHROM_ID\> is the 0-based index of a chromosome to from the \<BAM\> header and \<START\>/\<END\> are the chromosome's start and end position, respectively.
