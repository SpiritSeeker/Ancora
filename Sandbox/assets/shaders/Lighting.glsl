#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_Position;

void main()
{
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
  v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
  v_Normal = a_Normal;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_Position;

uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

// TO-DO: Implement this in view space instead of world space.
void main()
{
  // Ambient light
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * u_LightColor;

  // Diffuse light
  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(u_LightPosition - v_Position);
  vec3 diffuse = max(dot(normal, lightDirection), 0.0f) * u_LightColor;

  // Specular light
  float specularStrength = 0.5f;
  vec3 cameraDirection = normalize(u_CameraPosition - v_Position);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  vec3 specular = specularStrength * pow(max(dot(cameraDirection, reflectDirection), 0.0f), 32) * u_LightColor;

  // Compute final color
  color = vec4((ambient + diffuse + specular) * u_Color, 1.0f);
}
