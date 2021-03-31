#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#define NO_DATA_PARSED (-1)

namespace photon
{
    namespace parser
    {


        struct node
        {
            unsigned int id = 0;
            std::string tag;
        };

        struct globals
        {
            int current_line;
            int data_parsed;
            std::vector<node> openTags;
            int _id = 0; // tag_id
        };

        class attribute
        {

        public:
            int TAG;
            std::string NAME;
            std::string VALUE;

            attribute(int tag, std::string attrName, std::string attrValue)
            {
                TAG = tag;
                NAME = attrName;
                VALUE = attrValue;
            }
        };

        int get_height(attribute attr);
        int get_width(attribute attr);
        void fetch_starting_tag(std::string line, int index, globals &global);
        void fetch_endtag(std::string search_string, globals &global);
        void fetch_data(std::string search_string, globals &global, bool recurse);
        void encountered_dataORendtag(std::string line, int found, int dataEndPointAR, globals &global);
        void fetch_line(std::string line, int dataEndPointAR, globals &global);
        void parse(std::string path);
        std::map<unsigned int, parser::attribute> fetch_attr(std::string line, int end, int start, globals &global);

        void get_dimensions(std::map<unsigned int, attribute> attrs, int &height, int &width);
        bool validate_data(std::string data);
    }
}