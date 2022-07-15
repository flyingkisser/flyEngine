// Std. Includes
#include <iostream>
#include <map>
#include <string>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "dirUtil.h"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
// GL includes
#include "shader.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph

    int texID;
    int width;
    int height;
    int bearingX;
    int bearingY;
    float advance;
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;
int _vao=0;
int _vbo=0;
void RenderText(flyEngine::shader* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

int g1_screenWidth=0;
int g1_screenHigh=0;
int g1_winWidth=800;
int g1_winHigh=600;

// The MAIN function, from here we start our application and run the Game loop
int main2()
{
    char szHomeDir[1024]={0};
    dirUtil::getHomeDir(szHomeDir,sizeof(szHomeDir));
    strcat(szHomeDir,"/Documents/flyEngine/");
    dirUtil::setCurrentWorkDir(szHomeDir);
    printf("main:set current work dir %s\n",szHomeDir);
    
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"gladLoadGLLoader failed!"<<std::endl;
        glfwTerminate();
        return 0;
    }
    
    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // Set OpenGL options
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Compile and setup the shader
    flyEngine::shader* shader=new flyEngine::shader("res/shader/font_2d.vs", "res/shader/font.fs");
//    glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
    shader->use();
    shader->setMat4("matProj",glm::value_ptr(proj));
//    glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "matProj"), 1, GL_FALSE, glm::value_ptr(proj));
    
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    
    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "res/font/arial.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer
                     );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x,
            (int)texture,
            (int)face->glyph->bitmap.width,
            (int)face->glyph->bitmap.rows,
            (int)face->glyph->bitmap_left,
            (int)face->glyph->bitmap_top,
            (float)face->glyph->advance.x
        };

        // st.texID=texID;
        // st.width=_ftFace->glyph->bitmap.width;
        // st.height=_ftFace->glyph->bitmap.rows;
        // st.bearingX=_ftFace->glyph->bitmap_left;
        // st.bearingY=_ftFace->glyph->bitmap_top;
        // st.advance=_ftFace->glyph->advance.x;

        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    _vao=VAO;
    _vbo=VBO;
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check and call events
        glfwPollEvents();
        
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        // RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(255, 0.8f, 0.2f));
        RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 255, 0.9f));
        
        // Swap the buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

void RenderText(flyEngine::shader* shader, std::string text, GLfloat x, GLfloat y, GLfloat s, glm::vec3 color)
{

    // GLfloat scale=s;
    // shader->use();
    // glUniform3f(glGetUniformLocation(shader->getProgramID(), "textColor"), color.x/255, color.y/255, color.z/255);
    // glActiveTexture(GL_TEXTURE0);
    // glBindVertexArray(VAO);
   
    // std::string::const_iterator c;
    // for (c = text.begin(); c != text.end(); c++)
    // {
    //     Character ch = Characters[*c];
        
    //     GLfloat xpos = x + ch.Bearing.x * scale;
    //     GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
    //     GLfloat w = ch.Size.x * scale;
    //     GLfloat h = ch.Size.y * scale;
    //     // Update VBO for each character
    //     GLfloat vertices[6][4] = {
    //         { xpos,     ypos + h,   0.0, 0.0 },
    //         { xpos,     ypos,       0.0, 1.0 },
    //         { xpos + w, ypos,       1.0, 1.0 },
            
    //         { xpos,     ypos + h,   0.0, 0.0 },
    //         { xpos + w, ypos,       1.0, 1.0 },
    //         { xpos + w, ypos + h,   1.0, 0.0 }
    //     };
    //     // Render glyph texture over quad
    //     glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //     // Update content of VBO memory
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     // Render quad
    //     glDrawArrays(GL_TRIANGLES, 0, 6);
    //     // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    //     x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    // }
    // glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);

    int g_winWidth=800;
    int g_winHigh=600;
    shader->use();

    glm::mat4 proj=glm::ortho(0,g_winWidth,0,g_winHigh);
    // shader->setMat4("matProj", (float*)glm::value_ptr(proj));
    
    glViewport(0,0,g_winWidth,g_winHigh);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    color=glm::vec3(color.r/255,color.g/255,color.b/255);
    const char* _strText=text.c_str();
    int strLen=(int)strlen(_strText);
    glm::vec3 nodePos=glm::vec3(x,y,0);
    glm::vec3 pos=nodePos;
    glm::vec3 scale=glm::vec3(s,s,s);
    
    
    shader->setVec3("textColor", (float*)glm::value_ptr(color));
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);
    
   
    for(int i=0;i<strLen;i++){
        int k=(int)_strText[i];
        char c=_strText[i];
        Character st = Characters[c];
        // texFontStruct st=_fontObj->getTexStruct(k);
        float x=pos.x+st.bearingX*scale.x;
        float y=pos.y-(st.height-st.bearingY)*scale.y;
        float w=st.width*scale.x;
        float h=st.height*scale.y;
        float vertices[6][4]={
            {x,y+h,0,0},
            {x,y,0,1},
            {x+w,y,1,1},
            {x,y+h,0,0},
            {x+w,y,1,1},
            {x+w,y+h,1,0}
        };
        glBindTexture(GL_TEXTURE_2D,st.texID);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        
        glDrawArrays(GL_TRIANGLES,0,6);
        pos.x+=(st.advance/64)*scale.x;
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}
