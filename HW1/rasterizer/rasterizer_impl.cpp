#include <cstdint>

#include "image.hpp"
#include "loader.hpp"
#include "rasterizer.hpp"
#include <iostream>


bool _is_in_trig(const Triangle& trig, const float x, const float y) {
    const glm::vec3& A1  { trig.pos[0] };
    const glm::vec3& A2  { trig.pos[1] };
    const glm::vec3& A3  { trig.pos[2] };

    const glm::vec3 X { x, y, 0 };

    const auto S1 { glm::cross(X - A1, A2 - A1) };
    const auto S2 { glm::cross(X - A2, A3 - A2) };
    const auto S3 { glm::cross(X - A3, A1 - A3) };

    if (S1.z > 0 && S2.z > 0 && S3.z > 0) return true;
    if (S1.z < 0 && S2.z < 0 && S3.z < 0) return true;

    return false;
}
// TODO
void Rasterizer::DrawPixel(uint32_t x, uint32_t y, Triangle trig, AntiAliasConfig config, uint32_t spp, Image& image, Color color)
{
    // spp = samples per pixel
    if (config == AntiAliasConfig::NONE)            // if anti-aliasing is off
    {
        if (_is_in_trig(trig, x+0.5, y+0.5))         // if the pixel is inside the triangle
        {
            image.Set(x, y, color);
        }
    }
    else if (config == AntiAliasConfig::SSAA)       // if anti-aliasing is on
    {
        // grid supersampling
        // assumes spp is a perfect square
        int spp_sqrt = round(sqrt(spp));
        uint32_t count { 0 };
        double inc { 1.0 / spp_sqrt };

        for (auto i = 0; i < spp_sqrt; ++i)
        {
            for (auto j = 0; j < spp_sqrt; ++j)
            {
                if (_is_in_trig(trig, x + (i + 0.5) * inc, y + (j + 0.5) * inc))
                {
                    ++count;
                }
            }
        }
        image.Set(x, y, color * (float(count) / spp));
    }

    return;
}

// TODO
void Rasterizer::AddModel(MeshTransform transform, glm::mat4 rotation)
{
    /* model.push_back( model transformation constructed from translation, rotation and scale );*/
    return;
}

// TODO
void Rasterizer::SetView()
{
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = camera.lookAt;
    glm::vec3 cameraUp = camera.up;

    // TODO change this line to the correct view matrix
    this->view = glm::mat4(1.);

    return;
}

// TODO
void Rasterizer::SetProjection()
{
    const Camera& camera = this->loader.GetCamera();

    float nearClip = camera.nearClip;                   // near clipping distance, strictly positive
    float farClip = camera.farClip;                     // far clipping distance, strictly positive
    
    float width = camera.width;
    float height = camera.height;
    
    // TODO change this line to the correct projection matrix
    this->projection = glm::mat4(1.);

    return;
}

// TODO
void Rasterizer::SetScreenSpace()
{
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    // TODO change this line to the correct screenspace matrix
    this->screenspace = glm::mat4(1.);

    return;
}

// TODO
glm::vec3 Rasterizer::BarycentricCoordinate(glm::vec2 pos, Triangle trig)
{
    return glm::vec3();
}

// TODO
float Rasterizer::zBufferDefault = float();

// TODO
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{

    float result;
    ZBuffer.Set(x, y, result);

    return;
}

// TODO
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{

    Color result;
    image.Set(x, y, result);

    return;
}
