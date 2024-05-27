#version 330

#define MAX_NB_LIGHTS 4 

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

// attributs interpolés à partir des valeurs en sortie du shader de sommet (espace tangent)
in vec3 surface_position;
in vec2 surface_texcoord;
in vec3 lights_ts_positions[MAX_NB_LIGHTS];

// attribut en sortie
out vec4 fragment_color;

uniform Light lights[MAX_NB_LIGHTS];

// couleurs de réflexion du matériau
uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform float mat_shininess;

uniform int has_tex0;
uniform sampler2D tex0;

uniform int has_tex_ambient;
uniform sampler2D tex_ambient;

uniform int has_tex_specular;
uniform sampler2D tex_specular;

uniform int has_tex_normal;
uniform sampler2D tex_normal;

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
    vec3 normal = texture(tex_normal,surface_texcoord).rgb;    

    // passer du range de [0,1] à [-1,1]
    vec3 n=normalize(normal * 2.0 - 1.0);

    // calculer la direction de la surface vers la caméra (v)
    vec3 v=normalize(-surface_position);

    vec4 output_color = vec4(0., 0., 0., 1.);
    
    for (int i = 0; i < MAX_NB_LIGHTS; ++i) {
        if (lights[i].enabled == 0 || lights[i].lightType != 0)
            continue;

        // calculer la direction de la surface vers la lumière (l)
        vec3 l=normalize(lights_ts_positions[i]-surface_position);

        vec4 ambient = get_material_ambient() * lights[0].ambient;

        // calculer le niveau de réflexion diffuse (n • l)
        float diffuse_reflection=max(dot(n,l),0.);
        
        vec4 diffuse = diffuse_reflection * get_material_diffuse() * lights[0].diffuse;

        // réflexion spéculaire par défaut
        vec4 specular=vec4(0., 0., 0., 1.);

        // calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
        if(diffuse_reflection>0.)
        {
            // calculer la direction du demi-vecteur de réflection (h) en fonction du vecteur de vue (v) et de lumière (l)
            vec3 h=normalize(v+l);
            
            // calculer le niveau de réflexion spéculaire (n • h)
            float specular_reflection=pow(max(dot(n,h),0.),mat_shininess);

            specular = specular_reflection * get_material_specular() * lights[0].specular;
        }

        output_color += (ambient + diffuse + specular);
    }
    

    fragment_color=output_color;
}