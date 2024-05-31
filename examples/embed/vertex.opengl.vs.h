const char *vertex_vs =
    "#version 330\n"
    "layout(location=0) in vec2 position;\n"
    "layout(location=1) in vec2 texcoord;\n"
    "out vec2 uv;\n"
    "void main() {\n"
    "  gl_Position = vec4(position.xy, 0.0, 1.0);\n"
    "  uv = texcoord;\n"
    "}\n";