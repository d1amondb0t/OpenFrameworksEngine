// IFT3100H24 ~ gouraud_330_fs.glsl

#version 330

// attribut interpolé à partir des valeurs en sortie du shader de sommet
in vec4 surface_color;

// attribut en sortie
out vec4 fragment_color;

void main()
{
  // calculer la couleur du fragment
  fragment_color = surface_color;
}