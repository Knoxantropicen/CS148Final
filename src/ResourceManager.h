#pragma once

class RM {
private:
	RM() {}
	~RM() {}
	RM(RM const& copy);
	RM& operator=(RM const& copy);
public:
	static RM& getInstance() {
		static RM instance;
		return instance;
	}

	
}