#version 330

// attribut interpolé à partir des valeurs en sortie du shader de sommet
in vec3 surface_color;
in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragNormal;

// attribut en sortie
out vec4 fragment_color;

uniform sampler2D texture_diffuse; // The texture sampler
uniform sampler2D texture_normal; // The normal map sampler
uniform sampler2D texture_displacement;

uniform bool parallax_active;

uniform bool perturb_normal;

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

void main()
{
  vec3 n;
  vec3 viewDir = normalize(-fragPos);
   vec2 texCoords = TexCoord;

  if (parallax_active) {
    texCoords = ParallaxMapping(texCoords, viewDir);
  }

  if (perturb_normal) {
    n = PerturbNormal(fragNormal, normalize(fragPos), texCoords);
  } else {
    n = normalize(fragNormal);
  }

  // texture diffuse
  vec4 tex_color = texture(texture_diffuse, texCoords);

  vec3 adjusted_surface_color = surface_color * dot(n, normalize(-fragPos));

  // calculer la couleur du fragment
  fragment_color = vec4(adjusted_surface_color, 1.0) * tex_color;
}
