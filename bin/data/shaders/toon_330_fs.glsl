#version 330
// Source https://roystan.net/articles/toon-shader/

#define MAX_NB_LIGHTS 32

struct Light {
	float enabled; // 0 if disabled, 1 if enabled
	float lightType; // Point = 0, Directional = 1, Spot = 2, Area = 3
	vec4 position;
	vec4 ambient; // Color
	vec4 diffuse; // Color
	vec4 specular; // Color
	// != Directional
	float radius;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

// attributs interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;

uniform sampler2D diffuse_texture;

uniform float rim_amount;
uniform vec4 mat_rim;
uniform float rim_threshold;

// facteur de brillance spéculaire du matériau
uniform float mat_shininess;

// position d'une source de lumière
uniform Light lights[MAX_NB_LIGHTS];

uniform int has_tex0;
uniform sampler2D tex0;

uniform int has_tex_ambient;
uniform sampler2D tex_ambient;

uniform int has_tex_specular;
uniform sampler2D tex_specular;

vec4 get_material_ambient() {
  if (has_tex_ambient == 1) {
    return texture(tex_ambient, surface_texcoord).rgba;
  }
  return mat_ambient;
}

vec4 get_material_diffuse() {
  if (has_tex0 == 1) {
    return texture(tex0, surface_texcoord).rgba;
  }
  return mat_diffuse;
}

vec4 get_material_specular() {
  if (has_tex_specular == 1) {
    return texture(tex_specular, surface_texcoord).rgba;
  }
  return mat_specular;
}

void main()
{  
    // re-normaliser la normale après interpolation
    vec3 n=normalize(surface_normal);

    // calculer la direction de la surface vers la caméra (v)
    vec3 v=normalize(-surface_position);

    vec4 output_color=vec4(0., 0., 0., 1.);

    for (int i = 0; i < MAX_NB_LIGHTS; ++i) {
        if (lights[i].enabled < 0.5 || lights[i].lightType != 0)
            continue;

        vec3 light_position = vec3(lights[i].position);
        // calculer la direction de la surface vers la lumière (l)
        vec3 l=normalize(light_position-surface_position);

        vec4 ambient_color = get_material_ambient() * lights[i].ambient;

        // calculer le niveau de réflexion diffuse (n • l)
        float diffuse_reflection=smoothstep(0., 0.01, dot(n,l));

        vec4 diffuse_color = diffuse_reflection * get_material_diffuse() * lights[i].diffuse;

        // couleur spéculaire par défaut
        vec4 specular_color = vec4(0., 0., 0., 1.);

        if(diffuse_reflection > 0.) {
            // calculer la direction du demi-vecteur de réflection (h) en fonction du vecteur de vue (v) et de lumière (l)
            vec3 h=normalize(v+l);

            float specular_reflection=pow(max(dot(n,h),0.) * diffuse_reflection, mat_shininess);        
            specular_reflection = smoothstep(0.005, 0.01, specular_reflection);
        
            specular_color = specular_reflection * get_material_specular() * lights[i].specular;
        }

        float rim_intensity = (1 - dot(n, v)) * pow(dot(n,l), rim_threshold);
        rim_intensity = smoothstep(rim_amount - 0.01, rim_amount + 0.01, rim_intensity);

        vec4 rim_color = rim_intensity * mat_rim;

        output_color += ambient_color + diffuse_color + specular_color + rim_color;
    }

    fragment_color=output_color;
}