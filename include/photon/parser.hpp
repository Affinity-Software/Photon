#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#include "dom.hpp"

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
            _dom dom;
            char on_next_line = 'A'; // * A -> CONTINUE AS NORMAL; D -> THERE ARE ATTRIBUTES ON THIS LINE
            std::vector<dom::nodeInternal> pending_nodes;
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

        int get_height(std::string _value);
        int get_width(std::string _value);
        void fetch_starting_tag(std::string line, int index, globals &global);
        void fetch_endtag(std::string search_string, globals &global);
        void fetch_data(std::string search_string, globals &global, bool recurse);
        void parse(std::string path);
        void fetch_line(std::string line, globals &global);
        std::map<std::string, std::string> fetch_attr(std::string line, int end, int start);

        void get_dimensions(std::map<std::string, std::string> attrs, int &height, int &width);
        int validate_data(std::string data);
    }
}