#ifndef RAYMARCHER_HELPERS_HPP
#define RAYMARCHER_HELPERS_HPP

#include <vector>

#include <GL/gl.h>
#include <geGL/geGL.h>

static void
fillTexture(std::shared_ptr<ge::gl::Texture>& tex, size_t width, size_t height)
{
  std::vector<unsigned char> textureData;
  textureData.reserve(width * height * 4);

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      float uvX = (x / float(width)) - 0.5f;
      float uvY = (y / float(height)) - 0.5f;
      float dist = uvX * uvX + uvY * uvY;
      unsigned char color = static_cast<unsigned char>(100 * dist * 255.0);

      textureData[4 * (y * width + x) + 0] = 0;
      textureData[4 * (y * width + x) + 1] = 0;
      textureData[4 * (y * width + x) + 2] = color;
      textureData[4 * (y * width + x) + 3] = 0xFF;
    }
  }

  tex->setData2D(textureData.data());
}

struct FullscreenQuad
{
public:
  FullscreenQuad() { Initialize(); }

  void draw()
  {
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.unbind();
  }

private:
  void Initialize()
  {
    static float vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                                -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
                                1.0f,  1.0f,  0.0f, 1.0f, 1.0f,

                                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                                1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                1.0f,  1.0f,  0.0f, 1.0f, 1.0f,

                                0.0f,  0.0f,  0.0f, 0.0f };

    vbo.alloc(sizeof(float) * 5 * 6);
    vbo.setData(vertices, sizeof(float) * 5 * 6);

    vao.bind();
    vbo.bind(GL_ARRAY_BUFFER);
    /* // X,Y,Z */
    vao.addAttrib(&vbo, 0, 3, GL_FLOAT, 5 * sizeof(float));
    /* // UV */
    vao.addAttrib(&vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), sizeof(float) * 3);
    vao.unbind();
    vbo.unbind(GL_ARRAY_BUFFER);
  }
  ge::gl::Buffer vbo;
  ge::gl::VertexArray vao;
};

#endif
