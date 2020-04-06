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

void main()
{
  // Ambient light
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * u_LightColor;

  // Diffuse light
  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(u_LightPosition - v_Position);
  vec3 diffuse = max(dot(normal, lightDirection), 0.0f) * u_LightColor;

  // Compute final color
  color = vec4((ambient + diffuse) * u_Color, 1.0f);
}
