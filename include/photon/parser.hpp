#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#define NO_DATA_PARSED (-1)

namespace photon
{
    namespace parser
    {

        struct globals
        {
            int current_line;
            int data_parsed;
            std::vector<std::string> openTags;
        };

        class attribute
        {
        public:
            std::string TAG;
            std::string NAME;
            std::string VALUE;

            attribute(std::string tag, std::string attrName, std::string attrValue)
            {
                TAG = tag;
                NAME = attrName;
                VALUE = attrValue;
            }
        };

        std::vector<parser::attribute> fetch_attr(std::string line, int end, int start, std::string tagname);
        int get_height(attribute attr);
        int get_width(attribute attr);
        void fetch_starting_tag(std::string line, int index, globals& global);
        void fetch_endtag(std::string search_string, globals& global);
        void fetch_data(std::string search_string, globals& global, bool recurse);
        void encountered_dataORendtag(std::string line, int found, int dataEndPointAR, globals& global);
        void fetch_line(std::string line, int dataEndPointAR, globals& global);
        void parse(std::string path);
        void get_dimensions(std::vector<attribute> attrs, int &height, int &width);

        bool validate_data(std::string data);
    }
}