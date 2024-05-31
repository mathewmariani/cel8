const char *vertex_vs =
    "#version 300 es\n"
    "layout(location=0) in vec4 position;\n"
    "layout(location=1) in vec2 texcoord;\n"
    "out vec2 uv;\n"
    "void main() {\n"
    "  gl_Position = position;\n"
    "  uv = texcoord;\n"
    "}\n";