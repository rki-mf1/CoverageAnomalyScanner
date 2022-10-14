#include "parser.h"


void create_parser(argparse::ArgumentParser &parser, const int argc, char const *argv[]){

    parser.add_description("CAS - The Coverage Anomaly Scanner");
    parser.add_epilog("by T. Krannich (2022)");

    parser.add_argument("-b", "--bam")
        .required()
        .metavar("FILE")
        .help("Read alignment in BAM file format.");

    parser.add_argument("-c", "--chr")
        .metavar("INT")
        .help("A 0-based chromosome index from the BAM file.")
        .scan<'i', int>();

    parser.add_argument("-s", "--start")
        .metavar("INT")
        .help("Start position in the chromosome.")
        .scan<'i', int>();

    parser.add_argument("-e", "--end")
        .metavar("INT")
        .help("End position in the chromosome.")
        .scan<'i', int>();

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }


}