#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define  LOG_TAG    "render"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

enum TextureOrient
{
	TEXTURE_ROTATED_0		= 0x1,
	TEXTURE_ROTATED_90		= 0x2,	
	TEXTURE_ROTATED_180		= 0x3,
	TEXTURE_ROTATED_270		= 0x4,
};

class Render
{
public:
    Render();
    void                init(int width, int height);
    void                unit(void);

    void                onSurfaceCreated(void);
    void                onSurfaceChanged(int surfaceWidth, int surfaceHeight);
    
    // assume nv21, should be valid after onDrawFrame
    void                updateTextureFrame(unsigned char* data);
    void                onDrawFrame(void);

    void                setMirror(bool mirror);
    void                setSize(int width, int height);
    void                setRotation(TextureOrient rotation);

private:        
    void                initTexture(void);
    void                initShader(void);
    void                releaseTexture(void);
    void                drawTexture(void);
    GLuint              createProgram(void);
    GLuint              loadShader(GLenum shaderType, const char* pSource);
    
private:
    unsigned char*      nv21Data_;
    int                 width_;
    int                 height_;
    bool                mirror_;
    GLuint              textureID_[2];
    GLuint              program_;
    GLuint              positionHandle_;
    GLuint              texCoordHandle_;
    GLuint              texYHandle_;
    GLuint              texUVHandle_;
    
    GLfloat             vertices_[12];
    
    GLfloat             texCoord_[8];
    GLfloat             texCoordMirror_[8];
    
private:
    char*               vertexShader_;
    char*               fragmentShader_;
};