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

    glm::mat4x4 scaling {
        transform.scale.x,  0,  0,  0,
        0,  transform.scale.y,  0,  0,
        0,  0,  transform.scale.z,  0,
        0,  0,  0,  1,
    };
    
    glm::mat4x4 translation { 
        1,  0,  0,  0,
        0,  1,  0,  0,
        0,  0,  1,  0,
        transform.translation.x,  transform.translation.y,  transform.translation.z,  1,
    };

    model.push_back(translation * rotation * scaling);

    return;
}

// TODO
void Rasterizer::SetView()
{
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = glm::normalize(camera.lookAt - cameraPos);
    glm::vec3 cameraUp = glm::normalize(camera.up);
    glm::vec3 binormal = glm::normalize(glm::cross(cameraLookAt, cameraUp));

    glm::mat4 translation {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -cameraPos.x, -cameraPos.y, -cameraPos.z, 1
    };

    glm::mat4 rotation {
        binormal.x, cameraUp.x, -cameraLookAt.x, 0,
        binormal.y, cameraUp.y, -cameraLookAt.y, 0,
        binormal.z, cameraUp.z, -cameraLookAt.z, 0,
        0, 0, 0, 1
    };

    // TODO change this line to the correct view matrix
    this->view = rotation * translation;

    return;
}

// TODO
void Rasterizer::SetProjection()
{
    const Camera& camera = this->loader.GetCamera();

    float n = -camera.nearClip;                   // near clipping distance, strictly positive
    float f = -camera.farClip;                     // far clipping distance, strictly positive
    float width = camera.width;
    float height = camera.height;

    std::cout << "n: " << n << " f: " << f << " width: " << width << " height: " << height << std::endl;

    glm::mat4 persp_to_ortho {
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, 1,
        0, 0, -n * f, 0,
    };

    glm::mat4 ortho_translate {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -(n+f)/2, 1
    };

    glm::mat4 ortho_scale {
        2 / width, 0, 0, 0,
        0, 2 / height, 0, 0,
        0, 0, 2 / (n - f), 0,
        0, 0, 0, 1
    };

    // TODO change this line to the correct projection matrix
    this->projection = ortho_scale * ortho_translate * persp_to_ortho;

    return;
}

// TODO
void Rasterizer::SetScreenSpace()
{
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    // TODO change this line to the correct screenspace matrix
    this->screenspace = glm::mat4{
        width/2, 0, 0, 0,
        0, height/2, 0, 0,
        0, 0, 1, 0,
        width/2, height/2, 0, 1,
    };

    return;
}

// TODO
glm::vec3 Rasterizer::BarycentricCoordinate(glm::vec2 pos, Triangle trig)
{
    auto x = pos.x;
    auto y = pos.y;
    auto Xa = trig.pos[0].x;
    auto Ya = trig.pos[0].y;
    auto Xb = trig.pos[1].x;
    auto Yb = trig.pos[1].y;
    auto Xc = trig.pos[2].x;
    auto Yc = trig.pos[2].y;

    float alpha = ((Xb-x)*(Yc-Yb) + (y-Yb)*(Xc-Xb)) / ((Xb-Xa)*(Yc-Yb) + (Ya-Yb)*(Xc-Xb)); 
    float beta = ((Xc-x)*(Ya-Yc) + (y-Yc)*(Xa-Xc)) / ((Xc-Xb)*(Ya-Yc) + (Yb-Yc)*(Xa-Xc));
    float gamma = 1 - alpha - beta;

    return glm::vec3(alpha, beta, gamma);
}

// TODO
float Rasterizer::zBufferDefault = float(-1);

// TODO
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{
    transformed.Homogenize();
    glm::vec3 bc = BarycentricCoordinate(glm::vec2(x+0.5f, y+0.5f), transformed);
    if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) return;

    float Az = transformed.pos[0].z;
    float Bz = transformed.pos[1].z;
    float Cz = transformed.pos[2].z;

    float result = bc[0] * Az + bc[1] * Bz + bc[2] * Cz;
    if (result > ZBuffer.Get(x, y).value_or(zBufferDefault)) {
        ZBuffer.Set(x, y, result);
    }
}

// TODO
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{
    transformed.Homogenize();

    glm::vec3 bc = BarycentricCoordinate(glm::vec2(x+0.5f, y+0.5f), transformed);
    if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) return;

    float Az = transformed.pos[0].z;
    float Bz = transformed.pos[1].z;
    float Cz = transformed.pos[2].z;
    const Camera& camera = this->loader.GetCamera();
    float n = camera.nearClip;

    float result = bc.x * Az + bc.y * Bz + bc.z * Cz;

    if (result == ZBuffer.Get(x, y).value_or(zBufferDefault)) {
        // float ndc_x = bc[0] * transformed.pos[0].x + bc[1] * transformed.pos[1].x + bc[2] * transformed.pos[2].x;
        // float ndc_y = bc[0] * transformed.pos[0].y + bc[1] * transformed.pos[1].y + bc[2] * transformed.pos[2].y;
        // float world_z = 1 / (bc[0] / transformed.pos[0].z + bc[1] / transformed.pos[1].z + bc[2] / transformed.pos[2].z);
        // float world_x = ndc_x * world_z / n;
        // float world_y = ndc_y * world_z / n;

        glm::vec3 pos_world = bc[0] * original.pos[0] + 
                              bc[1] * original.pos[1] + 
                              bc[2] * original.pos[2];
        glm::vec3 normal = bc[0] * original.normal[0] + bc[1] * original.normal[1] + bc[2] * original.normal[2];

        // attempt to compute the non-approximate world coordinates of the pixel
        // original.Homogenize();
        // glm::vec3 bc_world = BarycentricCoordinate(glm::vec2(world_x, world_y), original);
        // glm::vec3 pos_world = bc_world[0] * original.pos[0] + bc_world[1] * original.pos[1] + bc_world[2] * original.pos[2];
        // glm::vec3 normal = bc_world[0] * original.normal[0] + bc_world[1] * original.normal[1] + bc_world[2] * original.normal[2];

        normal = glm::normalize(normal);

        glm::vec3 v = glm::normalize(camera.pos-pos_world);

        Color ambient = this->loader.GetAmbientColor();
        const std::vector<Light>& lights = this->loader.GetLights();
        for (const Light& light : lights) {
            glm::vec3 l = glm::normalize(light.pos-pos_world);
            glm::vec3 h = glm::normalize(l + v);

            float r_squared = pow(glm::length(light.pos-pos_world), 2);
            float l_d = (light.intensity/r_squared * glm::max(0.f, glm::dot(normal, l)));
            float l_s = (light.intensity/r_squared * pow(glm::max(0.f, glm::dot(normal, h)), this->loader.GetSpecularExponent()));
            ambient = ambient + light.color * (l_d + l_s);
        }

        image.Set(x, y, ambient);
    }

    return;
}
