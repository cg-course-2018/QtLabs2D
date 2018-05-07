#version 130

// Represents light source passed to shader.
struct LightSource
{
    // (x, y, z, 1) means positioned light.
    // (x, y, z, 0) means directed light.
    vec4 position;
    vec4 diffuse;
};

// Represents surface material passed to shader.
struct Material
{
    vec4 emission;
    vec4 diffuse;
	
};

// Constant Phong lighting model shininess.
const float kShininess = 30.0;

in vec3 v_normal;
in vec3 v_view_direction;

uniform Material u_material;
uniform LightSource u_light0;
uniform LightSource u_light1;
uniform mat4 u_view_matrix;

out vec4 out_fragColor;

float calculateDiffuseIntensity(LightSource light)
{
    vec3 view_direction = normalize(v_view_direction);
    vec3 normal = normalize(v_normal);

    // Calculate light_direction for both directed and undirected light sources.
    vec3 delta = light.position.w * view_direction;
    vec4 light_pos_in_view_space = u_view_matrix * light.position;
    vec3 light_direction = normalize(light_pos_in_view_space.xyz + delta);
    vec3 reflect_direction = normalize(-reflect(light_direction, normal));
	
    float diffuse = max(dot(normal, light_direction), 0.0);

	// Clamp intensity to [0..1].
    diffuse = clamp(diffuse, 0.0, 1.0);

    return diffuse;
}

void main()
{
	float light0_intensity = calculateDiffuseIntensity(u_light0);
	float light1_intensity = calculateDiffuseIntensity(u_light1);

    vec4 diffuse_intensity = u_material.diffuse * (u_light0.diffuse * light0_intensity + u_light1.diffuse * light1_intensity);

    out_fragColor = diffuse_intensity + u_material.emission;
}
