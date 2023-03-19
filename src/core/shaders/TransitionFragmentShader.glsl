#version 330

in vec2 tex_coord;

uniform sampler2D in_texture;
uniform float time;
uniform float time_total;

layout(location = 0) out vec4 out_color;

void main() {
	float time_normalized = time / time_total;
	out_color = vec4(0, 0, 0, time_normalized);
}