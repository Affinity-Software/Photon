#include "photon/parser.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace photon;

bool parser::validate_data(std::string data)
{
   char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
   for (char i : letters)
   {
      if (data.find(i) != std::string::npos || data.find(std::toupper(i)) != std::string::npos)
         return true;
   }

   for (int i = 0; i < 10; i++)
   {
      char c = (char)i;
      if (data.find(c) != std::string::npos)
         return true;
   }

   return false;
}

std::vector<parser::attribute> parser::fetch_attr(std::string line, int end, int start, std::string tagname)
{

   std::vector<std::string> parsed_attrs;
   std::vector<attribute> attrs;

   std::string searchString = line.substr(start, end);
   int count_attrs = 0;
   int done_attrs = 0;

   for (int i = 0; searchString[i]; i++)
   {
      if (searchString[i] == '=')
      {
         count_attrs++;
      }
   }

   for (int i = 0; searchString[i]; i++)
   {
      if (done_attrs >= count_attrs)
      {
         break;
      }

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

         attribute attr(tagname, name, value);
         attrs.push_back(attr);
         done_attrs++;
         parsed_attrs.push_back(name);
      }
   }

   return attrs;
}

int parser::get_height(attribute attr)
{
   int start = attr.VALUE.find("height") + 8;
   std::string searchAR = attr.VALUE.substr(start);
   std::string value = attr.VALUE.substr(start, searchAR.find(';') - 2);

   try
   {
      return std::stoi(value);
   }

   catch (const std::exception &e)
   {
      return 0;
   }
}

int parser::get_width(attribute attr)
{
   int start = attr.VALUE.find("width") + 7;
   std::string searchAR = attr.VALUE.substr(start);
   std::string value = attr.VALUE.substr(start, searchAR.find(';') - 2);

   try
   {
      return std::stoi(value);
   }
   catch (const std::exception &e)
   {
      return 0;
   }
}

void parser::get_dimensions(std::vector<attribute> attrs, int &height, int &width)
{
   for (attribute attr : attrs)
   {
      std::string attrName = attr.NAME;
      std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);

      if (attrName == "style")
      {
         if (attr.VALUE.find("height") != std::string::npos)
            height = get_height(attr);

         if (attr.VALUE.find("width") != std::string::npos)
            width = get_width(attr);

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

   // *! the height and width variable are defined here
   int width = 0;
   int height = 0;
   size_t findAttrInit = restoftheline.find('=');

   if (findAttrInit != std::string::npos && end_tag != std::string::npos)
   {
      if (findAttrInit < end_tag)
      {
         // tag has attrs
         tagname = restoftheline.substr(0, restoftheline.find(' '));

         // since the tag has attributes we will also need to process them
         int start = restoftheline.find(' ') + 1;
         int end = restoftheline.find('>') - 3;
         std::vector<attribute> attrs = fetch_attr(restoftheline, end, start, tagname);
         get_dimensions(attrs, height, width);
      }
   }
   else
   {
      // the tag dose not end in the line it starts
      if (end_tag == std::string::npos)
         tagname = restoftheline.substr(0);
      else
         tagname = restoftheline.substr(0, end_tag);
   }

   global.openTags.push_back(tagname);
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

   if (validate_data(data))
   {
      if (global.current_line > global.data_parsed)
      {
         global.data_parsed = global.current_line;

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

void parser::fetch_line(std::string line, int dataEndPointAR, globals &global)
{
   for (int found = 0; line[found]; found++)
   {
      bool startingtag = (line[found] == '<' && line[found + 1] != '/');
      bool endtag = line[found] == '<';

      if (startingtag)
         fetch_starting_tag(line, found, global);
      else if (endtag)
         fetch_endtag(line.substr(found + 1), global);
      else // data
      {
         if (found > dataEndPointAR)
            fetch_data(line.substr(found + 1), global, false);
      }
   }

   global.current_line++;
}

void parser::parse(std::string path)
{
   globals global;
   global.current_line = 0;
   global.data_parsed = -1;
   std::string line;
   std::ifstream htmlFile(path);
   if (htmlFile.is_open())
   {
      int dataEndPointAR = 0;
      std::string data;

      while (std::getline(htmlFile, line))
         fetch_line(line, dataEndPointAR, global);
   }
}