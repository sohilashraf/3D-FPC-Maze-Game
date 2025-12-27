#version 330 core
in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 outColor;

// Lighting uniforms
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// Texture uniform
uniform sampler2D textureSampler;
uniform int useTexture;  // Changed from bool to int

void main()
{
    // Base color from texture or default color
    vec3 baseColor;
    if (useTexture == 1)
    {
        baseColor = texture(textureSampler, fragTexCoord).rgb;
    }
    else
    {
        baseColor = vec3(0.8, 0.8, 0.8);  // Default gray color
    }
    
    // Normalize the normal vector
    vec3 norm = normalize(fragNormal);
    
    // Calculate light direction
    vec3 lightDir = normalize(lightPos - fragPos);
    
    // Calculate view direction
    vec3 viewDir = normalize(viewPos - fragPos);
    
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor * baseColor;
    
    // Diffuse lighting (Lambertian reflectance)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;
    
    // Specular lighting (Blinn-Phong)
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine all lighting components
    vec3 result = ambient + diffuse + specular;
    
    outColor = vec4(result, 1.0);
}