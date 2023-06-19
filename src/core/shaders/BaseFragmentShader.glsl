#version 330

in vec2 tex_coord;
in vec2 pos;

uniform sampler2D in_texture;
uniform float time;

uniform float playerX;
uniform float playerY;

uniform int render_flipped;

layout(location = 0) out vec4 out_color;

float uLightIntensity = 0.01;
float uLightRadius = 250.f;

uniform vec2 uPointLightPositions[40];
uniform int nLightSources;

float staticLightSourceRadius = 500.f;
vec3 staticLightColor = vec3(213, 72, 1);
float staticLightIntensity = 0.01;

uniform vec4 overlay_color;

void main() {
	if (render_flipped == 1) {
		out_color = texture2D(in_texture, vec2(1 - tex_coord.x, tex_coord.y));
	} else {
		out_color = texture2D(in_texture, tex_coord);
	}

	out_color.rgb += overlay_color.rgb * overlay_color.a;

	float d = distance(pos, vec2(playerX, playerY));

	vec3 ambient = out_color.rgb * 0.35;

	float value = 1.0 - smoothstep(0, uLightRadius, d);
	vec3 intensity = clamp(value, 0.0, 1.0) + ambient;

	for (int i = 0; i < nLightSources; i++) {
		d = distance(pos, vec2(uPointLightPositions[i].x, uPointLightPositions[i].y));

		value = 1.0 - smoothstep(0, staticLightSourceRadius, d);
		intensity += clamp(value, 0.0, 1.0) * staticLightColor * staticLightIntensity;
	}

	out_color = vec4(out_color.rgb * intensity, out_color.a);
}