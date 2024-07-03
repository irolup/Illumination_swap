#version 330

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;
in vec2 TexCoord;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// facteur de brillance spéculaire du matériau
uniform float brightness;

// position d'une source de lumière
uniform vec3 light_position;

// texture diffuse
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

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

void main()
{
    // re-normaliser la normale après interpolation
    vec3 n = normalize(surface_normal);

    // calculer la direction de la surface vers la lumière (l)
    vec3 l = normalize(light_position - surface_position);

    if (perturb_normal) {
        n = PerturbNormal(n, normalize(surface_position), TexCoord);
    } else {
        n = normalize(surface_normal);
    }

    // calculer le niveau de réflexion diffuse (n • l)
    float reflection_diffuse = max(dot(n, l), 0.0);

    // quantification de la réflexion diffuse pour des bandes de couleurs franches
    if (reflection_diffuse > 0.5) reflection_diffuse = 1.0;
    else if (reflection_diffuse > 0.2) reflection_diffuse = 0.5;
    else reflection_diffuse = 0.0;

    // réflexion spéculaire par défaut
    float reflection_specular = 0.0;

    // calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
    if (reflection_diffuse > 0.0)
    {
        // calculer la direction de la surface vers la caméra (v)
        vec3 v = normalize(-surface_position);

        // calculer la direction de la réflection (r) du rayon incident (-l) en fonction de la normale (n)
        vec3 r = reflect(-l, n);

        // calculer le niveau de réflexion spéculaire (r • v)
        reflection_specular = pow(max(dot(v, r), 0.0), brightness);
    }

    // calculer la couleur du fragment
    vec4 tex_color = texture(texture_diffuse, TexCoord);
    vec3 final_color = color_ambient +
                       color_diffuse * reflection_diffuse +
                       color_specular * reflection_specular;

    fragment_color = vec4(final_color, 1.0) * tex_color;
}