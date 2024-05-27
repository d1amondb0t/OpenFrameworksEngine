// IFT3100H24 ~ pbr_330_fs.glsl

#version 330

#define MAX_NB_LIGHTS 32

struct Light {
	float enabled; // 0 if disabled, 1 if enabled
	float lightType; // Point = 0, Directional = 1, Spot = 2, Area = 3
	vec4 position; // World space if PBR, otherwise view space
	vec4 ambient; // Color
	vec4 diffuse; // Color
	vec4 specular; // Color
	// != Directional
	float radius;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const float PI = 3.1415926535897932384626433832795;

const float light_attenuation_factor_constant = 0.01;
const float light_attenuation_factor_linear = 0.000001;
const float light_attenuation_factor_quadratic = 0.0000001;

// attributs de surface interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

// position de la camera dans l'espace de monde
uniform vec3 uCameraPos;

// couleurs de réflexion du matériau
uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;

// facteur de brillance du matériau
uniform float mat_shininess;

// facteur de métallicité du matériau
uniform float mat_metallic;

// facteur de rugosité du matériau
uniform float mat_roughness;

// facteur d'occlusion ambiante
uniform float mat_occlusion;

// indice de réfraction de l'effet de Fresnel;
uniform vec3 mat_fresnel_ior;

// facteur d'exposition
uniform float tone_mapping_exposure;

// mode de mappage tonal (Reinhard ou ACES filmic)
uniform float tone_mapping_toggle;

// facteur gamma
uniform float tone_mapping_gamma;

// texture diffuse
uniform sampler2D tex0;

// texture métallique
uniform sampler2D tex_metallic;

// texture de rugosité
uniform sampler2D tex_roughness;

// texture d'occlusion ambiante
uniform sampler2D tex_occlusion;

// couleur de la source de lumière
uniform vec3 light_color;

// intensité de la source de lumière
uniform float light_intensity;

// nombre de lumières
uniform Light lights[MAX_NB_LIGHTS];

// fonction de distribution des microfacettes (Trowbridge-Reitz)
float trowbridge_reitz(vec3 n, vec3 h, float roughness) 
{
	float a = roughness * roughness;
	float a2 = a * a;
	float ndh = max(dot(n, h), 0.0);
	float ndh2 = ndh * ndh;
	float numerator = a2;
	float denominator = (ndh2 * (a2 - 1.0) + 1.0);
	denominator = denominator * denominator * PI;
	return numerator / denominator;
}

// fonction géométrique pour calculer l'impact de l'occlusion et de l'ombrage des microfacettes (Schlick-Beckmann)
float schlick_beckmann(float costheta, float roughness) 
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;
	float numerator = costheta;
	float denominator = costheta * (1.0 - k) + k;
	return numerator / denominator;
}

// fonction géométrique avec occlusion et ombrage combinés (méthode de Smith)
float smith(vec3 n, vec3 l, vec3 v, float roughness) 
{
	float ndl = max(dot(n, l), 0.0);
	float ndv = max(dot(n, v), 0.0);
	float shadow = schlick_beckmann(ndl, roughness);
	float occlusion = schlick_beckmann(ndv, roughness);
	return shadow * occlusion;
}

// fonction qui calcul l'effet de Fresnel
vec3 schlick_fresnel(float costheta, vec3 f0) 
{
	return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

// mappage tonal de la couleur HDR vers LDR (Reinhard tone mapping)
vec3 tone_mapping_reinhard(vec3 x) 
{
	return x / (x + vec3(1.0));
}

// mappage tonal de la couleur HDR vers LDR (approximation de la courbe du ACES filmic tone mapping)
vec3 tone_mapping_aces_filmic(vec3 x) 
{
  float a = 2.51f;
  float b = 0.03f;
  float c = 2.43f;
  float d = 0.59f;
  float e = 0.14f;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// fonction qui calcule un modèle d'illumination de type pbr avec brdf de cook-torrance
vec3 brdf_cook_torrance() 
{
  	// re-normaliser la normale après interpolation
	vec3 n = normalize(surface_normal);

	// calculer la direction de la surface vers la caméra (v)
	vec3 v = normalize(uCameraPos-surface_position);

	// échantillonage de la texture diffuse
	vec4 texture_sample_diffuse = texture(tex0, surface_texcoord).rgba;

	// conversion de l'échantillon de la texture diffuse de l'espace gamma vers l'espace linéaire
	texture_sample_diffuse = pow(texture_sample_diffuse, vec4(tone_mapping_gamma));

	// échantillonage de la texture de métallicité
	float texture_sample_metallic = texture(tex_metallic, surface_texcoord).r;

	// échantillonage de la texture de rugosité
	float texture_sample_roughness = texture(tex_roughness, surface_texcoord).r;

	// échantillonage de la texture d'occlusion
	float texture_sample_occlusion = texture(tex_occlusion, surface_texcoord).r;

	// facteurs du matériau combinées avec les échantillons de couleur
	float metallic = mat_metallic * texture_sample_metallic;
	float roughness = mat_roughness * texture_sample_roughness;
	float occlusion = mat_occlusion * texture_sample_occlusion;

	// combiner l'échantillon de la texture diffuse avec la couleur diffuse du matériau
	vec4 albedo = mat_diffuse * texture_sample_diffuse;

	// calculer la réflexion ambiante
	vec4 ambient = mat_ambient * albedo * occlusion;

	vec4 output_color = vec4(0., 0., 0., 1.);

	for(int i = 0; i < MAX_NB_LIGHTS; ++i) {
		if(lights[i].enabled < 0.5 || lights[i].lightType != 0)
			continue;

		vec3 light_position = vec3(lights[i].position);

	  	// calculer la direction de la surface vers la lumière (l)
		vec3 l = normalize(light_position - surface_position);

	  	// calculer la direction du demi-vecteur de réflection (h) en fonction du vecteur de lumière (l) et de vue (v)
		vec3 h = normalize(l + v);

	  	// distance entre la position de la lumière et de la surface
		float light_distance = length(light_position - surface_position);

	  	// calculer l'atténuation de l'intensité de la lumière en fonction de la distance
		float light_attenuation = 1.0 / (light_attenuation_factor_constant + light_attenuation_factor_linear * light_distance + light_attenuation_factor_quadratic * (light_distance * light_distance));

	  	// calculer la radiance de la lumière
		vec3 radiance = light_color * light_attenuation * light_intensity;

	  	// calculer le niveau de réflexion diffuse (n • l)
		float diffuse_reflection = max(dot(n, l), 0.0);

	  	// calculer la distribution des microfacettes
		float d = trowbridge_reitz(n, h, roughness);

	  	// calculer la fonction géométrique
		float g = smith(n, l, v, roughness);

	  	// reflexion de la surface avec un angle d'incidence nul
		vec3 f0 = mat_fresnel_ior;

	  	// moduler l'effet de Fresnel ave la couleur diffuse en fonction du facteur de métallicité
		f0 = mix(f0, vec3(albedo), metallic);

	  	// calculer l'effet de Fresnel
		vec3 f = schlick_fresnel(max(dot(h, v), 0.0), f0);

	  	// calculer le numérateur de l'équation (produit des fonctions d, f et g)
		vec3 coor_torrance_numerator = d * f * g;

	  	// calculer le dénominateur de l'équation (facteur de normalisation)
		float coor_torrance_denominator = 4.0 * max(dot(n, v), 0.0) * diffuse_reflection;

	  	// calculer le résultat de l'équation avec le numérateur et de dénominateur
		vec3 specular = coor_torrance_numerator / max(coor_torrance_denominator, 0.001);

	  	// mixer avec la couleur spéculaire du matériau
		vec4 specular_color = vec4(specular, 1.) * mat_specular;

	  	// calculer le ratio de réflection de la lumière à partir de l'effet de Fresnel (contribution spéculaire)
		vec3 ks = f;

	  	// calculer le ratio de réfraction (contribution diffuse) proportionnelement à la contribution spéculaire
		vec3 kd = vec3(1.0) - ks;

	  	// pondérer la contribution diffuse en fonction du niveau de métallicité de la surface
		kd *= 1.0 - metallic;

	  	// calculer la réflectance de la fonction BRDF de Cook-Torrance
		vec4 reflectance = (vec4(kd, 1.) * albedo / PI + specular_color) * vec4(radiance, 1.) * diffuse_reflection;

	  	// mixer la couleur des composantes de réflexion
		vec4 color = (ambient + reflectance) * mat_shininess;

		output_color += color;
	}

  	// retourner la couleur
	return output_color.rgb;
}

void main() {
  	// évaluation du modèle d'illumination
	vec3 color = brdf_cook_torrance();

  	// ajustement de la couleur en fonction du facteur d'exposition
	color = vec3(1.0) - exp(-color * tone_mapping_exposure);

  	// mappage tonal de la couleur HDR vers LDR
  	if (tone_mapping_toggle > 0.5)
		color = tone_mapping_aces_filmic(color);
	else
		color = tone_mapping_reinhard(color);

  	// conversion de couleur de l'espace linéaire vers l'espace gamma
	color = pow(color, vec3(1.0 / tone_mapping_gamma));

  	// assigner la couleur final du fragment dans un attribut en sortie
    fragment_color = vec4(color, 1.0);
}