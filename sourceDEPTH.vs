#version 330 core
in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aNormal;

out vec3 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{  
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}