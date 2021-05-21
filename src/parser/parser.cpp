#include "photon/parser.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <tuple>

using namespace photon;

int parser::validate_data(std::string data)
{
   std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
   std::vector<char> ints = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
   for (int i = 0; data[i]; i++)
   {
      if (std::find(letters.begin(), letters.end(), data[i]) != letters.end() ||
          std::find(ints.begin(), ints.end(), data[i]) != ints.end())
         return i;
   }
   return -1;
}

std::map<std::string, std::string> parser::fetch_attr(std::string line, int start)
{
   int _id = 0;

   std::vector<std::string> parsed_attrs;
   std::map<std::string, std::string> attrs;

   std::string searchString = line.substr(start, line.find('>'));
   int count_attrs = 0;
   int done_attrs = 0;

   for (int i = 0; searchString[i]; i++)
   {
      if (searchString[i] == '=')
      {
         count_attrs++;
         if (searchString[i - 1] == ' ')
            searchString.replace(i - 1, 1, "");
         if (searchString[i + 1] == ' ')
            searchString.replace(i + 1, 1, "");
      }
   }

   for (int i = 0; searchString[i]; i++)
   {
      if (done_attrs >= count_attrs)
         break;

      char quote;
      if (searchString.find('"') != std::string::npos)
         quote = '"';
      else
         quote = '\'';

      if (std::find(parsed_attrs.begin(), parsed_attrs.end(), searchString.substr(0, searchString.find("="))) != parsed_attrs.end())
      {
         // since the attr has already been parsed we dont have to do anything with this attr
      }
      else
      {
         std::string name = searchString.substr(0, searchString.find("="));
         int start = searchString.find(quote);
         int end = searchString.substr(start + 1).find(quote);
         std::string value = searchString.substr(start + 1, end);

         size_t seperator = 2;
         size_t additionalSeperator = 1;

         if ((searchString.size() - 1) > (end + seperator + start + additionalSeperator))
         {
            std::string next = searchString.substr(end + start + 2);
            if (searchString.find(',') != std::string::npos || searchString.find(';') != std::string::npos)
               next = searchString.substr(end + start + 3);
            searchString = next;
         }

         attrs.insert({name, value});
         _id++;
         done_attrs++;
         parsed_attrs.push_back(name);
      }
   }

   return attrs;
}

int parser::get_height(std::string _value)
{
   int start = _value.find("height") + 7;
   std::string searchAR = _value.substr(start);
   std::string value = _value.substr(start, searchAR.find(';') - 2);

   try
   {
      return std::stoi(value);
   }
   catch (const std::exception &e)
   {
      return 0;
   }
}

int parser::get_width(std::string _value)
{
   int start = _value.find("width") + 7;
   std::string searchAR = _value.substr(start);
   std::string value = _value.substr(start, searchAR.find(';') - 2);

   try
   {
      return std::stoi(value);
   }
   catch (const std::exception &e)
   {
      return 0;
   }
}

void parser::get_dimensions(std::map<std::string, std::string> attrs, int &height, int &width)
{
   for (const auto attr : attrs)
   {
      std::string attrName = attr.first;
      std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);

      if (attrName == "style")
      {
         if (attr.second.find("height") != std::string::npos)
            height = get_height(attr.second);

         if (attr.second.find("width") != std::string::npos)
            width = get_width(attr.second);

         break;
      }
   }
}

void parser::fetch_starting_tag(std::string line, int index, globals &global)
{
   // its a starting tag
   std::string tagname;
   std::string restoftheline = line.substr(index + 1);
   size_t end_tag = restoftheline.find('>');
   std::map<std::string, std::string> attrs;

   // int width = 0;
   // int height = 0;
   size_t findAttrInit = restoftheline.find('=');

   if (findAttrInit != std::string::npos)
   {
      if (findAttrInit < end_tag)
      {
         // tag has attrs
         tagname = restoftheline.substr(0, restoftheline.find(' '));

         // since the tag has attributes we will also need to process them
         int start = restoftheline.find(' ') + 1;
         attrs = fetch_attr(restoftheline, start);
         global._id++;
         // get_dimensions(attrs, height, width);
      }

      if (line.find('>') == std::string::npos)
      {

         global.on_next_line = 'D';
         if (global.openTags.empty()) { insert_node(0, tagname, attrs, global); }
         else { insert_node(global.openTags.back().id + 1, tagname, attrs, global); }

         return;
      }
   }
   else
   {
      // the tag dose not end in the line it starts
      if (end_tag == std::string::npos)
      {
         tagname = restoftheline.substr(0, restoftheline.find(' '));
         global.on_next_line = 'D';

         if (global.openTags.empty()) { insert_node(0, tagname, attrs, global); }
         else { insert_node(global.openTags.back().id + 1, tagname, attrs, global); }

         return;
      }
      else
         tagname = restoftheline.substr(0, end_tag);
   }

   unsigned int id;

   if (global.openTags.empty()) id = global.dom->createNode(0, "", attrs);
   else id = global.dom->insertNode({dom::_type::_node, global.openTags.back().id, {}, {}, attrs,
    0, // <- this ####################
     ""});
   if (tagname != "img") {
      if (!global.openTags.empty()) 
         global.openTags.push_back({id, global.openTags.back().id, tagname, attrs});
      else global.openTags.push_back({id, 0, tagname, attrs});
   }

   global.elements.insert({id, tagname});
}

void parser::fetch_endtag(std::string search_string, globals &global)
{
   size_t end;
   if (search_string.find('>') != std::string::npos)
      end = search_string.find('>');
   else
      end = 0;

   std::string endtag = search_string.substr(1, end - 1);
   global.openTags.pop_back();
}

void parser::fetch_data(std::string search_string, globals &global, bool recurse)
{
   size_t dataStartAR = search_string.find('>') + 1;
   std::string dataSearch = search_string.substr(dataStartAR);
   std::string data;
   if (dataSearch.find('<') == std::string::npos)
      data = dataSearch.substr(0);
   else
      data = dataSearch.substr(0, dataSearch.find('<'));

   if (validate_data(data) != -1)
   {
      if (global.current_line > global.data_parsed)
      {
         global.data_parsed = global.current_line;

         if (global.openTags.empty())
            global.dom->crateTextNode(0, data);
         else
            global.dom->crateTextNode(global.openTags.back().id, data);

         if (dataSearch.find('<') != std::string::npos)
         {
            bool stat1 = search_string[dataSearch.find('<') + dataStartAR + 1];
            bool stat3 = search_string[dataSearch.find('<') + dataStartAR + 1] != '/';
            if (stat1 && (stat3))
            {
               fetch_data(search_string.substr(dataSearch.find('<') + dataStartAR + 1), global, true);
            }
         }
      }

      else if (recurse)
      {
         if (dataSearch.find('<') != std::string::npos)
         {
            bool stat1 = search_string[dataSearch.find('<') + dataStartAR + 1];
            bool stat3 = search_string[dataSearch.find('<') + dataStartAR + 1] != '/';
            if (stat1 && (stat3))
               fetch_data(search_string.substr(dataSearch.find('<') + dataStartAR + 1), global, true);
         }
      }
   }
}

void parser::fetch_line(std::string line, globals &global)
{

   if (validate_data(line) != -1 && !global.pending_nodes.empty())
   {
      if (global.on_next_line == 'D')
      {
         std::map<std::string, std::string> attrs = fetch_attr(line.substr(validate_data(line)), 0);
         for (auto attr : attrs)
            global.pending_nodes.back().attributes.insert({attr.first, attr.second});

         if (line.find('>') != std::string::npos)
         {
            // dom::nodeInternal node = {dom::_type::_node, global.pending_nodes.back().parent, {},
            // {}, global.pending_nodes.back().attributes, };
            global.on_next_line = 'A';

            unsigned int id = global.dom->createNode(global.pending_nodes.back().parent, global.pending_nodes.back().tag,
            global.pending_nodes.back().attributes);

            global.elements.insert({id, global.openTags.back().tag});
            global.pending_nodes.pop_back();
         }
      }
   }

   long unsigned int last_starting_tag = 0;
   for (long unsigned int found = 0; line[found]; found++)
   {
      bool startingtag = (line[found] == '<' && line[found + 1] != '/');
      bool endtag = line[found] == '<';

      if (startingtag)
      {
         fetch_starting_tag(line, found, global);
         last_starting_tag = found;
      }
      else if (endtag)
         fetch_endtag(line.substr(found), global);
      else
      {
         if (last_starting_tag < found && found > line.find(">")) // data
            fetch_data(line.substr(found), global, false);
      }
   }

   global.current_line++;
}

std::tuple<std::shared_ptr<_dom>, std::map<unsigned int, std::string>> parser::parse(std::string path)
{
   globals global;
   global.current_line = 0;
   global.data_parsed = -1;
   global.dom = std::make_shared<_dom>();
   std::string line;
   std::ifstream htmlFile(path);
   if (htmlFile.is_open())
   {
      std::string data;

      while (std::getline(htmlFile, line))
         fetch_line(line, global);
   }

   return {global.dom,global.elements};
}

void parser::insert_node (unsigned int id, std::string tagname, 
std::map<std::string, std::string> attrs,
globals &global )
{
   if (global.openTags.empty())
   {
      node insert = {id, 0, tagname, attrs};
      global.openTags.push_back(insert);
      global.pending_nodes.push_back(insert);
   }

   else
   {
      node insert = {id, global.openTags.back().id, tagname, attrs};
      global.openTags.push_back(insert);
      global.pending_nodes.push_back(insert);
   }
}