#include "render.h"

#include <string.h>

static const GLfloat g_Vertices[]    = {-1.0f, 1.0f,  0.0f, -1.0f,
                                        -1.0f, 0.0f,  1.0f,  1.0f,
                                         0.0f, 1.0f, -1.0f,  0.0f
                                       };

static const GLfloat g_TexRot_0[]    = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
static const GLfloat g_TexRot_90[]   = {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
static const GLfloat g_TexRot_180[]  = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
static const GLfloat g_TexRot_270[]  = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

static const GLfloat g_TexMirror[]   = {1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};                                    
                                    
Render::Render()
{
    nv21Data_          = 0;
    width_             = 0;
    height_            = 0;
    mirror_            = false;
    memset(textureID_, 0, sizeof(textureID_));
    program_           = 0;
    positionHandle_    = 0;
    texCoordHandle_    = 0;
    texYHandle_        = 0;
    texUVHandle_       = 0;
    
    memcpy(vertices_, g_Vertices, sizeof(g_Vertices));
    memcpy(texCoord_, g_TexRot_270, sizeof(g_TexRot_270));
    memcpy(texCoordMirror_, g_TexMirror, sizeof(g_TexMirror));

    vertexShader_      = 
                         "attribute vec4 aPosition;\n"
                         "attribute vec2 aTexCoord;\n"
                         "varying vec2 textureCoordinate;\n"
                         "void main() {\n"
                         "	gl_Position = aPosition;\n"
                         "	textureCoordinate = aTexCoord;\n"
                         "}\n";
                         
    fragmentShader_    =
                         "precision mediump float;\n"
                          "varying vec2 textureCoordinate;\n"
                          "uniform sampler2D uTextureY;\n"
                          "uniform sampler2D uTextureUV;\n"
                          "void main() {\n"
                          "	vec4 y = vec4((texture2D(uTextureY, textureCoordinate).r - 16./255.) * 1.164);\n"
                          "  	vec4 u = vec4(texture2D(uTextureUV, textureCoordinate).a - 128./255.);\n"
                          "  	vec4 v = vec4(texture2D(uTextureUV, textureCoordinate).r - 128./255.);\n"
                          "  	y += v * vec4(1.596, -0.813, 0, 0);\n"
                          "  	y += u * vec4(0, -0.392, 2.017, 0);\n"
                          "  	y.a = 1.0;\n"
                          "  	gl_FragColor = y;\n"
                          "}\n";
}

void Render::init(int width, int height)
{
    width_  = width;
    height_ = height;
}

void Render::unit(void)
{
    //Note that when the EGL context is lost, all OpenGL resources associated with that context will be automatically deleted. 
    //You do not need to call the corresponding "glDelete" methods such as glDeleteTextures to manually delete these lost resources.
    releaseTexture();
}

void Render::onSurfaceCreated(void)
{
    glClearColor(0, 0, 0, 1);
    initTexture();
	initShader();
}

void Render::onSurfaceChanged(int surfaceWidth, int surfaceHeight)
{
    glViewport(0, 0, surfaceWidth, surfaceHeight);
}

void Render::updateTextureFrame(unsigned char* data)
{
    nv21Data_ = data;
}

void Render::onDrawFrame(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTexture();
}

void Render::setMirror(bool mirror)
{
    mirror_ = mirror;
}

void Render::setSize(int width, int height)
{
    width_  = width;
    height_ = height;
}

void Render::setRotation(TextureOrient rotation)
{
    switch(rotation)
    {
        case TEXTURE_ROTATED_0:
            memcpy(texCoord_, g_TexRot_0, sizeof(g_TexRot_0));
            break;
        case TEXTURE_ROTATED_90:
            memcpy(texCoord_, g_TexRot_90, sizeof(g_TexRot_90));
            break;
        case TEXTURE_ROTATED_180:
            memcpy(texCoord_, g_TexRot_180, sizeof(g_TexRot_180));
            break;
        case TEXTURE_ROTATED_270:
            memcpy(texCoord_, g_TexRot_270, sizeof(g_TexRot_270));
            break;
        default:
            break;
    }
}

void Render::initTexture(void)
{
    glGenTextures(sizeof(textureID_)/sizeof(textureID_[0]), textureID_);
	
    for(int i = 0; i < sizeof(textureID_)/sizeof(textureID_[0]); i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureID_[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    }
}

void Render::initShader(void)
{
    program_        = createProgram();
	positionHandle_ = glGetAttribLocation(program_, "aPosition");
	texCoordHandle_ = glGetAttribLocation(program_, "aTexCoord");
	texYHandle_     = glGetUniformLocation(program_, "uTextureY");
	texUVHandle_    = glGetUniformLocation(program_, "uTextureUV");
}

void Render::releaseTexture(void)
{
    glDeleteTextures(sizeof(textureID_)/sizeof(textureID_[0]), textureID_);   
}

void Render::drawTexture(void)
{     
	if(nv21Data_ == 0 || width_ == 0 || height_ == 0)
    {
		return;
	}

	glUseProgram(program_);

	glVertexAttribPointer(positionHandle_, 3, GL_FLOAT, GL_FALSE, 0, vertices_);    
	glEnableVertexAttribArray(positionHandle_);
	glVertexAttribPointer(texCoordHandle_, 2, GL_FLOAT, GL_FALSE, 0, mirror_ ? texCoordMirror_ : texCoord_);
	glEnableVertexAttribArray(texCoordHandle_);
    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID_[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width_, height_, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nv21Data_);
	glUniform1i(texYHandle_, 0);
    
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID_[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width_>>1, height_>>1, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, nv21Data_+width_*height_);
	glUniform1i(texUVHandle_, 1);
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glDisableVertexAttribArray(positionHandle_);
	glDisableVertexAttribArray(texCoordHandle_);
    
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Render::createProgram(void)
{
    GLuint	vertexShader;
	GLuint	fragmentShader;
	GLint	linked;

	vertexShader   = loadShader(GL_VERTEX_SHADER, vertexShader_);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShader_);

	GLuint program = glCreateProgram();
	if(program == 0)
	{
		LOGE("create program failed");
		
        return 0;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram (program);

	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if(linked != GL_TRUE)
	{
		LOGE("link failed");
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			LOGE("Error linking program: %s", infoLog);

			free(infoLog);
		}

		glDeleteProgram(program);
		program = 0;
	}
	
	return program;
}

GLuint Render::loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if(shader) 
	{
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 1;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if(compiled == 0) 
		{
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if(infoLen) 
			{
                char* buf = (char*)malloc(infoLen);
                if(buf)
				{
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d: %s", shaderType, buf);
                    
                    free(buf);
                }
            }
			
			glDeleteShader(shader);
			shader = 0;
        }
    }
    
    return shader;
}
























