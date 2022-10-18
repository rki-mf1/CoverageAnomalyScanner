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

    parser.add_argument("-r", "--range")
        .help("Genomic range in SAMTOOLS style, e.g. chr:beg-end");

    parser.add_argument("--stddev-coeff")
        .metavar("INT")
        .help("Coefficient for the outlier threshold: INT * sd(data)")
        .default_value(3u)
        .scan<'u', unsigned>();

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    // sanity check if -c, -s or -e are used, than all three HAVE to be used
    if (parser.is_used("-c")) {
        if ( !parser.is_used("-s") ) {
            throw std::runtime_error("[Parser] All of -c -s -e have to be provided together.");
        }
        if ( !parser.is_used("-e") ) {
            throw std::runtime_error("[Parser] All of -c -s -e have to be provided together.");
        }
    }

    // sanity check: only use (-cse) OR -r
    if (parser.is_used("-c") && parser.is_used("-r")) {
        throw std::runtime_error("[Parser] Both input options -c and -r are not allowed together.");
    }

    // sanity check: force usage of (-cse) OR -r
    if (!parser.is_used("-c") && !parser.is_used("-r")) {
        throw std::runtime_error("[Parser] One of the input options -c and -r has to be provided.");
    }
}


range_t parse_range(argparse::ArgumentParser &parser){

    // read SAMTOOLS-like chromosome range, e.g. chrX:1,234-5,678
        
    samFile *fp;
    sam_hdr_t *hdr;

    range_t return_container;

    const char *bam_file  = parser.get<std::string>("--bam").c_str();

    if (!(fp = sam_open(bam_file, "r"))) {
        perror(bam_file);
        exit(1);
    }

    if (!(hdr = sam_hdr_read(fp))) {
        fprintf(stderr, "[Parser] Could not read BAM file header\n");
        exit(1);
    }
    
    const char *reg = parser.get<std::string>("--range").c_str();
    
    int flags = 0;

    while (*reg) {
        reg = sam_parse_region(hdr, reg, &return_container.tid_ , &return_container.beg_, &return_container.end_, flags);
        if (!reg) {
            fprintf(stderr, "[Parser] Failed to parse region\n");
            exit(1);
        }
#ifdef PRINT
        printf("[Parser] %-20s %12"PRIhts_pos" %12"PRIhts_pos"\n",
            return_container.tid_ == -1 ? "*" : hdr->target_name[return_container.tid_],
            return_container.beg_, return_container.end_);
#endif
    }

    sam_hdr_destroy(hdr);
    sam_close(fp);

    return return_container;
}
