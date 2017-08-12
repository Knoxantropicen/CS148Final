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
uniform float alpha, phi;
vec3 localPos;

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
	float pi = 3.14159265359f;
	vec3 a = vec3(0.0f, 0.0f, 1.0f);
	mat4 rotV = mat4(cos(-alpha), 0, sin(-alpha), 0, 
					0, 1, 0, 0, 
					-sin(-alpha), 0, cos(-alpha), 0,
					0, 0, 0, 1);
	float c = cos(-phi), s = sin(-phi), t = 1.0f - cos(-phi);
	float ux = -sin(alpha + pi/2), uy = 0.0f, uz = -cos(alpha + pi/2);
	mat4 rotH = mat4(t*ux*ux+c, t*ux*uy-s*uz, t*ux*uz+s*uy, 0,
					t*ux*uy+s*uz, t*uy*uy+c, t*uy*uz-s*ux, 0,
					t*ux*uz-s*uy, t*uy*uz+s*ux, t*uz*uz+c, 0,
					0, 0, 0, 1);

	vTime = Time + dt;

	float theta = rand(Pos.xy + normalize(vec2(vTime, 1 / vTime))) * pi * 2;
	float radius = rand(Pos.yz + normalize(vec2(vTime, 1 / vTime))) * 0.75;
	float lr = rand(Pos.xz + normalize(vec2(vTime, 1 / vTime)));

	float rnd = rand(vec2(theta, radius));

	if (lr < 0.5f) localPos = vec3(radius * sin(theta) - 1.25f, radius * cos(theta) - 0.05f, 0.0f);
	else localPos = vec3(radius * sin(theta) + 1.25f, radius * cos(theta) - 0.05f, 0.0f);

	a = vec3(sin(alpha)*cos(phi), sin(phi), cos(alpha)*cos(phi));
	localPos += vec3(0.0f, 0.0f, -0.5f + rnd);

	vec4 tempLocalPos = rotH * rotV * vec4(localPos, 1.0f);
	localPos = vec3(tempLocalPos.x, tempLocalPos.y, tempLocalPos.z);

	if (vTime > 0.0f) {
		vVel = Vel + a * dt;
		vPos = Pos + Vel * dt + 0.5 * a * dt * dt;
		vCol = Col;
	} else {
		vVel = Vel;
		vPos = iniPos + localPos;
		vCol = Col;
	}

	if (vTime > 4.8f) {
		vTime -= 4.8f;
		vPos = iniPos + localPos;
		vVel = vec3(0.0f, 0.0f, 0.0f);
	}

	// if (vPos.x < xmin) {vPos.x = 2 * xmin - vPos.x; vVel.x = -0.4 * vVel.x;}
	// if (vPos.x > xmax) {vPos.x = 2 * xmax - vPos.x; vVel.x = -0.4 * vVel.x;};
	// if (vPos.y < ymin) {vPos.y = 2 * ymin - vPos.y; vVel.y = -0.4 * vVel.y;}
	// if (vPos.y > ymax) {vPos.y = 2 * ymax - vPos.y; vVel.y = -0.4 * vVel.y;};
	// if (vPos.z < zmin) {vPos.z = 2 * zmin - vPos.z; vVel.z = -0.4 * vVel.z;}
	// if (vPos.z > zmax) {vPos.z = 2 * zmax - vPos.z; vVel.z = -0.4 * vVel.z;};
}
