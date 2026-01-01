#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
const float INF = std::numeric_limits<float>::infinity();


using namespace std;

struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    Vec3 operator-(const Vec3& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    Vec3 operator*(float s) const {
        return {x * s, y * s, z * s};
    }
};

struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;   

};


float dot(const Vec3& a, const Vec3& b) 
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 normalize(const Vec3& v)
{
    float len = sqrt(dot(v, v));
    return v * (1.0f / len);
}

bool hitSphere(const Sphere& s, Vec3 rayOrig, Vec3 rayDir, float& t) {

    //Vector from sphere center to ray origin
    Vec3 oc = rayOrig - s.center;

    //a*t² + b*t + c = 0

    float a = dot(rayDir, rayDir);
    float b = 2.0f * dot(oc, rayDir);
    float c = dot(oc, oc) - s.radius * s.radius;

    //to check wether a ray actaully hits the sphere or not, the equation being a rad^2 - (point on the sphere - origin of sphere)^2 
    //< 0  No intersection
    //= 0 Grazes sphere (tangent)
    //> 0 Two intersections (enter & exit)

    float disc = b*b - 4*a*c;
    if (disc < 0) return false;

    //t is the distance (parameter) along the ray where the intersection happens
    //At what distance along the ray do we hit the object
    t = (-b - sqrt(disc)) / (2*a);
    return t > 0;
}


int main()
{
    int height = 2400;
    int width = 2400;
    
    std::ofstream img("output.ppm");
    img<<"P3\n"<<width<< " " << height << "\n255\n";

    // for(int y =0;y<height;y++)
    // {
    //     for(int x = 0;x<width;x++)
    //     {
    //         img << "255 0 0";
    //     }
    //     img << "\n";
    // }


//camera positioning 
//viewport is the dimensions of the scene where as width and height are dimensions of the pixels rendered for the scene
Vec3 cameraPos = {0, 0, 0};
float viewportHeight = 2.0f;
float viewportWidth = 2.0f;
float focalLength = 1.0f;

//Ray Per Pixel
//the first 2 coords are used to render the direction of each pixel in the scene from the centre(camera)The third coordinate is the depth of the image plane (viewport)


//spheres
vector<Sphere> spheres = {{{0, 0, -3}, 1.0f,{255, 255, 0}}, {{1, 5, -3}, 3.0f,{255, 225, 0}}, {{-7, 2, -4}, 2.0f,{215, 255, 3}}};

// RAY PER PIXEL LOOP

    //loop used to create a ray for each individual pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            Vec3 rayDir = {
                (x / (float)width  - 0.5f) * viewportWidth,
                (y / (float)height - 0.5f) * viewportHeight,
                -focalLength
            };

            rayDir = normalize(rayDir);

            float t; //distance of point from origin of ray to sphere

            //loops between the spheres present in the vector and places them in closestsphere
            float closest = INF;
            Sphere* ClosestSphere = nullptr;

            for (auto& sphere : spheres)
            {
                if (hitSphere(sphere, cameraPos, rayDir, t) && t < closest) {
                closest = t;
                ClosestSphere = &sphere;
            }
            }

            if(!ClosestSphere)
            {
                img << "50 80 200 "; //background
                continue;
            }

            Vec3 hitPoint = cameraPos + rayDir * closest;
            Vec3 normal = normalize(hitPoint - ClosestSphere->center);

            //Point light stuff
            Vec3 lightPos = {5, 5, 0};
            Vec3 toLight = lightPos - hitPoint;
            float distance = sqrt(dot(toLight, toLight));
            Vec3 lightDir = normalize(toLight);

            //shadow stuff
            bool inShadow = false;
            Vec3 shadowOrigin = hitPoint + normal * 0.001f;

            for (auto& sphere : spheres) 
            {
                float shadowT;
                if (&sphere != ClosestSphere && hitSphere(sphere, shadowOrigin, lightDir, shadowT) && shadowT < distance)
                {
                inShadow = true;
                break;
                }
            }

            
           
                //lighting intensity
                //This code computes how much light hits the surface at the ray–sphere intersection by measuring the angle between the surface normal and the light direction, then converts that brightness into a grayscale pixel value.

                float intensity = std::max(dot(normal, lightDir), 0.0f);
                float attenuation = 1.0f / (1.0f + distance * distance);

                intensity *= attenuation;

            if (inShadow) 
            {
                intensity *= 0.2f; // ambient only
            }

                int r = int(ClosestSphere->color.x * intensity);
                int g = int(ClosestSphere->color.y * intensity);
                int b = int(ClosestSphere->color.z * intensity);

                img << r << " " << g << " " << b << " "; //final sphere + lighting

        }
        img << "\n";
    }

img.close();

return 0;

}