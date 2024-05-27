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

// attributs de sommet
in vec4 position;
in vec4 normal;
in vec2 texcoord;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;

// attributs en sortie
out vec3 surface_position;
out vec2 surface_texcoord;

out vec3 lights_ts_positions[MAX_NB_LIGHTS];

uniform Light lights[MAX_NB_LIGHTS];

// attributs uniformes
uniform mat4x4 modelMatrix;
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;
uniform mat4x4 viewMatrix;

void main()
{
    // calculer la matrice de changement vers l'espace tangent
    vec3 T = normalize(vec3(modelViewMatrix * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(modelViewMatrix * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(modelViewMatrix * normal));
    mat3x3 TBN = transpose(mat3x3(T, B, N));
    
    // transformation de la position du sommet dans l'espace tangent
    surface_position=TBN*vec3(modelViewMatrix*position);

    surface_texcoord=texcoord;

    // transformation de la position de la lumière dans l'espace tangent
    for (int i = 0; i < MAX_NB_LIGHTS; ++i) {
        if (lights[i].enabled == 0)
            continue;

        lights_ts_positions[i] = TBN*vec3(lights[i].position);
    }
    
    // transformation de la position du sommet par les matrices de modèle, vue et projection
    gl_Position=projectionMatrix*modelViewMatrix*position;
}