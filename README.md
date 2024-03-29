# CAS - The Coverage Anomaly Scanner

[![C/C++ CI](https://github.com/rki-mf1/CoverageAnomalyScanner/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/rki-mf1/CoverageAnomalyScanner/actions/workflows/c-cpp.yml)
[![Version](https://img.shields.io/github/v/release/rki-mf1/CoverageAnomalyScanner)](https://github.com/rki-mf1/CoverageAnomalyScanner/releases/latest)
[![install with bioconda](https://img.shields.io/badge/install%20with-bioconda-brightgreen.svg?style=flat)](http://bioconda.github.io/recipes/coverageanomalyscanner/README.html)

Tool to find, interpret and convert unexpected changes in read coverage in genomic sequence data

## Table of Contents
1. [Installation](#installation) <br>
    1.1 [Dependencies](#dependencies) <br>
    1.2 [Install with bioconda](#install-with-bioconda)<br>
    1.3 [Build from source](#build-from-source) <br>
2. [Program execution](#program-execution) <br>
    2.1 [Running the CoverageAnomalyScanner](#running-the-coverageanomalyscanner) <br>
    2.2 [Output](#output) <br>
    2.3 [Visualizing CAS' metadata](#visualizing-cas-metadata) <br>
    2.4 [Help](#help) <br>
    
## Installation

### Dependencies
_cas_ was implemented with a 64-bit Linux environment in mind. Using other operating systems might break its functionality and are not maintained. The easiest way to install _cas_ is with bioconda.  If you like to build _cas_ from source then follow the description in the [Build from source](#build-from-source) section. The _cas_ repository comes with git submodules by default, i.e. there is no need to install direct dependencies manually. However, in order to build [htslib](https://github.com/samtools/htslib) from source make sure you have _libbz2_ and _liblzma_ available on your system, e.g. with root privileges on ubuntu linux use `sudo apt install libbz2-dev lzma`. To build the _cas_ binary from source you need a C++ compiler that supports the C++17 standard (tested with gcc v9.4.0, but should require only gcc >=v8.3.0).

### Install with bioconda
```
conda install -c bioconda coverageanomalyscanner
```

### Build from source
```
git clone https://github.com/rki-mf1/CoverageAnomalyScanner.git --recursive
cd CoverageAnomalyScanner 
make -C htslib
mkdir build
make
```

If the steps above compile successfully there should be a binary `cas` in your working directory.

[Advanced build] If you already have a clone of one or multiple submodules available and don't want to download them again then you can omit the `--recursive` flag when cloning the github repository. In this case make sure to adjust the paths in the CXXFLAGS and LDLIBS variables within the Makefile. E.g. for htslib you have to manually set the path to its archive (_libhts.a_).

## Program execution

### Running the CoverageAnomalyScanner
The `cas` program has two intended ways of starting a scan (see examples below). The first example uses a BAM file and three separate input parameters to specify a window of the genome to be scanned. The `--chr` parameter is the zero-based chromosome index from the mandatory BAM file. If the index of the desired chromosome is unclear use `samtools view -H` on the BAM file to determine the index from the `@SQ` fields.
```
cas --bam FILE --chr 0 --start 400 --end 900
```
The second way of starting the program uses a BAM file and the input format of a [SAMTOOLS](http://www.htslib.org/)-like genomic range. Here, the chromosome name itself can be used together with a range.
```
cas --bam FILE --range chr1:400-900
```

### Output
A run of `cas` prints a report of the predicted positions with a read coverage anomaly inside the given genomic window. Moreover, a [VCF file](https://samtools.github.io/hts-specs/VCFv4.2.pdf) _predictedEvents.vcf_ is generated for all pairs of predicted positions where the increase and decrease in read coverage follows the (nested) parenthesis rules. I.e. an increase-decrease pair is interpreted as duplication event and a decrese-increase pair is interpreted as deletion event (deletions are _WIP_).

### Visualizing CAS' metadata
The GitHub repository provides a subfolder _util_ with a R script to visualize the metadata that `cas` is generating to determine coverage anomalies. The metadata itself is only written to disc if the program is (re)compiled with
```
make print
```
instead of only `make` as described in the [Build from source](#build-from-source) section. When using the printing program binary of `cas` a program execution generates a _coverage.csv_ (CSV) in the current working directory. The R script inside the _util_ subfolder reads the CSV file and generates plots which visualize the metadata. The most convenient method to generate the plots is to use R from the command line
```bash
chmod +x <path/to/util>/covplot.R
Rscript <path/to/util>/covplot.R <path/to>/coverage.csv
```
 
The commands above generate a _Rplots.pdf_ file with plots. If there is no Rscript command line application available in your working environment the plots can be manually generated in R via
```R
coverage <- read.delim("coverage.csv", header=FALSE, row.names=1)
```

and an execution of the `VARIABLES` and `PLOTS` sections of the _covplot.R_.


### Help
```
Usage: cas [-h] --bam FILE [--chr INT] [--start INT] [--end INT] [--range VAR] [--threshold FLOAT]

CAS - The Coverage Anomaly Scanner

Optional arguments:
  -h, --help            shows help message and exits 
  -v, --version         prints version information and exits 
  -b, --bam FILE        Read alignment in BAM file format. [required]
  -c, --chr INT         A 0-based chromosome index from the BAM file. 
  -s, --start INT       Start position in the chromosome. 
  -e, --end INT         End position in the chromosome. 
  -r, --range           Genomic range in SAMTOOLS style, e.g. chr:beg-end 
  --threshold FLOAT     Constant threshold for the anomaly detection. Overwrites the internal default formula. 

by T. Krannich (2022)
```

[BACK TO TOP](#table-of-contents)
