#version 400 core

in vec3 objectColor;
in float lifeTime;

out vec4 color;

uniform sampler2D tex;
uniform bool first;

float lifeStage = lifeTime / 4.8f;
float whiteStage = 0.0, yellowStage = 0.005, redStage = 0.2, greyStage = 0.75, blackStage = 0.9;
vec3 white = vec3(1.0f, 1.0f, 1.0f), yellow = vec3(1.0f, 1.0f, 0.0f), red = vec3(1.0f, 0.0f, 0.0f), grey = vec3(0.5f, 0.5f, 0.5f), black = vec3(0.0f, 0.0f, 0.0f);

vec3 flameColor() {
	if (lifeStage < yellowStage) {
		return (white * (yellowStage - lifeStage) + yellow * (lifeStage - whiteStage)) / yellowStage;
	}
	if (lifeStage < redStage) {
		return (yellow * (redStage - lifeStage) + red * (lifeStage - yellowStage)) / (redStage - yellowStage);
	}
	if (lifeStage < greyStage) {
		return (red * (greyStage - lifeStage) + grey * (lifeStage - redStage)) / (greyStage - redStage);
	}
	if (lifeStage < blackStage) {
		return (grey * (blackStage - lifeStage) + black * (lifeStage - greyStage)) / (blackStage - greyStage);
	}
	return black;
}

void main()
{
    //color = texture(tex, gl_PointCoord) * vec4(objectColor, 1.0f);
    //color = texture(tex, vec2(gl_PointCoord.x * 0.125 + 0.125, gl_PointCoord.y * 0.125));
    //int phase = int(lifeTime / 0.1f);
	//vec2 pointCoord = vec2(gl_PointCoord.x * 0.125 + (phase % 8) * 0.125, gl_PointCoord.y * 0.125 + phase / 8 * 0.125);
    //color = texture(tex, gl_PointCoord);

	vec4 shapeColor = texture(tex, gl_PointCoord);
	if (shapeColor.w < 0.0000001f) discard;
	// color = vec4(vec3(1.0f, 0.0f, 0.0f) * (1.0f - lifeTime / 6.4f) + vec3(0.0f, 1.0f, 1.0f) * (lifeTime / 6.4f), pow(3, -4 * lifeTime / 6.4f));
	// color = vec4(flameColor(), pow(3, -4 * lifeTime / 6.4f));
	color = vec4(flameColor(), 1.0f - lifeStage);
}
