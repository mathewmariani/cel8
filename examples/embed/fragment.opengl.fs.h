const char *fragment_fs =
    "#version 330\n"
    "uniform vec3[16] palette;\n"
    "uniform sampler2D screen;\n"
    "in vec2 uv;\n"
    "out vec4 frag_color;\n"
    "vec4 getColor(int idx) { return vec4(palette[idx], 1.0); }\n"
    "void main() {\n"
    "  int c = int(texture(screen, uv).r * 255.0 + 0.5);\n"
    "  frag_color = getColor(c);\n"
    "}\n";
