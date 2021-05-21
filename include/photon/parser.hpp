#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include "parser/cssPreoperties.hpp"

#include "dom.hpp"

#define NO_DATA_PARSED (-1)

namespace photon
{
    namespace parser
    {

        struct node
        {
            unsigned int id;
            unsigned int parent;
            std::string tag;
            std::map<std::string, std::string> attributes;
        };

        struct globals
        {
            int current_line;
            int data_parsed;
            std::vector<node> openTags;
            int _id = 0; // tag_id
            std::shared_ptr<_dom> dom;
            char on_next_line = 'A'; // * A -> CONTINUE AS NORMAL; D -> THERE ARE ATTRIBUTES ON THIS LINE
            std::vector<node> pending_nodes;
            std::map<unsigned int, std::string> elements;
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
        std::shared_ptr<_dom> parse(std::string path);
        void fetch_line(std::string line, globals &global);
        std::map<std::string, std::string> fetch_attr(std::string line, int start);
        void insert_node(unsigned int id, std::string tagname, std::map<std::string, std::string> attrs, globals &global);

        void get_dimensions(std::map<std::string, std::string> attrs, int &height, int &width);
        int validate_data(std::string data);
    }

    namespace cssparser
    {
        enum tag : int
        {
            with,
            height,
            border,
            border_with,
            border_color
        };

        struct Selector
        {
            //TODO: finish enums
            enum Type
            {
                tagName,
                className,
                id
            };
            Type type;
            std::string value;
            enum Relation
            {
                children,
                directChildren
            };
            Relation relation;
        };

        struct qery
        {
            std::vector<Selector> selector;
            std::unordered_map<int, float> tags;
        };

        struct definition
        {
            properties property;
            std::vector<std::string> short_hands; 
        };

        struct style
        {
            Selector selector;
            std::vector<definition> props;
        };

        std::vector<std::string> short_hands(std::string value);
        std::vector<definition> props(std::string content);
        size_t validate_file(std::string file);
        std::vector<style> parse(std::string path);
        std::vector<std::string> fetch_selectors(std::string selector_string);
    }
}