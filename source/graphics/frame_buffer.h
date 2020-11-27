#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <vector>

#include "texture.h"
#include "../gl_includes.h"

class FrameBuffer
{

  public:

    const GLuint id, width, height, samples;

    SharedTexture colorTexture;
    std::vector<SharedTexture> colorTextures;
    SharedTexture depthTexture;

    /**
     * Constructor for a (multisampled) FrameBuffer.
     * NOTE: multisampling is not supported in WebGL, samples will automatically be set to 0.
     */
    FrameBuffer(GLuint width, GLuint height, GLuint samples=0);

    ~FrameBuffer();

    void bind();

    void unbind();

    // format can be GL_RGB or GL_RGBA. magFilter and minFilter can be GL_LINEAR for example.
    void addColorTexture(GLuint format, GLuint magFilter, GLuint minFilter);

    void addColorTexture(GLuint internalFormat, GLuint format, GLuint magFilter, GLuint minFilter);

    void addColorBuffer(GLuint format);

    void addDepthTexture(GLuint magFilter, GLuint minFilter);

    void addDepthBuffer();

    void setDrawBuffers();

    // function to get the color pixels. Note: it binds the FrameBuffer
    void bindAndGetPixels(GLenum format, std::vector<GLubyte> &out, unsigned int outOffset);

    float getPixelDepth(int x, int y);

  private:
    FrameBuffer *sampled = NULL;

    static void unbindCurrent();

};

#endif
