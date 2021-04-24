#include "photon/parser.hpp"

using namespace photon;

std::vector<std::string> cssparser::fetch_selectors(std::string selector_string)
{
    std::vector<std::string> selectors;
    for (int i=0; selector_string[i]; i++)
    {
        if (selector_string[i] == ' ' && selector_string[i + 1] == '+')
            selector_string.replace(i, 1, "");
        
        if (selector_string[i] == ' ' && i > 0)
        {
            if (selector_string[i - 1] == '+') selector_string.replace(i, 1, "");
        }
    }

    size_t not_found = -1;
    while (validate_file(selector_string) != not_found)
    {
        std::string selector = selector_string.substr(0, selector_string.find(' '));
        selector_string.replace(0, selector_string.find(' '), "");
        if (validate_file(selector_string) != -1) 
            selector_string.replace(0, validate_file(selector_string), "");
        selectors.push_back(selector);
    }
    return selectors;
}

std::map<std::string, std::string> cssparser::props(std::string content)
{

    //* THIS FUNCTION IS HERE TO RETURN ALL THE PROPS WITH THEIR REPESTIVE VALUES AS A MAP WITH ID AND VALUE BOTH OF TYPE STRING

    std::map<std::string, std::string> result;
    size_t not_found = -1;
    while (validate_file(content) != not_found)
    {
        if (validate_file(content) != not_found) content.replace(0, validate_file(content), "");
        std::string prop = content.substr(0, content.find(':'));
        std::string value = content.substr(content.find(':') + 1, content.find(';') - content.find(':') - 1);
        value.replace(0, validate_file(value), "");
        result.insert({prop, value});
        if (validate_file(content) != not_found)
        {
            content.replace(0, content.find(';'), "");
        }
    }

    return result;
}

size_t cssparser::validate_file(std::string file)
{

    /*
    * THIS FUNCTION IS HERE TO RETURN THE INDEX OF THE FIRST CHARACTER IN THE STRING
    * IF THERE IS NO CHARACTER, IT MEANS THAT THE STRING IS EMPTY OR IT HAS JUST SPACES
    * IN THAT CASE THE FUNCTION WILL RETURN -1.
    */

    std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                 'u', 'v', 'w', 'x', 'y', 'z'};

    std::vector<char> ints = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; file[i]; i++)
    {
        if (std::find(letters.begin(), letters.end(), file[i]) != letters.end() ||
            std::find(ints.begin(), ints.end(), file[i]) != ints.end())
            return i;
    }
    return -1;
}

void cssparser::parse(std::string path)
{
    std::string line;
    std::ifstream cssfile(path);
    if (!cssfile.is_open())
        throw std::invalid_argument("uinable to open file");

    cssfile.seekg(0, cssfile.end);
    size_t size = cssfile.tellg();
    cssfile.seekg(0, cssfile.beg);

    std::string file;
    file.resize(size);
    cssfile.read(&file[0], size);

    size_t not_found = -1;
    while (validate_file(file) != not_found)
    {
        size_t startingPoint = validate_file(file);

        // Remove the indentation of the file
        file.replace(0, startingPoint, "");

        std::string selector = file.substr(0, file.find('{'));
        for (int i = 0; selector[i]; i++)
        {
            if (selector[i] == '\n')
                selector.replace(i, 1, "");
        }

        fetch_selectors(selector);

        size_t content_start = file.find('{');
        size_t content_end = file.find('}');

        //if (file[file.find('}') + 2]) content_end -= 7;

        std::string content = file.substr(content_start + 1, content_end - content_start -1);
        std::map<std::string, std::string> prop = props(content);
        file.replace(0, file.find('}'), "");
    }
}