// #version 400 core

// in vec3 FragPos;
// in vec3 Normal;
// in vec2 TexCoord;
// // in vec4 MatColor;

// out vec4 color;

// vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
// vec3 lightPos = vec3(3.0f, 3.0f, 3.0f);
// vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

// uniform vec3 viewPos;

// uniform vec3 diffuse;
// uniform vec3 ambient;
// uniform vec3 specular;
// uniform vec3 emissive;
// uniform float shininess;
// uniform int texCount;

// uniform sampler2D texUnit;

// void main()
// {

// 	// Ambient
//     // float ambientStrength = 0.2f;
//     // vec3 ambient = ambientStrength * lightColor;

//     // // Diffuse
//     // vec3 norm = normalize(Normal);
//     // vec3 lightDir = normalize(lightPos - FragPos);
//     // float diff = max(dot(norm, lightDir), 0.0);
//     // vec3 diffuse = diff * lightColor;

//     // // Specular
//     // float specularStrength = 1.64f;
//     // vec3 viewDir = normalize(viewPos - FragPos);
//     // vec3 reflectDir = reflect(-lightDir, norm);
//     // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
//     // vec3 specular = specularStrength * spec * lightColor;

//     // vec3 result = (ambient + diffuse + specular) * objectColor;

//     // vec4 phongColor = vec4(result, 1.0f);

//     // color = phongColor;

//     color = texture2D(texUnit, TexCoord);
// }

#version 400 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

// uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

uniform samplerCube skybox;

uniform vec3 viewPos;

out vec4 color;

void main()
{
    // Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
    // Reflection
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    float reflect_intensity = texture(texture_reflection1, TexCoords).r;
    vec4 reflect_color = texture(skybox, R) * reflect_intensity;
    color = diffuse_color + reflect_color;
}
