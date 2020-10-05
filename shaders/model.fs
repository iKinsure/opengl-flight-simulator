#version 330 core
//----------------------------------------------------------------------------------------
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
    float alpha;
}; 
struct MaterialMap {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
//----------------------------------------------------------------------------------------

#define NR_POINT_LIGHTS 4
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;                // material properties
uniform MaterialMap materialMap;          // maps from texture
uniform bool has_texture;                 // color or texture

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 FragColor;
in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normals1;
//uniform sampler2D texture_height1;

void main()
{    
    if (material.alpha < 0.01) {
        discard;
	}
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
	}
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 
    FragColor = vec4(result, material.alpha);
}//----------------------------------------------------------------------------------------


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0); // diffuse shading
    vec3 reflectDir = reflect(-lightDir, normal); // specular shading
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    if (has_texture) {
        //ambient = light.ambient * material.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        //diffuse = light.diffuse * diff * material.diffuse * vec3(texture(materialMap.diffuse, TexCoords));
        //specular = light.specular * spec * material.specular * vec3(texture(materialMap.specular, TexCoords));
        ambient  = light.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        diffuse  = light.diffuse * diff * vec3(texture(materialMap.diffuse, TexCoords));
        specular = light.specular * spec * vec3(texture(materialMap.specular, TexCoords));
	}
    else 
    {
        ambient  = light.ambient  *  material.ambient;
        diffuse  = light.diffuse  *  (diff  * material.diffuse);
        specular = light.specular *  (spec  * material.specular);
	}
    return (ambient + diffuse + specular);
}//----------------------------------------------------------------------------------------



// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0); // diffuse shading
    vec3 reflectDir = reflect(-lightDir, normal); // specular shading
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos); // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    
    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    if (has_texture) {
        //ambient = light.ambient * material.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        //diffuse = light.diffuse * diff * material.diffuse * vec3(texture(materialMap.diffuse, TexCoords));
        //specular = light.specular * spec * material.specular * vec3(texture(materialMap.specular, TexCoords));
        ambient  = light.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        diffuse  = light.diffuse * diff * vec3(texture(materialMap.diffuse, TexCoords));
        specular = light.specular * spec * vec3(texture(materialMap.specular, TexCoords));
	}
    else 
    {
        ambient  = light.ambient  *  material.ambient;
        diffuse  = light.diffuse  *  (diff  * material.diffuse);
        specular = light.specular *  (spec  * material.specular);
	}


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}//----------------------------------------------------------------------------------------



// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0); // diffuse shading
    vec3 reflectDir = reflect(-lightDir, normal); // specular shading
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos); // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-light.direction));  // spotlight intensity
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient; vec3 diffuse; vec3 specular;
    if (has_texture) {
        //ambient = light.ambient * material.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        //diffuse = light.diffuse * diff * material.diffuse * vec3(texture(materialMap.diffuse, TexCoords));
        //specular = light.specular * spec * material.specular * vec3(texture(materialMap.specular, TexCoords));
        ambient  = light.ambient * vec3(texture(materialMap.diffuse, TexCoords)); 
        diffuse  = light.diffuse * diff * vec3(texture(materialMap.diffuse, TexCoords));
        specular = light.specular * spec * vec3(texture(materialMap.specular, TexCoords));
	}
    else 
    {
        ambient  = light.ambient  *  material.ambient;
        diffuse  = light.diffuse  *  (diff  * material.diffuse);
        specular = light.specular *  (spec  * material.specular);
	}


    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}//----------------------------------------------------------------------------------------