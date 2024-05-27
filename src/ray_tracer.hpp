// IFT3100H23_Raytracer/raytracer.cpp
// Exemple d'un raytracer minimaliste.
// Inspir� de smallpt.
// make : g++ -std=c++11 raytracer.cpp -o raytracer
// run  : ./raytracer image_width image_height ray_per_pixel

// https://www.coolutils.com/online/PPM-to-PNG

#define M_PI 3.14159265358979323846

#pragma once

#include "camera/camera_controller.h"
#include "ofMain.h"
#include "renderer.h"
// c
#include <cmath>  // sqrt pow fabs cos sin

// c++
#include <fstream>
#include <iostream>  // std cout endl
#include <sstream>   // stringstream

// c++11
#include <chrono>
#include <random>
#include <vector>

// param�tres du programme
const int max_depth = 3;
const double camera_fov = 0.5135;  // ~30 degr�s
constexpr double gamma_correction = 1 / 2.2;

// variables du programme
int image_width = 0;
int image_height = 0;
int ray_per_pixel = 0;
int pixel_count = 0;

// types de mat�riau
enum class SurfaceType { diffuse, specular, refraction };

// fonction pour borner une valeur num�rique entre 0 et 1
double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}

// type vecteur 3D
struct Vector {
    double x, y, z;

    // construction
    constexpr Vector() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vector(double x) : x(x), y(0.0), z(0.0) {}
    constexpr Vector(double x, double y) : x(x), y(y), z(0.0) {}
    constexpr Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    // produit scalaire (dot product)
    double dot(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }

    // produit vectoriel (cross product)
    Vector cross(const Vector& v) const { return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

    // multiplication vectorielle
    Vector multiply(const Vector& v) const { return Vector(x * v.x, y * v.y, z * v.z); }

    // multiplication scalaire
    Vector operator*(double s) const { return Vector(x * s, y * s, z * s); }

    // addition vectorielle
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }

    // soustraction vectorielle
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }

    Vector operator/(const Vector& v) const { return Vector(x / v.x, y / v.y, z / v.z); }

    // normalisation
    Vector& normalize() { return *this = *this * (1.0 / sqrt(x * x + y * y + z * z)); }

    double norm() { return sqrt(x * x + y * y + z * z); }
};

// structure d'un rayon sous forme de droite param�trique
// �quation d'un rayon : p(t) = o + td
struct Ray {
    Vector origin;
    Vector direction;

    Ray(Vector o, Vector d) : origin(o), direction(d) {}
};

// type Sphere sous forme d'un point central et d'un rayon
struct Sphere {
    double radius;    // rayon de la sph�re
    Vector position;  // position du centre de la sph�re
    Vector emission;  // couleur �missive de la sph�re
    Vector color;     // couleur diffuse de la sph�re

    SurfaceType material;  // type de r�flexion de la sph�re

    // constructeur
    Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m)
        : radius(r), position(p), emission(e), color(c), material(m) {}

    // fonction d'intersection entre la sph�re et un rayon
    double intersect(const Ray& ray) const {
        // distance de l'intersection la plus pr�s si elle existe
        double distance;

        // seuil de tol�rance num�rique du test d'intersection
        double epsilon = 1e-4;

        // distance du point d'intersection
        double t;

        // vecteur entre le centre de la sph�re et l'origine du rayon
        Vector delta = position - ray.origin;

        // calculer a
        double a = delta.dot(delta);

        // calculer b
        double b = delta.dot(ray.direction);

        // calculer c
        double c = radius * radius;

        // calculer le discriminant de l'�quation quadratique
        double discriminant = b * b - a + c;

        // valider si le discriminant est n�gatif
        if (discriminant < 0) {
            // il n'y a pas d'intersection avec cette sph�re
            return distance = 0;
        }

        // calculer la racine carr�e du discriminant seulement si non n�gatif
        discriminant = sqrt(discriminant);

        // d�terminer la distance de la premi�re intersection
        t = b - discriminant;

        // valider si la distance de la premi�re intersection est dans le seuil de tol�rance
        if (t > epsilon)
            distance = t;
        else {
            // d�terminer la distance de la premi�re intersection
            t = b + discriminant;

            // valider si la distance de la seconde intersection est dans le seuil de tol�rance
            if (t > epsilon)
                distance = t;
            else
                distance = 0;
        }

        // retourner la distance du point d'intersection
        return distance;
    }
};

struct Plane {
    Vector position;
    Vector normal;
    Vector emission;
    Vector color;
    SurfaceType material;

    Plane(Vector p, Vector n, Vector e, Vector c, SurfaceType m)
        : position(p), normal(n), emission(e), color(c), material(m) {}

    double intersect(const Ray& ray) const {
        double denom = normal.dot(ray.direction);

        if (fabs(denom) > 1e-6) {
            double t = ((position - normal).dot(ray.origin)) / denom;

            if (t >= 0) {
                return t;
            }
        }
        return 0;
    }
};

struct Box {
    Vector min;
    Vector max;
    Vector emission;
    Vector color;
    SurfaceType material;

    Box(const Vector& min, const Vector& max, const Vector& e, const Vector& c, SurfaceType m)
        : min(min), max(max), emission(e), color(c), material(m) {}

    double intersect(const Ray& ray) const {
        double txmin = (min.x - ray.origin.x) / ray.direction.x;
        double txmax = (max.x - ray.origin.x) / ray.direction.x;

        if (txmin > txmax) {
            double temp = txmax;
            txmax = txmin;
            txmin = txmax;
        }

        double tymin = (min.y - ray.origin.y) / ray.direction.y;
        double tymax = (max.y - ray.origin.y) / ray.direction.y;

        if (tymin > tymax) {
            double temp = tymax;
            tymax = tymin;
            tymin = tymax;
        }

        if ((txmin > tymax) || (tymin > txmax)) return 0;

        if (tymin > txmin) txmin = tymin;
        if (tymax < txmax) txmax = tymax;
        double tzmin = (min.z - ray.origin.z) / ray.direction.z;
        double tzmax = (max.z - ray.origin.z) / ray.direction.z;

        if (tzmin > tzmax) {
            double temp = tzmax;
            tzmax = tzmin;
            tzmin = tzmax;
        }

        if ((txmin > tzmax) || (tzmin > txmax)) return 0;

        if (tzmin > txmin) txmin = tzmin;
        if (tzmax < txmax) txmax = tzmax;

        Vector intersection = Vector(txmin, tymin, tzmin);

        double distance = (intersection.x - ray.origin.x) / ray.direction.x +
                          (intersection.y - ray.origin.y) / ray.direction.y +
                          (intersection.z - ray.origin.z) / ray.direction.z;

        // std::cout << (intersection.x - ray.origin.x) / ray.direction.x << " "
        //           << (intersection.y - ray.origin.y) / ray.direction.y << " "
        //           << (intersection.z - ray.origin.z) / ray.direction.z << std::endl;

        return distance;
    }
};

// structure d'une cam�ra
struct Camera {
    Vector position;
    Vector orientation;
    Vector target;

    Vector axis_x;
    Vector axis_y;
    Vector axis_z;

    Vector up;

    float viewport_width;
    float viewport_height;

    float fov;

    Camera(Vector p, Vector o) : position(p), orientation(o) {}

    // fonction qui permet de calibrer la cam�ra en fonction la valeur courante de ses attributs
    void calibrate() {
        axis_z = orientation;
        axis_x = Vector(viewport_width * fov / viewport_height);
        axis_y = axis_x.cross(axis_z).normalize() * fov;
    }
};

// structure d'une image
struct Image {
    int width;   // largeur de l'image
    int height;  // hauteur de l'image
    int count;   // nombre de pixels
    int index;   // index d'un des pixels

    double size;  // poids en m�moire (en m�gaoctets)

    // pointeur vers une s�quence de pixels sous forme vectorielle
    Vector* pixel;

    // constructeurs
    Image() : width(1), height(1) {}
    Image(int w) : width(w), height(w) {}
    Image(int w, int h) : width(w), height(h) {}

    // fonction qui redimensionne l'image
    void resize(int w, int h) {
        if (w > 0 && h > 0) {
            width = w;
            height = h;

            // calculer le nombre de pixels dans l'image
            count = width * height;

            // calculer la taille de l'espace m�moire (en m�gaoctets)
            size = sizeof(Vector) * count / pow(1024.0, 2);

            // allocation de la m�moire pour les pixels de l'image
            pixel = new Vector[count];
        }
    }

    ~Image() {
        if (pixel != nullptr) delete[] pixel;
    }
};

// d�claration du graphe de spheres
std::vector<Sphere> sceneSphere;

// d�claration du graphe de plans
std::vector<Plane> scenePlane;

// d�claration du graphe de Boxs
std::vector<Box> sceneBox;

// orientation initiale de la cam�ra
Vector camera_orientation(Vector(0, -0.042612, -1).normalize());

// position initiale de la cam�ra
constexpr Vector camera_position(50, 50, 300);

// d�claration de la cam�ra utilis�e pour rendre la sc�ne
Camera camera(camera_position, camera_orientation);

// d�claration d'une image
Image image;

// source d'entropie
std::random_device rd;

// g�n�rateur de nombres pseudo-al�atoires (algorithme mersenne twister)
std::mt19937 rng{rd()};

// distribution uniforme entre 0 et 1
std::uniform_real_distribution<double> random01{0.0, 1.0};

// traitement de la couleur d'un pixel
int format_color_component(double value) {
    // appliquer la correction gamma
    value = pow(clamp(value), gamma_correction);

    // convertir la valeur dans l'espace de couleur
    value = value * 255 + 0.5;

    // conversion num�rique de r�el vers entier
    return static_cast<int>(value);
}

enum Geomety { SPHERE, PLANE, BOX };

// fonction qui valide s'il y a intersection entre un rayon et les g�om�tries de la sc�ne
// retourne l'intersection la plus pr�s de la cam�ra (distance et index de l'�l�ment)
bool raycast(const Ray& ray, double& distance, int& id, int& type) {
    // variable temporaire pour la distance d'une intersection entre un rayon et une sph�re
    double d;

    // initialiser la distance � une valeur suffisamment �loign�e pour qu'on la consid�re comme l'infinie
    double infinity = distance = 1e20;

    int l = static_cast<int>(sceneBox.size());

    for (int index = 0; index < l; ++index) {
        d = sceneBox[index].intersect(ray);

        if (d && d < distance) {
            distance = d;
            id = index;
            type = BOX;
        }
    }

    // nombre d'�l�ments dans la sc�ne
    int n = static_cast<int>(scenePlane.size());

    for (int index = 0; index < n; ++index) {
        d = scenePlane[index].intersect(ray);

        if (d && d < distance) {
            distance = d;
            id = index;
            type = PLANE;
        }
    }

    int m = static_cast<int>(sceneSphere.size());

    for (int index = 0; index < m; ++index) {
        d = sceneSphere[index].intersect(ray);

        if (d && d < distance) {
            distance = d;
            id = index;
            type = SPHERE;
        }
    }

    // std::cout << type << std::endl;

    // il y a eu intersection si la distance est plus petite que l'infini
    if (distance < infinity)
        return true;
    else
        return false;
}

// fonction r�cursive qui calcule la radiance
Vector compute_radiance(const Ray& ray, int depth) {
    // valeur de la radiance
    Vector radiance;

    // distance de l'intersection
    double distance;

    // identifiant de la g�om�trie en intersection
    int id = 0;

    int typeGeometry = -1;

    // valider s'il n'y a pas intersection
    if (!raycast(ray, distance, id, typeGeometry)) return Vector();  // couleur par d�fault (noir)

    if (typeGeometry == SPHERE) {
        // r�f�rence sur une g�om�trie en intersection avec un rayon
        const Sphere& obj = sceneSphere[id];

        // calculer les coordonn�es du point d'intersection
        Vector x = ray.origin + ray.direction * distance;

        // calculer la normale au point d'intersection
        Vector n = (x - obj.position).normalize();

        // ajustement de la direction de la normale
        Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

        // isoler la composante de couleur la plus puissante
        Vector f = obj.color;
        double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

        // valider si la limite du nombre de r�cursions est atteinte
        if (++depth > max_depth) {
            // test de probabilit�
            if (random01(rng) < threshold)
                f = f * (1 / threshold);
            else
                return obj.emission;
        }

        if (obj.material == SurfaceType::diffuse) {
            // mat�riau avec r�flexion diffuse

            double r1 = 2 * M_PI * random01(rng);
            double r2 = random01(rng);
            double r2s = sqrt(r2);

            Vector w = na;
            Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
            Vector v = w.cross(u);
            Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

            radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

            return radiance;
        } else if (obj.material == SurfaceType::specular) {
            // mat�riau avec r�flexion sp�culaire

            radiance = obj.emission +
                       f.multiply(compute_radiance(Ray(x, ray.direction - n * 2.0 * n.dot(ray.direction)), depth));

            return radiance;
        } else if (obj.material == SurfaceType::refraction) {
            // mat�riau avec r�flexion r�fraction

            Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

            bool into = n.dot(na) > 0;

            double ior = 1.5;  // indice de r�fraction du verre
            double nc = 1.0;
            double nt = ior;
            double nnt = into ? nc / nt : nt / nc;
            double ddn = ray.direction.dot(na);
            double cos2t;

            if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0) {
                radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

                return radiance;
            }

            Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

            // effet de fresnel
            double a = nt - nc;
            double b = nt + nc;
            double r0 = a * a / (b * b);
            double c = 1.0 - (into ? -ddn : tdir.dot(n));
            double re = r0 + (1.0 - r0) * c * c * c * c * c;
            double tr = 1 - re;
            double p = 0.25 + 0.5 * re;
            double rp = re / p;
            double tp = tr / (1.0 - p);

            radiance =
                obj.emission + f.multiply(depth > 2 ? (random01(rng) < p ? compute_radiance(reflection_ray, depth) * rp
                                                                         : compute_radiance(Ray(x, tdir), depth) * tp)
                                                    : compute_radiance(reflection_ray, depth) * re +
                                                          compute_radiance(Ray(x, tdir), depth) * tr);

            return radiance;
        } else {
            return radiance;
        }
    } else if (typeGeometry == PLANE) {
        // r�f�rence sur une g�om�trie en intersection avec un rayon
        const Plane& obj = scenePlane[id];

        // calculer les coordonn�es du point d'intersection
        Vector x = ray.origin + ray.direction * distance;

        // calculer la normale au point d'intersection
        Vector n = obj.normal;

        // ajustement de la direction de la normale
        Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

        // isoler la composante de couleur la plus puissante
        Vector f = obj.color;
        double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

        // valider si la limite du nombre de r�cursions est atteinte
        if (++depth > max_depth) {
            // test de probabilit�
            if (random01(rng) < threshold)
                f = f * (1 / threshold);
            else
                return obj.emission;
        }

        if (obj.material == SurfaceType::diffuse) {
            // mat�riau avec r�flexion diffuse

            double r1 = 2 * M_PI * random01(rng);
            double r2 = random01(rng);
            double r2s = sqrt(r2);

            Vector w = na;
            Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
            Vector v = w.cross(u);
            Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

            radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

            return radiance;
        } else if (obj.material == SurfaceType::specular) {
            // mat�riau avec r�flexion sp�culaire

            radiance = obj.emission +
                       f.multiply(compute_radiance(Ray(x, ray.direction - na * 2.0 * na.dot(ray.direction)), depth));

            return radiance;
        } else if (obj.material == SurfaceType::refraction) {
            // mat�riau avec r�flexion r�fraction

            Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

            bool into = n.dot(na) > 0;

            double ior = 1.5;  // indice de r�fraction du verre
            double nc = 1.0;
            double nt = ior;
            double nnt = into ? nc / nt : nt / nc;
            double ddn = ray.direction.dot(na);
            double cos2t;

            if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0) {
                radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

                return radiance;
            }

            Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

            // effet de fresnel
            double a = nt - nc;
            double b = nt + nc;
            double r0 = a * a / (b * b);
            double c = 1.0 - (into ? -ddn : tdir.dot(n));
            double re = r0 + (1.0 - r0) * c * c * c * c * c;
            double tr = 1 - re;
            double p = 0.25 + 0.5 * re;
            double rp = re / p;
            double tp = tr / (1.0 - p);

            radiance =
                obj.emission + f.multiply(depth > 2 ? (random01(rng) < p ? compute_radiance(reflection_ray, depth) * rp
                                                                         : compute_radiance(Ray(x, tdir), depth) * tp)
                                                    : compute_radiance(reflection_ray, depth) * re +
                                                          compute_radiance(Ray(x, tdir), depth) * tr);

            return radiance;
        } else {
            return radiance;
        }
    } else if (typeGeometry == BOX) {
        // std::cout << "Box";
        //  r�f�rence sur une g�om�trie en intersection avec un rayon
        const Box& obj = sceneBox[id];

        // calculer les coordonn�es du point d'intersection
        Vector x = ray.origin + ray.direction * distance;

        // calculer la normale au point d'intersection
        double epsilon = 0.00001;
        Vector n = Vector();
        if (x.x - obj.min.x < epsilon) {
            n = Vector(-1, 0, 0) * 10000;  // Plan gauche
        } else if (x.x - obj.max.x < epsilon) {
            n = Vector(1, 0, 0) * 10000;  // Plan droit
        } else if (x.y - obj.min.y < epsilon) {
            n = Vector(0, -1, 0) * 10000;  // Plan bas
        } else if (x.y - obj.max.y < epsilon) {
            n = Vector(0, 1, 0) * 10000;  // Plan haut
        } else if (x.z - obj.min.z < epsilon) {
            n = Vector(0, 0, -1) * 10000;  // Plan arri�re
        } else if (x.z - obj.max.z < epsilon) {
            n = Vector(0, 0, 1) * 10000;  // Plan avant
        }

        // ajustement de la direction de la normale
        Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

        // isoler la composante de couleur la plus puissante
        Vector f = obj.color;
        double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

        // valider si la limite du nombre de r�cursions est atteinte
        if (++depth > max_depth) {
            // test de probabilit�
            if (random01(rng) < threshold)
                f = f * (1 / threshold);
            else
                return obj.emission;
        }

        if (obj.material == SurfaceType::diffuse) {
            // mat�riau avec r�flexion diffuse

            double r1 = 2 * M_PI * random01(rng);
            double r2 = random01(rng);
            double r2s = sqrt(r2);

            Vector w = na;
            Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
            Vector v = w.cross(u);
            Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

            radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

            return radiance;
        } else if (obj.material == SurfaceType::specular) {
            // mat�riau avec r�flexion sp�culaire

            radiance = obj.emission +
                       f.multiply(compute_radiance(Ray(x, ray.direction - n * 2.0 * n.dot(ray.direction)), depth));

            return radiance;
        } else if (obj.material == SurfaceType::refraction) {
            // mat�riau avec r�flexion r�fraction

            Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

            bool into = n.dot(na) > 0;

            double ior = 1.5;  // indice de r�fraction du verre
            double nc = 1.0;
            double nt = ior;
            double nnt = into ? nc / nt : nt / nc;
            double ddn = ray.direction.dot(na);
            double cos2t;

            if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0) {
                radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

                return radiance;
            }

            Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

            // effet de fresnel
            double a = nt - nc;
            double b = nt + nc;
            double r0 = a * a / (b * b);
            double c = 1.0 - (into ? -ddn : tdir.dot(n));
            double re = r0 + (1.0 - r0) * c * c * c * c * c;
            double tr = 1 - re;
            double p = 0.25 + 0.5 * re;
            double rp = re / p;
            double tp = tr / (1.0 - p);

            radiance =
                obj.emission + f.multiply(depth > 2 ? (random01(rng) < p ? compute_radiance(reflection_ray, depth) * rp
                                                                         : compute_radiance(Ray(x, tdir), depth) * tp)
                                                    : compute_radiance(reflection_ray, depth) * re +
                                                          compute_radiance(Ray(x, tdir), depth) * tr);

            return radiance;
        } else {
            return radiance;
        }
    }
    return Vector(0, 0, 0);
}

// fonction qui permet de sauvegarder des pixels dans un fichier image (format .ppm)
void save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel) {
    // nom du fichier image de type .ppm (portable pixmap)
    std::stringstream ss;
    ss << "rendu/image" << width << "x" << height << "_" << ray_per_pixel << ".ppm";
    std::string filename = ss.str();

    // d�claration et ouverture du fichier en mode �criture
    std::ofstream file;
    file.open(filename, std::ios::out);

    // ent�te du ficher pour une image avec un espace de couleur RGB 24 bits (P3 pour pixmap)
    file << "P3\n";

    // largeur et hauteur de l'image sur la seconde ligne de l'ent�te
    file << width << ' ' << height << '\n';

    // valeur maximale de l'espace de couleur sur la troisi�me ligne de l'ent�te
    file << "255\n";

    // �criture des pixels dans le fichier image
    for (int index = 0; index < width * height; ++index) {
        // �crire la couleur du pixel dans le fichier image
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].x)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].y)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].z)) << " ";
    }

    // fermeture du fichier
    file.close();

    ofImage dest;
    dest.allocate(width, height, OF_IMAGE_COLOR);
    int i = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int index = y * width + x;
            Vector p = pixel[index];

            dest.setColor(x, y, ofColor(p.x * 255.f, p.y * 255.f, p.z * 255.f));
        }
    }

    dest.save("raytracing.jpg");
}

// fonction d'initialisation de la sc�ne
void init(std::shared_ptr<engine::Scene> scene, std::shared_ptr<engine::CameraController> cameraController) {
    std::cout << "scene setup" << std::endl;

    constexpr double anchor = 1e5;
    constexpr double wall_radius = anchor;

    constexpr double box_size_x = 100.0;
    constexpr double box_size_y = 81.6;
    constexpr double box_size_z = 81.6;

    constexpr double box_x_min = 0.0;
    constexpr double box_x_max = box_size_x;
    constexpr double box_y_min = 0.0;
    constexpr double box_y_max = box_size_y;
    constexpr double box_z_min = 0.0;
    constexpr double box_z_max = box_size_z;

    constexpr double box_center_x = (box_x_max - box_x_min) / 2.0;
    constexpr double box_center_y = (box_y_max - box_y_min) / 2.0;
    constexpr double box_center_z = (box_z_max - box_z_min) / 2.0;

    // vider la sc�ne de son contenu
    sceneSphere.clear();
    scenePlane.clear();

    bool reflexion = true;

    for (auto element : scene->getElements()) {
        if (element->getName() == "Plane") {
            glm::vec3 glmP = element->getAbsolutePosition();
            Vector p = Vector(glmP.x, glmP.y, glmP.z);
            glm::vec3 glmN = element->getAbsoluteRotation().asVec3();
            Vector n = Vector(glmN.x, glmN.y, glmN.z);
            Plane plane = Plane(p, n * 1000000000.0, Vector(), Vector(100, 100, 100), SurfaceType::diffuse);
            scenePlane.push_back(plane);
        }
        if (element->getName() == "Sphere") {
            if (reflexion) {
                glm::vec3 glmP = element->getAbsolutePosition();
                Vector p = Vector(glmP.x, glmP.y, glmP.z);
                double r = (double)element->getAbsoluteScale().r;
                Sphere sphere = Sphere(r, p, Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular);
                sceneSphere.push_back(sphere);
                reflexion = true;
            } else {
                glm::vec3 glmP = element->getAbsolutePosition();
                Vector p = Vector(glmP.x, glmP.y, glmP.z);
                double r = (double)element->getAbsoluteScale().r;
                Sphere sphere = Sphere(r, p, Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction);
                sceneSphere.push_back(sphere);
                reflexion = false;
            }
        }
    }

    sceneSphere.push_back(Sphere(wall_radius,
                                 Vector(box_center_x, -anchor + box_size_y, box_size_z),
                                 Vector(),
                                 Vector(0.75, 0.75, 0.75),
                                 SurfaceType::diffuse));

    sceneSphere.push_back(Sphere(600,
                                 Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z),
                                 Vector(15, 15, 15),
                                 Vector(0.0, 0.0, 0.0),
                                 SurfaceType::diffuse));

    scenePlane.push_back(Plane(Vector(0, 40, 75),
                               Vector(-100000.f, 0.f, 0.f),
                               Vector(),
                               Vector(0.75, 0.75, 0.75),
                               SurfaceType::diffuse));  // mur droit

    scenePlane.push_back(Plane(Vector(box_center_x, anchor - wall_radius, box_size_z),
                               Vector(0.f, 100000.f, 0.f),
                               Vector(),
                               Vector(0.75, 0.75, 0.75),
                               SurfaceType::diffuse));  // sol

    scenePlane.push_back(Plane(Vector(box_center_x, box_center_y, anchor - wall_radius + 20.0),
                               Vector(0.f, 0.f, 100000.f),
                               Vector(),
                               Vector(0.25, 0.75, 0.25),
                               SurfaceType::diffuse));  // fond

    std::cout << box_center_x << " " << box_center_y << " " << box_center_z << std::endl;

    sceneBox.clear();

    sceneBox.push_back(
        Box(Vector(30, 30, 40), Vector(75, 40, 75), Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse));

    // Box(const Vector& min_corner_, const Vector& max_corner_, const Vector& e, const Vector& c, SurfaceType m)
    //     : min_corner(min_corner_), max_corner(max_corner_), emission(e), color(c), material(m) {}

    // allocation de la m�moire de l'image en fonction des param�tres du programme
    image.resize(image_width, image_height);

    std::cout << "image resize to " << image.width << "x" << image.height << " (" << image.count << " pixels, "
              << image.size << " MB)" << std::endl;

    // calibration de la cam�ra
    camera.viewport_width = image.width;
    camera.viewport_height = image.height;
    camera.fov = camera_fov;
    camera.calibrate();
}

// fonction appel�e quand le rendu de la sc�ne est termin�e
void post_render() {
    // �crire les pixels dans un fichier image
    ray_per_pixel = 32;

    save_image_file(image.width, image.height, ray_per_pixel, image.pixel);

    std::cout << "raytracer task is done" << std::endl;
}

// fonction de rendu de la sc�ne
void render() {
    std::cout << "render start" << std::endl;

    unsigned short x = 0;

    int index, y, s, sx, sy = 0;

    float progression = 0.0f;

    double r1, r2 = 0.0;
    double dx, dy = 0.0;

    Vector radiance;

    Vector distance;

    // it�ration sur les rang�es de pixels
    for (y = 0; y < image_height; ++y) {
        // calculer le pourcentage de progression
        progression = 100.0f * y / (image_height - 1.0f);

        // afficher le pourcentage de progression du rendu dans la console
        fprintf(stderr, "\rraytracing (%d rays per pixel) : %4.1f %%", ray_per_pixel, progression);

        // it�ration sur les colonnes de pixels
        for (x = 0; x < image_width; ++x) {
            // d�terminer l'index du pixel
            index = (image_height - y - 1) * image_width + x;

            // it�ration sur les rang�es du bloc de 2x2 �chantillons
            for (sy = 0; sy < 2; ++sy) {
                // it�ration sur les colonnes du bloc de 2x2 �chantillons
                for (sx = 0; sx < 2; ++sx) {
                    // initialiser la radiance
                    radiance = Vector();

                    // it�ration des sur les rayons par pixel
                    for (s = 0; s < ray_per_pixel; ++s) {
                        // filtre de la tente
                        r1 = 2.0 * random01(rng);
                        dx = r1 < 1.0 ? sqrt(r1) - 1.0 : 1.0 - sqrt(2.0 - r1);

                        r2 = 2.0 * random01(rng);
                        dy = r2 < 1.0 ? sqrt(r2) - 1.0 : 1.0 - sqrt(2.0 - r2);

                        // calculer la distance de l'�chantillon
                        distance = camera.axis_x * (((sx + 0.5 + dx) / 2.0 + x) / image_width - 0.5) +
                                   camera.axis_y * (((sy + 0.5 + dy) / 2.0 + y) / image_height - 0.5) + camera.axis_z;

                        // appel r�cursif du calcul de la radiance
                        radiance = radiance +
                                   compute_radiance(Ray(camera.position + distance * 140, distance.normalize()), 0) *
                                       (1.0 / ray_per_pixel);
                    }

                    image.pixel[index] =
                        image.pixel[index] + Vector(clamp(radiance.x), clamp(radiance.y), clamp(radiance.z)) * 0.25;
                }
            }
        }
    }

    std::cout << "\nrender done" << std::endl;
}

// fonction de mise � jour
void update() {
    auto t1 = std::chrono::high_resolution_clock::now();

    // rendu de la sc�ne
    render();

    auto t2 = std::chrono::high_resolution_clock::now();

    // temps de calcul
    auto render_time = t2 - t1;

    std::cout << "render time : " << std::chrono::duration_cast<std::chrono::seconds>(render_time).count() << "s"
              << std::endl;
}

void rayTracing(std::shared_ptr<engine::Scene> scene, std::shared_ptr<engine::CameraController> cameraController) {
    std::cout << "raytracer start" << std::endl;

    image_width = 256;
    image_height = 256;
    ray_per_pixel = 16;

    std::cout << "program init done" << std::endl;
    Box boxTest = Box(Vector(0, 0, 0), Vector(100, 100, 100), Vector(), Vector(100, 100, 100), SurfaceType::diffuse);
    std::cout << boxTest.intersect(Ray(Vector(100, 1, 1), Vector(1, 1, 1))) << std::endl;
    std::cout << boxTest.intersect(Ray(Vector(100, 1, 1), Vector(1, 1, 1))) << std::endl;
    // initialisation de la sc�ne
    init(scene, cameraController);

    // faire un rendu de la sc�ne par lancer de rayons
    update();

    // proc�dure post-rendu (sauvegarde de l'image et d�sallocation de la m�moire)
    post_render();
}
