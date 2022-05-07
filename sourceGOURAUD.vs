#version 330 core
in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aNormal;

out vec3 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 objColor;
uniform float kd;
uniform float ka;
uniform float ks;
uniform float ns;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;

void main()
{  
   vec3 vert = vec3(model*vec4(aPos, 1.0));
   vec3 norm = normalize(vec3(model*vec4(aNormal,1.0)));

   vec3 lightDir = normalize(lightPosition - vert);   
   vec3 eyeDir = normalize(vert);
   vec3 refDir = normalize(-reflect(lightDir,norm)); 
   
   vec3 ambient = ka * lightColor;

   float diff = kd * lightIntensity * max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float spec = ks * lightIntensity * pow(max(dot(refDir,eyeDir), 0.0), ns);
   vec3 specular = spec * lightColor;  

   color = (ambient + specular + diffuse)*objColor;

   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}