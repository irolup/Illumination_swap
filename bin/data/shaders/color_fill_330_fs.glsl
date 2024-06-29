#version 330

// couleur de remplissage du matériau
uniform vec3 color;
uniform sampler2D texture_diffuse;

in vec2 TexCoord;

out vec4 fragment_color;

void main()
{
  vec4 tex_color = texture(texture_diffuse, TexCoord);

  // déterminer la couleur du fragment
  fragment_color = vec4(color, 1.0) * tex_color;
}
