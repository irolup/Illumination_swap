#version 330

// attribut interpolé à partir des valeurs en sortie du shader de sommet
in vec3 surface_color;
in vec2 TexCoord;


// attribut en sortie
out vec4 fragment_color;

uniform sampler2D texture_diffuse; // The texture sampler



void main()
{
  // texture diffuse
  vec4 tex_color = texture(texture_diffuse, TexCoord);

  // calculer la couleur du fragment
  fragment_color = vec4(surface_color, 1.0) * tex_color;
}
