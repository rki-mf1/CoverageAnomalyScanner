/**
 * @file     src/parser.h
 * @brief    This file contains the program parser.
 * 
*/
#ifndef PARSER_
#define PARSER_

#include <argparse/argparse.hpp>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <string>



typedef struct{
    int tid_ = -1;
    hts_pos_t beg_ = -1;
    hts_pos_t end_ = -1;
} range_t;


void create_parser(argparse::ArgumentParser &parser, const int argc, char const *argv[]);


range_t parse_range(argparse::ArgumentParser &parser);









#endif /* PARSER_ */