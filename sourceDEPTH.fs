#version 330 core
out vec4 FragColor;

uniform vec3 objColor;
uniform float kd;
uniform float ka;
uniform float ks;
uniform float ns;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;

float near = 0.1; 
float far  = 20.0;

float linearDepth(float depth) 
{   
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
   float depth = linearDepth(gl_FragCoord.z) / far;
   FragColor = vec4(vec3(depth), 1.0);
}