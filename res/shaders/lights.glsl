struct PointLight
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    vec3 Position;

    float Constant;
    float Linear;
    float Quadratic;
};

uniform PointLight uPointLight;

vec4 compute_point_light_color(vec3 normal, vec3 fragPosition, vec3 viewPosition, Material material)
{
    float distance = length(uPointLight.Position - fragPosition);
    float attenuation = 1.0 / (uPointLight.Constant + uPointLight.Linear * distance + uPointLight.Quadratic * (distance * distance));

    // Point light calculations
    vec3 normNormal = normalize(normal);
    vec3 lightDirection = normalize(uPointLight.Position - fragPosition);

    // Diffuse
    float diffuse = max(dot(normNormal, lightDirection), 0.0);
    vec3 diffuseColor = diffuse * uPointLight.DiffuseColor * material.Diffuse;
    // Ambient
    vec3 ambientColor = uPointLight.AmbientColor * material.Ambient;

    // Specular
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normNormal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.Shininess);
    vec3 specularColor = material.Specular * specular * uPointLight.SpecularColor;

    return vec4((ambientColor + diffuseColor + specularColor) * attenuation, 1.0);
}


struct DirectionalLight
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    vec3 Direction;
};

uniform DirectionalLight uDirectionalLight;

vec4 compute_directional_light_color(vec3 normal, vec3 fragPosition, vec3 viewPosition, Material material)
{
    vec3 lightDirection = normalize(-uDirectionalLight.Direction);

    // Point light calculations
    vec3 normNormal = normalize(normal);

    // Diffuse
    float diffuse = max(dot(normNormal, lightDirection), 0.0);
    vec3 diffuseColor = diffuse * uDirectionalLight.DiffuseColor * material.Diffuse;
    // Ambient
    vec3 ambientColor = uDirectionalLight.AmbientColor * material.Ambient;

    // Specular
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normNormal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.Shininess);
    vec3 specularColor = material.Specular * specular * uDirectionalLight.SpecularColor;

    return vec4(ambientColor + diffuseColor + specularColor, 1.0);
}