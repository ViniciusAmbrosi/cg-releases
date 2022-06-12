#version 450 core

in vec3 scaledNormal; //vetor normal com corre��o de escala
in vec3 fragPos; //posi��o em coord de mundo do objeto

uniform vec3 lightPos;      //posi�ao da fonte de luz 
uniform vec3 viewPos;       //posicao da camera
uniform vec3 lightColor;    //cor da fonte de luz
uniform vec3 finalColor;    //cor do objeto

uniform float ka;           //coeficiente de reflex�o ambiente
uniform float kd;           //coeficiente de reflex�o difuse
uniform float ks;           //coeficiente de reflex�o especular
uniform float n;            //expoente da reflex�o especular

out vec4 color;

void main()
{
    // Ambient
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * lightColor * ka;
  	
    // Diffuse 
    vec3 norm = normalize(scaledNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * kd;
    
    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    vec3 specular = spec * lightColor * ks;  
        
    //vec3 result = (ambient + diffuse) * finalColor;
    vec3 result = (ambient + diffuse) * finalColor + specular;
	
    color = vec4(result, 1.0f);
}
    