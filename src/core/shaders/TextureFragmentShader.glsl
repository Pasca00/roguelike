#version 330

in vec2 tex_coord;
in vec2 pos;

uniform sampler2D in_texture;
uniform float time;

uniform float playerX;
uniform float playerY;

uniform int render_flipped;

layout(location = 0) out vec4 out_color;

void main() {
	if (render_flipped == 1) {
		out_color = texture2D(in_texture, vec2(1 - tex_coord.x, tex_coord.y));
	} else {
		out_color = texture2D(in_texture, tex_coord);
	}
}