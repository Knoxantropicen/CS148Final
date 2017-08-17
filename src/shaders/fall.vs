#version 400 core

layout (location = 0) in vec3 Pos;

out vec3 vPos;

uniform float dt;	// delta time
uniform vec3 cameraPos;

float ground, sky;
vec3 vel;

float rand(vec2 n)
{
  return -1.0f + 2 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{
	ground = -100.0f;
	sky = 300.0f;
	vel = vec3(0.0f, -10.0f, 0.0f);

	vPos = Pos + vel * dt;

	if (vPos.y < ground) {
		vPos.y = sky + rand(vPos.xy) * 10.0;
		vPos.x = cameraPos.x + rand(vec2(vPos.y, 1/vPos.x)) * 400.0f;
		vPos.z = cameraPos.z + rand(vec2(vPos.x, 1/vPos.z)) * 400.0f;
	}
}
