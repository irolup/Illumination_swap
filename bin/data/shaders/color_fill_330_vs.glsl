#version 330

// attribut de sommet
in vec4 position;
in vec2 texcoord;

out vec2 TexCoord;

// attributs uniformes
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main()
{
  // attribuer les coordonnées de texture
  TexCoord = texcoord;
  // transformation de la position du sommet par les matrices de modèle, vue et projection
  gl_Position = projectionMatrix * modelViewMatrix * position;
}
