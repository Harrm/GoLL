
uniform int cellState;
uniform sampler2D texture;

float value(vec3 c) {
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
	return q.x;
}

void main() {
	vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
	float value = value(texColor.rgb);
	gl_FragColor = value > 0.5 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1);
}
