#version 130

// Represents light source passed to shader.
struct LightSource
{
    // (x, y, z, 1) means positioned light.
    // (x, y, z, 0) means directed light.
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};

// Keeps intermediate result.
struct LightIntensity
{
    float diffuse;
    float specular;
};

// Constant Phong lighting model shininess.
const float kShininess = 30.0;

in vec3 v_normal;
in vec3 v_view_direction;

uniform LightSource u_light0;
uniform LightSource u_light1;
uniform mat4 u_view_matrix;

out vec4 out_fragColor;

LightIntensity calculateIntensity(LightSource light)
{
    vec3 view_direction = normalize(v_view_direction);
    vec3 normal = normalize(v_normal);

    // Calculate light_direction for both directed and undirected light sources.
    vec3 delta = light.position.w * view_direction;
    vec4 light_pos_in_view_space = u_view_matrix * light.position;
    vec3 light_direction = normalize(light_pos_in_view_space.xyz + delta);
    vec3 reflect_direction = normalize(-reflect(light_direction, normal));
	
    LightIntensity result;
    result.diffuse = max(dot(normal, light_direction), 0.0);
    result.specular = pow(max(dot(reflect_direction, view_direction), 0.0), kShininess);

	// Clamp intensity to [0..1].
    result.diffuse = clamp(result.diffuse, 0.0, 1.0);
    result.specular = clamp(result.specular, 0.0, 1.0);

    return result;
}

void main()
{
	// Material colors - constant right now.
	const vec4 material_specular_color = vec4(0.7, 0.7, 0.7, 1);
	const vec4 material_diffuse_color = vec4(0.7, 0.7, 0.7, 1);
	const vec4 material_emission_color = vec4(0.15, 0.15, 0.15, 1);

	LightIntensity light0_intensity = calculateIntensity(u_light0);
	LightIntensity light1_intensity = calculateIntensity(u_light1);

    vec4 diffuse_intensity = material_diffuse_color * (u_light0.diffuse * light0_intensity.diffuse + u_light1.diffuse * light1_intensity.diffuse);
    vec4 specular_intensity = material_specular_color * (u_light0.specular * light0_intensity.specular + u_light1.specular * light1_intensity.specular);

    out_fragColor = diffuse_intensity + specular_intensity + material_emission_color;
}
