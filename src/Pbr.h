#pragma once
#include <glm/glm.hpp>
#include <algorithm>
const float PI = 3.1415926;

// predefined F0 fresnel value

const glm::vec3 IRON{ 0.562, 0.565, 0.578 }, Copper{ 0.955, 0.638, 0.538 }, GOLD{ 1.000, 0.782, 0.344 }, SILVER{ 0.972, 0.960, 0.915 };

enum class ndf
{
    Beckman,
    Blinn,
    GGX
};

/* Normal Distribution Function*/
// n stand for normal, m stand for microfacts normal

// GGX NDF
float NDF(glm::vec3 n, glm::vec3 m, float roughness, ndf _ndf)
{
    switch (_ndf) {
    case ndf::Beckman:
    {
        float ndotm = std::max(glm::dot(n, m), 0.0f);
        float ndotm2 = ndotm * ndotm;
        float roughness2 = roughness * roughness;
        float nom = std::exp((ndotm2 - 1.0f) / (roughness2 * ndotm2));
        float denom = PI * roughness2 * ndotm2 * ndotm2;
        return nom / denom;
    }
    case ndf::Blinn:
    {
        float ndotm = std::max(glm::dot(n, m), 0.0f);
        float denom = 2 * PI;
        float nom = (roughness + 2.0f) * std::powf(ndotm, roughness);
        return nom / denom;
    }
    case ndf::GGX:
    {
        float alpha = roughness * roughness;
        float alpha2 = alpha * alpha;
        float ndotm = std::max(glm::dot(n, m), 0.0f);
        float ndotm2 = ndotm * ndotm;

        float nom = alpha2;
        float denom = (ndotm2 * (alpha2 - 1.0) + 1.0);
        denom = PI * denom * denom;

        return nom / denom;
    }
    }
}

// Geometry


float GeometrySmith(glm::vec3 n, glm::vec3 v, glm::vec3 l, float k)
{
    float ndotv = std::max(glm::dot(n, v), 0.0f);
    float denom1 = ndotv * (1.0f - k) + k;

    float ndotl = std::max(glm::dot(n, l), 0.0f);
    float denom2 = ndotl * (1.0f - k) + k;

    return ndotv * ndotl / (denom1 * denom2);

}

// Fresnel
glm::vec3 Fresnel(glm::vec3 F0, glm::vec3 n, glm::vec3 l)
{
    // schlick approx
    return { F0 + (glm::vec3(1.0f) - F0) * powf((1 - std::max(glm::dot(n, l), 0.0f)),5.0f) };
}
