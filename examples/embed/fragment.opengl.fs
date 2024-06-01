#version 330

uniform vec3[16] palette;
uniform sampler2D screen;
uniform sampler2D font;

in vec2 uv;

out vec4 frag_color;

vec4 getColor(int idx) {
  return vec4(palette[idx], 1.0);
}

void main() {
  int c = int(texture(screen, uv).r * 255.0 + 0.5);
  frag_color = getColor(c);
}