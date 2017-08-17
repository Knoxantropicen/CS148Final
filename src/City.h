#pragma once

#include "ResourceManager.h"
#include "Building.h"

class City {
public:
	City() {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);
		std::uniform_real_distribution<> dis3(0, 3);

		m_buildingLine = 8;
		m_scale = 200;
		m_streetOffset = 0.1;
		m_blockOffset = 0.5;

		m_translations = RM::getInstance().translations;
		int index = 0;
		for (int x = -4; x <= 4; ++x) {
			for (int z = -4; z <= 4; ++z) {
				m_translations[index++] = glm::vec3(x * m_buildingLine * (m_blockOffset + m_streetOffset) * m_scale, 0, 6 * z * m_buildingLine * (m_blockOffset + m_streetOffset * m_scale));
			}
		}

		for (int i = -m_buildingLine/2; i < m_buildingLine/2; ++i) {
			for (int j = -m_buildingLine/2; j < m_buildingLine/2; ++j) {
				// srand(rd());
				glm::vec3 currPos(i * (m_blockOffset + m_streetOffset) * m_scale, -1.0f * m_scale, j * (m_blockOffset + m_streetOffset) * m_scale);
				m_buildings.push_back(new Building(0.25 + 0.25 * dis(gen), 0.3 + 0.7 * dis(gen), 0.25 + 0.25 * dis(gen), currPos, m_scale, int(dis3(gen))));
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

	glm::vec3 * m_translations;
};