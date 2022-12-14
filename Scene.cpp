#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                // sample from mesh triangle
                // however in this case,
                // if there are several light sources, the integral may not be correct
                // because the denominator of the pdf only counts one light source's area
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

Vector3f Scene::castRay(const Ray &ray, const Intersection &intersection, int depth) const
{
    Material *m = intersection.m;

    if(m->hasEmission()){
        return m->getEmission();
    }
    Vector3f hitPoint = intersection.coords;
    Vector3f N = intersection.normal;
    Intersection pos;
    float pdf_light;
    
    sampleLight(pos, pdf_light);
    Vector3f x = pos.coords;
    Vector3f wsOrig = x-hitPoint;
    Vector3f ws = wsOrig.normalized();
    Vector3f wo = -ray.direction;
    Vector3f NN = pos.normal;
    Vector3f L_dir = 0.0f;

    if((intersect(Ray(hitPoint, ws)).coords - x).norm2() < EPSILON){
        L_dir = pos.emit * m->eval(wo, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN) / (wsOrig.norm2() * pdf_light);
    }

    Vector3f L_indir = 0.0f;
    if(get_random_float() < RussianRoulette){
        Vector3f wi = m->sample(wo , N);
        Ray ray2(hitPoint, wi);
        Intersection intersection2 = intersect(ray2);
        if(intersection2.happened && !(intersection2.m->hasEmission())){
            float pdf = std::max(m->pdf(wo, wi, N), EPSILON);
            L_indir = castRay(ray2, intersection2, depth+1) * m->eval(wo, wi, N) * dotProduct(wi, N) / (pdf * RussianRoulette);
        }
    }

    return L_indir + L_dir;
}