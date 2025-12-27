#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;      // Changed from location 3
layout(location = 2) in vec2 texCoord;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main()
{
    // Transform position to world space
    vec4 worldPos = model_mat * vec4(position, 1.0);
    fragPos = worldPos.xyz;
    
    // Transform normal to world space
    mat3 normalMatrix = transpose(inverse(mat3(model_mat)));
    fragNormal = normalize(normalMatrix * normal);
    
    // Pass through texture coordinates
    fragTexCoord = texCoord;
    
    // Calculate final vertex position
    gl_Position = proj_mat * view_mat * worldPos;
}