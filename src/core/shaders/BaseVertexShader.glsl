#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec2 tex_coord;
out vec2 pos;

void main() {
	tex_coord = v_texture_coord;
	pos = (Model * vec4(v_position, 1.f)).xy;
	gl_Position =  Projection * View * Model * vec4(v_position, 1.f);
}