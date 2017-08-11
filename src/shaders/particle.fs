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
	vec4 shapeColor = texture(tex, gl_PointCoord);
	if (shapeColor.w < 0.000001f) discard;
	color = vec4(flameColor(), 1.0f - lifeStage);
}
