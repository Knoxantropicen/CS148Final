#version 400 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Vel;
layout (location = 2) in vec3 Col;
layout (location = 3) in float Time;

out vec3 vPos;
out vec3 vVel;
out vec3 vCol;
out float vTime;

uniform float dt;	// delta time
uniform vec3 iniPos;	// initially generated position

vec3 a;	//acceleration

// boundary
// float xmin, xmax;
// float ymin, ymax;
// float zmin, zmax;

float rand(vec2 n)
{
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{
	a = vec3(1.0f, 0.0f, 0.0f);

	// ymin = -3.0f;
	// ymax = 3.0f;
	// xmin = zmin = -10.0f;
	// xmax = zmax = 10.0f;

	vTime = Time + dt;

	float rnd = rand(Pos.xy + normalize(vec2(vTime, 1 / vTime)));

	if (vTime > 0.0f) {
		vVel = Vel + a * dt;
		vPos = Pos + Vel * dt + 0.5 * a * dt * dt;
		vCol = Col;
	} else {
		vVel = Vel;
		vPos = iniPos + rnd * vec3(0.0f, 5.0f, 0.0f);
		vCol = Col;
	}

	if (vTime > 6.4f) {
		vTime -= 6.4f;
		vPos = iniPos + rnd * vec3(0.0f, 5.0f, 0.0f);
		vVel = vec3(0.0f, 0.0f, 0.0f);
	}

	// if (vPos.x < xmin) {vPos.x = 2 * xmin - vPos.x; vVel.x = -0.4 * vVel.x;}
	// if (vPos.x > xmax) {vPos.x = 2 * xmax - vPos.x; vVel.x = -0.4 * vVel.x;};
	// if (vPos.y < ymin) {vPos.y = 2 * ymin - vPos.y; vVel.y = -0.4 * vVel.y;}
	// if (vPos.y > ymax) {vPos.y = 2 * ymax - vPos.y; vVel.y = -0.4 * vVel.y;};
	// if (vPos.z < zmin) {vPos.z = 2 * zmin - vPos.z; vVel.z = -0.4 * vVel.z;}
	// if (vPos.z > zmax) {vPos.z = 2 * zmax - vPos.z; vVel.z = -0.4 * vVel.z;};
}
