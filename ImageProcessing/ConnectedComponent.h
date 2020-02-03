#pragma once
#include<vector>

class Component {
public:
	int label;
	int parent;
	int size;

	bool operator > (const Component& comp) const {
		return size > comp.size;
	}
};

class ComponentGroup {
public:
	std::vector<Component> components;

	Component& Find(Component& c);
	void MakeNewComponent(int label);
	void AddNewElement(int label);
	void Merge(int label_x, int label_y);
};