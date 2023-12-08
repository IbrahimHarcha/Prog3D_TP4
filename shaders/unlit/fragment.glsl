#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D bumpTexture;
uniform sampler2D normalTexture;

uniform vec3 lightPosition;

void main() {
    // FragColor = color;
    // FragColor = texture(colorTexture, o_uv0) * color;
    // DEBUG: position
    // FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    // FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);

    // Bump map
    vec3 normal = normalize(o_normalWorld);
    float bump = texture(bumpTexture, o_uv0).r; // on récupere la composante rouge car suffisant quand c'est en niveau de gris...
    normal = normalize(normal + vec3(bump, bump, bump)); // on ajoute le facteur de relief (bump) à chaque composante de la normale...

    vec3 lightDir = normalize(lightPosition - o_positionWorld);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * color.rgb;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color.rgb;

    // Specular
    float specularStrength = 0.5;
    float shininess = 32.0;
    vec3 viewDir = normalize(-lightDir);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * color.rgb;

    // Combine texture color with Phong lighting
    vec4 texColor = texture(colorTexture, o_uv0);
    vec3 finalColor = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(finalColor, 1.0);
    
}

