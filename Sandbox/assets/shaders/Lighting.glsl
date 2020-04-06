#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_Position;
out vec2 v_TexCoords;

void main()
{
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
  v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
  v_Normal = a_Normal;
  v_TexCoords = a_TexCoords;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoords;

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light
{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform Light u_Light;

// TO-DO: Implement this in view space instead of world space.
void main()
{
  // Ambient light
  vec3 ambient = vec3(texture(u_Material.diffuse, v_TexCoords)) * u_Light.ambient;

  // Diffuse light
  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(u_Light.position - v_Position);
  vec3 diffuse = vec3(texture(u_Material.diffuse, v_TexCoords)) * max(dot(normal, lightDirection), 0.0f) * u_Light.diffuse;

  // Specular light
  vec3 cameraDirection = normalize(u_CameraPosition - v_Position);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  vec3 specular = vec3(texture(u_Material.specular, v_TexCoords)) * pow(max(dot(cameraDirection, reflectDirection), 0.0f), u_Material.shininess) * u_Light.specular;

  // Compute final color
  color = vec4(ambient + diffuse + specular, 1.0f);
}
