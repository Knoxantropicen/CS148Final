#pragma once

#include "ResourceManager.h"
#include "Building.h"

class City {
public:
	City() {

		std::random_device rd;
		srand(rd());

		m_buildingLine = 8;
		m_scale = 200;
		m_streetOffset = 0.1;
		m_blockOffset = 0.5;
		for (int i = -m_buildingLine/2; i < m_buildingLine/2; ++i) {
			for (int j = -m_buildingLine/2; j < m_buildingLine/2; ++j) {
				// srand(rd());
				glm::vec3 currPos(i * (m_blockOffset + m_streetOffset) * m_scale, -1.0f * m_scale, j * (m_blockOffset + m_streetOffset) * m_scale);
				m_buildings.push_back(new Building(0.25 + 0.25 * drand48(), 0.3 + 0.7 * drand48(), 0.25 + 0.25 * drand48(), currPos, m_scale, rand()%3));
			}
		}
	}

	void render() const {
		for (size_t i = 0; i < m_buildings.size(); ++i) {
			m_buildings[i]->render();
		}
	}

	void advance() {
		render();
	}
private:
	std::vector<Building*> m_buildings;

	int m_buildingLine;
	float m_scale;
	float m_streetOffset, m_blockOffset;
};