#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoord;

void main()
{
  v_TexCoord = a_Position;
  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
  color = texture(u_Skybox, v_TexCoord);
  // color = vec4(v_TexCoord + 0.5f, 1.0f);
}
