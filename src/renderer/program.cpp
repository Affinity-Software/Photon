#include "renderer/program.hpp"
#include "glHelper.hpp"
#include <stdexcept>
#include <fstream>
#include <array>
#include <cassert>
#include <tuple>
#include <iostream>

using namespace photon::renderer;

static std::tuple<std::vector<std::string>,std::vector<std::string>,std::vector<std::string>>
parseShaderMake(const std::string& path);

program::program(const std::string& shaderMakeFilePath)
{
	auto [inputs , paths, uniforms] = parseShaderMake(shaderMakeFilePath);

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

	for(auto& i : shaderIds)
		{GLC(glAttachShader(id,i))};

	if(inputs.size() > GL_MAX_VERTEX_ATTRIBS)
		throw std::out_of_range("to many vertex atribs");

	for(size_t i = 0; i < inputs.size(); i++)
	{
		GLC(glBindAttribLocation(id,i,inputs[i].c_str()))
	};

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

	for(auto& i : uniforms)
		uniformNames[i] = GLC(glGetUniformLocation(id,i.c_str()));
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
	const GLchar**  srcStrings = reinterpret_cast<const GLchar**>(alloca(srcCount*sizeof(GLchar)));
	for(size_t i = 0; i < srcCount;i++)
		srcStrings[i] = source[i].c_str();
	GLC(glShaderSource(shaderId,srcCount,srcStrings,NULL));
	GLC(glCompileShader(shaderId));

#ifdef DEBUG
	GLint success;
	GLC(glGetShaderiv(shaderId,GL_COMPILE_STATUS,&success));
	if(success == GL_FALSE)
	{
		int length;
		GLC(glGetShaderiv(shaderId,GL_INFO_LOG_LENGTH,&length));
		std::string logs;
		logs.resize(length);
		GLC(glGetShaderInfoLog(shaderId,length,nullptr,&logs[0]));
		throw std::logic_error(logs);
	}
#endif
	shaderIds.push_back(shaderId);
}

static std::tuple<std::vector<std::string>,std::vector<std::string>,std::vector<std::string>>
parseShaderMake(const std::string& path)
{
	std::fstream file(path);

	if(!file)
		throw std::invalid_argument("file not found");

	std::vector<std::string> inputs;
	std::vector<std::string> paths;
	std::vector<std::string> uniforms;

	std::string line;
	size_t lineCount = 0;
	while (std::getline(file,line))
	{
		size_t firstSpaceLoc = line.find(" ");
		std::string type = line.substr(0, firstSpaceLoc);

		line.erase(0,firstSpaceLoc+1);
		if(type == "input")
		{
			int loc = line.find("[");
			if(loc != std::string::npos)
			{
				std::string name = line.substr(0,loc);
				int lenght;
				try
				{
					lenght = std::stoi(line.substr(loc+1,line.find("]")-loc-1));
				}
				catch(std::exception)
				{
					std::cerr << "[ERROR] [glShaderMake] invalid array size value on line " << lineCount << "in file" << path << std::endl;
					std::exit(1);
				}
				
				for(int i = 0; i < lenght; i++)
					inputs.push_back(name + std::to_string(i));
			}
			else
				inputs.push_back(line);

		}
		else if(type == "file")
			paths.push_back(line);
		else if(type == "uniform")
			uniforms.push_back(line);
		else
		{
			std::cerr << "[ERROR] [glShaderMake] unkown type on line " << lineCount << "in file" << path << std::endl;
			std::exit(1);
		}

		lineCount++;
	}

	return {inputs, paths, uniforms};
}

void program::setUniform(std::string name,glm::fvec1 value){bind();GLC(glUniform1f(uniformNames[name],value.x));}
void program::setUniform(std::string name,glm::fvec2 value){bind();GLC(glUniform2f(uniformNames[name],value.x,value.y));}
void program::setUniform(std::string name,glm::fvec3 value){bind();GLC(glUniform3f(uniformNames[name],value.x,value.y,value.z));}
void program::setUniform(std::string name,glm::fvec4 value){bind();GLC(glUniform4f(uniformNames[name],value.x,value.y,value.z,value.w));}
void program::setUniform(std::string name,glm::ivec1 value){bind();GLC(glUniform1i(uniformNames[name],value.x));}
void program::setUniform(std::string name,glm::ivec2 value){bind();GLC(glUniform2i(uniformNames[name],value.x,value.y));}
void program::setUniform(std::string name,glm::ivec3 value){bind();GLC(glUniform3i(uniformNames[name],value.x,value.y,value.z));}
void program::setUniform(std::string name,glm::ivec4 value){bind();GLC(glUniform4i(uniformNames[name],value.x,value.y,value.z,value.w));}
void program::setUniform(std::string name,glm::mat2 value){bind();GLC(glUniformMatrix2fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat3 value){bind();GLC(glUniformMatrix3fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat4 value){bind();GLC(glUniformMatrix4fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat2x3 value){bind();GLC(glUniformMatrix2x3fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat3x2 value){bind();GLC(glUniformMatrix3x2fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat2x4 value){bind();GLC(glUniformMatrix2x4fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat4x2 value){bind();GLC(glUniformMatrix4x2fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat3x4 value){bind();GLC(glUniformMatrix3x4fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,glm::mat4x3 value){bind();GLC(glUniformMatrix4x3fv(uniformNames[name],1,GL_FALSE,&value[0][0]));}
void program::setUniform(std::string name,std::vector<glm::fvec1> value){bind();GLC(glUniform1fv(uniformNames[name],value.size(),(GLfloat*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::fvec2> value){bind();GLC(glUniform2fv(uniformNames[name],value.size(),(GLfloat*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::fvec3> value){bind();GLC(glUniform3fv(uniformNames[name],value.size(),(GLfloat*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::fvec4> value){bind();GLC(glUniform4fv(uniformNames[name],value.size(),(GLfloat*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::ivec1> value){bind();GLC(glUniform1iv(uniformNames[name],value.size(),(GLint*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::ivec2> value){bind();GLC(glUniform2iv(uniformNames[name],value.size(),(GLint*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::ivec3> value){bind();GLC(glUniform3iv(uniformNames[name],value.size(),(GLint*)value.data()));}
void program::setUniform(std::string name,std::vector<glm::ivec4> value){bind();GLC(glUniform4iv(uniformNames[name],value.size(),(GLint*)value.data()));}