#include "renderer/program.hpp"
#include "glHelper.hpp"
#include <stdexcept>
#include <fstream>
#include <array>
#include <cassert>
#include <tuple>

using namespace photon::renderer;

static std::tuple<std::vector<std::string>,std::vector<std::string>> parseShaderMake(const std::string& path);

program::program(const std::string& shaderMakeFilePath)
{
   auto [inputs , paths] = parseShaderMake(shaderMakeFilePath);

   std::vector<std::string> vsSrc;
   std::vector<std::string> fsSrc;

   std::string basePath = shaderMakeFilePath.substr(0,shaderMakeFilePath.rfind("/")+1);

   for(auto& i : paths)
   {
      
      std::ifstream file(basePath+i);
      if(!file)
         throw std::invalid_argument("file not found or failed to open");
   
      file.seekg(0,file.end);
      size_t size = file.tellg();
      file.seekg(0,file.beg);

      std::string temp;
      temp.resize(size);
      file.read(&temp[0],size);
      if(i.rfind(".vert") != std::string::npos)
         vsSrc.push_back(temp);
      else if(i.rfind(".frag") != std::string::npos)
         fsSrc.push_back(temp);
      else
         throw std::invalid_argument("unkown file extension");

   }
   addShader(vsSrc,GL_VERTEX_SHADER);
   addShader(fsSrc,GL_FRAGMENT_SHADER);
   id = GLC(glCreateProgram());

   if(inputs.size() > GL_MAX_VERTEX_ATTRIBS)
      throw std::out_of_range("you are crazy");

   for(size_t i = 0; i < inputs.size(); i++)
   {
      GLC(glBindAttribLocation(id,i,inputs[i].c_str()));
   }

   for(auto& i : shaderIds)
   {
      GLC(glAttachShader(id,i));
   }

   GLC(glLinkProgram(id));

#ifdef DEBUG
   GLint success;
   GLC(glGetProgramiv(id,GL_LINK_STATUS,&success));
   if(!success)
   {
      int length;
      GLC(glGetProgramiv(id,GL_INFO_LOG_LENGTH,&length));
      std::string logs;
      logs.resize(length);
      GLC(glGetProgramInfoLog(id,length,nullptr,&logs[0]));
      throw std::logic_error(logs);
   }
#endif
}

program::~program()
{
   for(auto& i : shaderIds)
   {
      GLC(glDetachShader(id,i));
      GLC(glDeleteShader(i));
   }
   GLC(glDeleteProgram(id));
}

void program::bind() const
{
   GLC(glUseProgram(id));
}

void program::unbind() const
{
   GLC(glUseProgram(0));
}

void program::addShader(const std::vector<std::string>& source,GLint type)
{
   GLint shaderId = GLC(glCreateShader(type));
   size_t srcCount = source.size();
   GLint* srcSize = reinterpret_cast<GLint*>(alloca(srcCount*sizeof(GLint)));
   const GLchar**  srcStrings = reinterpret_cast<const GLchar**>(alloca(srcCount*sizeof(GLchar)));
   for(size_t i = 0; i < srcCount;i++)
   {
      srcSize[i] = source[i].length();
      srcStrings[i] = source[i].c_str();
   }
   GLC(glShaderSource(shaderId,srcCount,srcStrings,srcSize));
   GLC(glCompileShader(shaderId));

#ifdef DEBUG
   GLint success;
   GLC(glGetShaderiv(shaderId,GL_COMPILE_STATUS,&success));
   if(success == GL_FALSE)
   {
      int length;
      GLC(glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length));
      std::string logs;
      logs.resize(length);
      GLC(glGetShaderInfoLog(id,length,nullptr,&logs[0]));
      throw std::logic_error(logs);
   }
#endif
   shaderIds.push_back(shaderId);
}

static std::tuple<std::vector<std::string>,std::vector<std::string>> parseShaderMake(const std::string& path)
{
   std::fstream file(path);

   if(!file)
      throw std::invalid_argument("file not found");

   std::vector<std::string> inputs;
   std::vector<std::string> paths;

   std::string line;
   while (std::getline(file,line))
   {
      size_t firstSpaceLoc = line.find(" ");
      std::string type = line.substr(0, firstSpaceLoc);

      line.erase(0,firstSpaceLoc+1);
      if(type == "input")
         inputs.push_back(line);
      else if(type == "file")
         paths.push_back(line);
      else
         throw std::logic_error("unkown type");
   }

   return {inputs, paths};
}