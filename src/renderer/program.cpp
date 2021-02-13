#include "renderer/program.hpp"
#include "glHelper.hpp"
#include <stdexcept>
#include <fstream>
#include <array>

using namespace photon::renderer;

program::program(const std::vector<std::string>& sources)
{
   std::vector<std::string> vsSrc;
   std::vector<std::string> fsSrc;
   for(auto& i : sources)
   {
      
      std::ifstream file(i);
      if(!file)
         throw std::invalid_argument("file not found");
   
      file.seekg(0,file.end);
      size_t size = file.tellg();
      file.seekg(0,file.beg);

      std::string temp;
      temp.resize(size);
      file.read(&temp[0],size);
      if(i.find(".vs."))
         vsSrc.push_back(temp);
      else if(i.find(".fs."))
         fsSrc.push_back(temp);

   }
   addShader(vsSrc,GL_VERTEX_SHADER);
   addShader(fsSrc,GL_FRAGMENT_SHADER);
   id = GLC(glCreateProgram());
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

/*    GLC(glValidateProgram(id));
   GLC(glGetProgramiv(id,GL_VALIDATE_STATUS,&success));
   if(!success)
   {
      int length;
      GLC(glGetProgramiv(id,GL_INFO_LOG_LENGTH,&length));
      std::string logs;
      logs.resize(length);
      GLC(glGetProgramInfoLog(id,length,nullptr,&logs[0]));
      throw std::logic_error(logs);
   } */
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

void program::addShader(const std::vector<std::string>& source,GLenum type)
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
   GLC(glGetShaderiv(id,GL_COMPILE_STATUS,&success));
   if(!success)
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