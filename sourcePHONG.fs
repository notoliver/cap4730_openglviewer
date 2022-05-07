#version 330 core
in vec3 norm;
in vec3 vert; 

out vec4 FragColor;

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
   vec3 lightDir = normalize(lightPosition - vert);   
   vec3 eyeDir = normalize(vert);
   vec3 refDir = normalize(-reflect(lightDir,norm)); 
   
   vec3 ambient = ka * lightColor;

   float diff = kd * lightIntensity * max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float spec = ks * lightIntensity * pow(max(dot(refDir,eyeDir), 0.0), ns);
   vec3 specular = spec * lightColor;  

   vec3 color = (ambient + specular + diffuse)*objColor;

   FragColor = vec4(color, 1.0);
}