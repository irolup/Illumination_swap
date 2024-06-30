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
uniform float brightness;
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

vec3 PerturbNormal(in vec3 N, in vec3 V, in vec2 texcoord) {

    vec3 map = texture(texture_normal, texcoord).rgb;

    map = map * 2.0 - 1.0;


    mat3 TBN = CotangentFrame(N, -V, texcoord);
    return normalize(TBN * map);
}

void main()
{

    vec3 V = normalize(surface_position);

    vec3 n = surface_normal;
    if (perturb_normal) {
        n = PerturbNormal(normalize(surface_normal), normalize(surface_position), TexCoord);
    } else {
        n = normalize(surface_normal);
    }

    vec3 l = normalize(light_position - surface_position);

    float reflection_diffuse = max(dot(n, l), 0.0);

    float reflection_specular = 0.0;

    //calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
    if (reflection_diffuse > 0.0) {
        vec3 v = normalize(-surface_position);
        vec3 r = reflect(-l, n);
        reflection_specular = pow(max(dot(v, r), 0.0), brightness);
    }

    vec4 tex_color = texture(texture_diffuse, TexCoord);
    //vec3 diffuseColor = reflection_diffuse * texture(texture_diffuse, TexCoord).rgb;

    // calculer la couleur du fragment
    fragment_color = vec4(
        color_ambient +
        color_diffuse * reflection_diffuse +
        color_specular * reflection_specular, 1.0) * tex_color;
}