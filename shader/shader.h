//
//  shader.h
//  flyEngine
//
//  Created by joe on 10/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "glRef.h"

NS_FLYENGINE_BEGIN

class shader : public glRef{
    private:
        unsigned int _idProgram=0;
        char* _szVert=NULL;
        char* _szFrag=NULL;
        char* _szGeo=NULL;
        char* _szTessControl=NULL;
        char* _szTessEval=NULL;
        char* _szVertFileName=NULL;
        char* _szFragFileName=NULL;
        char* _szGeometryFileName=NULL;
        char* _szTessControlFileName=NULL;
        char* _szTessEvalFileName=NULL;
        
    public:
        shader(const char* szVertFileName,const char* szFragFileName,const char* szGeometry=NULL,
               const char* szTessControl=NULL,const char* szTessEval=NULL);
        ~shader();
        bool isSuccess();
        void use();
        void setBool(const char* name,bool v,bool debug=false);
        void setBool(std::string name,bool v,bool debug=false);
        void setInt(const char* name,int v,bool debug=false);
        void setInt(std::string name,int v,bool debug=false);
        void setFloat(const char* name,float v,bool debug=false);
        void setFloat(std::string name,float v,bool debug=false);
        void setMat4(const char* name,float* v,bool debug=false);
        void setMat4(const char* name,glm::mat4 v,bool debug=false);
        void setMat4(std::string name,glm::mat4 v,bool debug=false);
        void setMat4Multi(const char *name, float* v,int count,bool debug=false);
    
        void setVec2(const char* name,float* v,bool debug=false);
        void setVec2(const char* name,float v1,float v2,bool debug=false);
        void setVec2(const char* name,glm::vec2 vector2,bool debug=false);
        void setVec2(std::string name,glm::vec2 vector2,bool debug=false);
    
        void setVec3(const char* name,float* v,bool debug=false);
        void setVec3(const char* name,float v1,float v2,float v3,bool debug=false);
        void setVec3(const char* name,glm::vec3 vector3,bool debug=false);
        void setVec3(std::string name,glm::vec3 vector3,bool debug=false);
    
        void setVec4(const char* name,float* v,bool debug=false);
        void setVec4(const char* name,float v1,float v2,float v3,float v4,bool debug=false);
        void setVec4(const char* name,glm::vec4 vector4,bool debug=false);
        void setVec4(std::string name,glm::vec4 vector4,bool debug=false);
    
        bool bindGeometry(const char* szGeometry);//support on opengl 3.2+
        
        float getFloat(const char* name,bool debug=false);
        bool getBool(const char* name,bool debug=false);
        unsigned int getProgramID(){return _idProgram;};
        
        void compile();
        bool readFile();
};

NS_FLYENGINE_END


#endif /* shader_h */
