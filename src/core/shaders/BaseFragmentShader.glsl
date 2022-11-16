#version 330

in vec2 tex_coord;

uniform sampler2D in_texture;
uniform float time;

uniform int render_flipped;

void main() {
	if (render_flipped == 1) {
		gl_FragColor = texture2D(in_texture, vec2(1 - tex_coord.x, tex_coord.y));
	} else {
		gl_FragColor = texture2D(in_texture, tex_coord);
	}
}