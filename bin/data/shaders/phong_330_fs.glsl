#version 330

// interpolated attributes from the vertex shader
in vec3 surface_position;
in vec3 surface_normal;
in vec2 TexCoord;

// output attribute
out vec4 fragment_color;

// material reflection colors
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// specular brightness factor of the material
uniform float brightness;

// light source position
uniform vec3 light_position;

// textures
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_displacement;

uniform bool parallax_active;
uniform bool perturb_normal;

mat3 CotangentFrame(in vec3 N, in vec3 p, in vec2 uv) {
  vec3 dp1 = dFdx(p);
  vec3 dp2 = dFdy(p);
  vec2 duv1 = dFdx(uv);
  vec2 duv2 = dFdy(uv);

  vec3 dp2perp = cross(dp2, N);
  vec3 dp1perp = cross(N, dp1);
  vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
  return mat3(T * invmax, B * invmax, N);
}

vec3 PerturbNormal(in vec3 N, in vec3 V, in vec2 TexCoord) {
  vec3 map = texture(texture_normal, TexCoord).rgb;
  map = map * 2.0 - 1.0;

  mat3 TBN = CotangentFrame(N, -V, TexCoord);
  return normalize(TBN * map);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
  const float minLayers = 8;
  const float maxLayers = 32;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
  vec2 P = viewDir.xy / viewDir.z * 0.1; // parallax scale peut mettre variable pour controle
  vec2 deltaTexCoords = P / numLayers;

  vec2 currentTexCoords = texCoords;
  float currentDepthMapValue = texture(texture_displacement, currentTexCoords).r;

  while (currentLayerDepth < currentDepthMapValue) {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = texture(texture_displacement, currentTexCoords).r;
    currentLayerDepth += layerDepth;
  }

  // Get texture coordinates before collision (reverse operations)
  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

  // Get depth after and before collision for linear interpolation
  float afterDepth  = currentDepthMapValue - currentLayerDepth;
  float beforeDepth = texture(texture_displacement, prevTexCoords).r - currentLayerDepth + layerDepth;

  // Interpolation of texture coordinates
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}

void main() {
  // normalize the normal after interpolation
  vec3 n = normalize(surface_normal);
  vec3 viewDir = normalize(-surface_position);

  vec2 texCoords = TexCoord;
  if (parallax_active) {
    texCoords = ParallaxMapping(texCoords, viewDir);
  }

  if (perturb_normal) {
    n = PerturbNormal(n, viewDir, texCoords);
  } else {
    n = normalize(surface_normal);
  }

  // calculate the direction from the surface to the light
  vec3 l = normalize(light_position - surface_position);

  // calculate the diffuse reflection level (n • l)
  float reflection_diffuse = max(dot(n, l), 0.0);

  // default specular reflection
  float reflection_specular = 0.0;

  // calculate the specular reflection only if there is diffuse reflection
  if (reflection_diffuse > 0.0) {
    vec3 r = reflect(-l, n);
    reflection_specular = pow(max(dot(viewDir, r), 0.0), brightness);
  }

  vec4 tex_color = texture(texture_diffuse, texCoords);

  // calculate the fragment color
  fragment_color = vec4(
    color_ambient +
    color_diffuse * reflection_diffuse +
    color_specular * reflection_specular, 1.0) * tex_color;
}