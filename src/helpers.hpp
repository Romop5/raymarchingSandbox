#include <geGL/geGL.h>
#include <vector>

void fillTexture(std::shared_ptr<ge::gl::Texture>&  tex, size_t width, size_t height)
{
    std::vector<unsigned char> textureData;
    textureData.reserve(width*height*4);

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            float uvX = (x/float(width))-0.5f;
            float uvY = (y/float(height))-0.5f;
            float dist = uvX*uvX+uvY*uvY;
            unsigned char color = static_cast<unsigned char>(100*dist*255.0);

            textureData[4*(y*width+x)+0] = 0;
            textureData[4*(y*width+x)+1] = 0;
            textureData[4*(y*width+x)+2] = color;
            textureData[4*(y*width+x)+3] = 0xFF;
        }
    }

    tex->setData2D(textureData.data());
}

struct FullscreenQuad
{
    std::shared_ptr<ge::gl::VertexArray> vao;
    std::shared_ptr<ge::gl::Buffer> vbo;
    FullscreenQuad()
    {
        float vertices[] = {
             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        vbo = std::make_shared<ge::gl::Buffer>();
        vbo->alloc(sizeof(float)*5*6);
        vbo->setData(vertices, sizeof(float)*5*6);

        vao = std::make_shared<ge::gl::VertexArray>();
        vao->bind();
        // X,Y,Z
        vao->addAttrib(vbo, 0, 3, GL_FLOAT,5*sizeof(float));
        // UV
        vao->addAttrib(vbo, 1, 2, GL_FLOAT,5*sizeof(float), sizeof(float)*3);
        vao->unbind();
        vbo->unbind(GL_ARRAY_BUFFER);
    }
};
