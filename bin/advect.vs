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
uniform vec3 a;		// acceleration

void main()
{
	vVel = Vel + a * dt;
	vPos = Pos + Vel * dt + 0.5 * a * dt * dt;
	vCol = Col;
	vTime = Time + dt;
}
