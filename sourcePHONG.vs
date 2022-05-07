#version 330 core
in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aNormal;

out vec3 color;
out vec3 norm;
out vec3 vert;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{  
   vert = vec3(model*vec4(aPos, 1.0));
   norm = normalize(vec3(model*vec4(aNormal,1.0)));

   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}