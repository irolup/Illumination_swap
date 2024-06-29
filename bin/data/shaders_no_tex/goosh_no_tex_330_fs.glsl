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

void main()
{
    // re-normaliser la normale après interpolation
    vec3 n = normalize(surface_normal);

    // calculer la direction de la surface vers la lumière (l)
    vec3 l = normalize(light_position - surface_position);

    // calculer le niveau de réflexion diffuse (n • l)
    float reflection_diffuse = max(dot(n, l), 0.0);

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

    // Effet "Goosh": ajout de perturbations et de teintes personnalisées
    vec3 goosh_color = vec3(0.2, 0.5, 0.7); // teinte personnalisée pour l'effet "Goosh"
    
    // ajout d'une perturbation en fonction des coordonnées de texture synthétiques
    float perturbation = sin(TexCoord.x * 10.0) * cos(TexCoord.y * 10.0);

    // calculer la couleur finale avec l'effet de perturbation
    vec3 final_color = color_ambient +
                       color_diffuse * reflection_diffuse +
                       color_specular * reflection_specular +
                       goosh_color * perturbation;

    fragment_color = vec4(final_color, 1.0);
}