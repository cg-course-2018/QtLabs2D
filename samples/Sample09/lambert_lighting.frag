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
in vec3 v_pos_in_world_space;

uniform Material u_material;
uniform LightSource u_light0;
uniform LightSource u_light1;

out vec4 out_fragColor;

float calculateDiffuseIntensity(LightSource light)
{
    vec3 normal = normalize(v_normal);

    // Calculate direction_to_light for both directed and undirected light sources.
    vec3 direction_to_light = normalize(vec3(light.position) - light.position.w * v_pos_in_world_space);

    float diffuse = max(dot(normal, direction_to_light), 0.0);

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
