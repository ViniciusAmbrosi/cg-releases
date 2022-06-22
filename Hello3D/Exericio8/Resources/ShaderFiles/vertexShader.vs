#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 3) in vec3 kaInput;
layout (location = 4) in vec3 ksInput;
layout (location = 5) in vec3 kdInput;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 scaledNormal;
out vec3 fragPos;
out vec2 TexCoords;

out vec3 ka;
out vec3 ks;
out vec3 kd;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    scaledNormal = normal; //mat3(transpose(inverse(model))) * normal;   
    fragPos = vec3(model * vec4(position, 1.0f));

    TexCoords = aTexCoords;

    ka = kaInput;
    ks = ksInput;
    kd = kdInput;
}