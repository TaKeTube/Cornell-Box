#pragma once
#include "Vector.hpp"
#include "global.hpp"

enum MaterialType {DIFFUSE, MICROFACET};
enum SamplingType {UNIFORM, IS_COSWEIGHTED, IS_BRDF};

class Material{
private:

    // Compute reflection direction
    Vector3f reflect(const Vector3f &I, const Vector3f &N) const
    {
        return I - 2 * dotProduct(I, N) * N;
    }

    // Compute refraction direction using Snell's law
    // We need to handle with care the two possible situations:
    //    - When the ray is inside the object
    //    - When the ray is outside.
    // If the ray is outside, you need to make cosi positive cosi = -N.I
    // If the ray is inside, you need to invert the refractive indices and negate the normal N
    Vector3f refract(const Vector3f &I, const Vector3f &N, const float &ior) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        Vector3f n = N;
        if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
    }

    // Compute Fresnel equation
    // \param I is the incident view direction
    // \param N is the normal at the intersection point
    // \param ior is the material refractive index
    // \param[out] kr is the amount of light reflected
    void fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {  std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
    }

    Vector3f toWorld(const Vector3f &a, const Vector3f &N){
        // build a TNB coordinate, 
        // then transform a in the TNB coordinate into the world coordinate
        Vector3f B, C;
        if (std::fabs(N.x) > std::fabs(N.y)){
            float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
            C = Vector3f(N.z * invLen, 0.0f, -N.x *invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
            C = Vector3f(0.0f, N.z * invLen, -N.y *invLen);
        }
        B = crossProduct(C, N);
        return a.x * B + a.y * C + a.z * N;
    }

public:
    MaterialType m_type;
    SamplingType m_sample;
    Vector3f m_emission;
    float ior;
    float alpha;    // roughness
    float ks;
    Vector3f rho;   // intrinsic color
    Vector3f Kd, Ks;
    Vector3f F0;
    float specularExponent;

    inline Material(MaterialType t=DIFFUSE, Vector3f e=Vector3f(0,0,0), SamplingType s=UNIFORM);
    inline MaterialType getType();
    inline Vector3f getEmission();
    inline bool hasEmission();

    // sample a ray by Material properties
    inline Vector3f sample(const Vector3f &wo, const Vector3f &N);
    // given a ray, calculate the PdF of this ray
    inline float pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);
    // given a ray, calculate the contribution of this ray
    inline Vector3f eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

};

Material::Material(MaterialType t, Vector3f e, SamplingType s){
    m_type = t;
    m_sample = s;
    m_emission = e;
}

MaterialType Material::getType(){return m_type;}
Vector3f Material::getEmission() {return m_emission;}
bool Material::hasEmission() {
    if (m_emission.norm() > EPSILON) return true;
    else return false;
}

Vector3f Material::sample(const Vector3f &wo, const Vector3f &N){
    switch(m_sample){
        case UNIFORM:
        {
            // uniform sample on the hemisphere
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            return toWorld(localRay, N).normalized();
        }
        case IS_COSWEIGHTED:
        {
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::sqrt(1.0f - x_1);
            float r = std::sqrt(x_1), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            return toWorld(localRay, N).normalized();
        }
        case IS_BRDF:
        {
            float x_1 = get_random_float(), x_2 = get_random_float();
            float a = (1-x_1)/(x_1*(alpha*alpha-1)+1);
            float z = std::sqrt(a);
            float r = std::sqrt(1-a), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            Vector3f wh = normalize(toWorld(localRay, N));
            Vector3f wi = normalize(2*dotProduct(wo, wh)*wh-wo);
            return wi;
        }
        default:
        {
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            return toWorld(localRay, N).normalized();
        }
    }
}

float Material::pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_sample){
        case UNIFORM:
        {
            // uniform sample probability 1 / (2 * PI)
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
        case IS_COSWEIGHTED:
        {
            if (dotProduct(wo, N) > 0.0f){
                float NdotWo = std::max(dotProduct(wo, N), 0.f);
                return NdotWo / M_PI;
            }
            else
                return 0.0f;
            break;
        }
        case IS_BRDF:
        {
            if (dotProduct(wo, N) > 0.0f){
                Vector3f wh = (wo + wi).normalized();
                float Ndoth = std::max(dotProduct(N, wh), 0.f);
                float hdotwo = std::max(dotProduct(wo, wh), 0.f);
                float alpha2 = alpha*alpha;
                float temp = (Ndoth*Ndoth*(alpha2-1.0f)+1);
                float D = alpha2/(M_PI*temp*temp);

                return Ndoth*D*0.25/hdotwo;
            }
            else
                return 0.0f;
            break;
        }
        default:
        {
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
    }
}

Vector3f Material::eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // calculate the contribution of diffuse model
            float cosalpha = dotProduct(N, wo);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = Kd / M_PI;
                return diffuse;
            }
            else
                // return Vector3f(0.0f);
                return 0.0f;
            break;
        }
        case MICROFACET:
        {
            float NdotWo = dotProduct(N, wo);
            if (NdotWo <= 0.0f) {
                return Vector3f(0.0f);
            }
            Vector3f h = (wi + wo).normalized();
            float NdotWi = dotProduct(N, wi);
            float Ndoth = dotProduct(N, h);
            if (NdotWi <= 0.0f || Ndoth <= 0.0f){
                return Vector3f(0.0f);
            }
            
            Vector3f F = F0 + (Vector3f(1.0f)-F0)*pow(1-NdotWo,5);
            
            float k = (alpha+1)*(alpha+1)*0.125f;
            float G = NdotWo*NdotWi/(NdotWo*(1-k)+k)/(NdotWi*(1-k)+k);

            float alpha2 = alpha*alpha;
            float temp = (Ndoth*Ndoth*(alpha2-1.0f)+1);
            float D = alpha2/(M_PI*temp*temp);

            Vector3f cook_torrance = F*D*G/(4*NdotWo*NdotWi);
            Vector3f lambert = rho/M_PI;

            return (1-ks)*lambert + ks*cook_torrance;
            // return (1.f-std::max(std::max(F.x, F.y), F.z))*lambert + cook_torrance;
        }
        default:
        {
            // calculate the contribution of diffuse model
            float cosalpha = dotProduct(N, wo);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = Kd / M_PI;
                return diffuse;
            }
            else
                return Vector3f(0.0f);
            break;
        }
    }
}
