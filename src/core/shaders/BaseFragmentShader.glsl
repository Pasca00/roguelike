#version 330

in vec2 tex_coord;

uniform sampler2D in_texture;
uniform float time;

uniform float playerX;
uniform float playerY;

uniform int render_flipped;

layout(location = 0) out vec4 out_color;

float uLightIntensity = 0.5;
float uLightRadius = 20;

void main() {
	if (render_flipped == 1) {
		out_color = texture2D(in_texture, vec2(1 - tex_coord.x, tex_coord.y));
	} else {
		out_color = texture2D(in_texture, tex_coord);
	}

	float d = distance(tex_coord, vec2(playerX, playerY));

	float intensity = 1.0 - smoothstep(uLightRadius, uLightRadius - 0.1, d) * uLightIntensity;

	out_color = vec4(out_color.rgb * intensity, out_color.a);
}