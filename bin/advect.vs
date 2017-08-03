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

vec3 a;	//acceleration

// boundary
float xmin, xmax;
float ymin, ymax;
float zmin, zmax;

void main()
{
	a = vec3(0.0f, -0.98f, 0.0f);

	xmin = ymin = zmin = -10.0f;
	xmax = ymax = zmax = 3.0f;

	vVel = Vel + a * dt;
	vPos = Pos + Vel * dt + 0.5 * a * dt * dt;
	vCol = Col;
	vTime = Time + dt;

	if (vPos.x < xmin) {vPos.x = 2 * xmin - vPos.x; vVel.x = -0.4 * vVel.x;}
	if (vPos.x > xmax) {vPos.x = 2 * xmax - vPos.x; vVel.x = -0.4 * vVel.x;};
	if (vPos.y < ymin) {vPos.y = 2 * ymin - vPos.y; vVel.y = -0.4 * vVel.y;}
	if (vPos.y > ymax) {vPos.y = 2 * ymax - vPos.y; vVel.y = -0.4 * vVel.y;};
	if (vPos.z < zmin) {vPos.z = 2 * zmin - vPos.z; vVel.z = -0.4 * vVel.z;}
	if (vPos.z > zmax) {vPos.z = 2 * zmax - vPos.z; vVel.z = -0.4 * vVel.z;};
}
