#include "ConnectedComponent.h"


Component& ComponentGroup::Find(Component& c) {
	if (c.label == c.parent) {
		return c;
	}
	else {
		return Find(components[c.parent - 1]);
	}
}

void ComponentGroup::MakeNewComponent(int label) {
	//m_labels[z][y * m_img->width() + x] = label;

	Component comp;
	comp.label = comp.parent = label;
	comp.size = 1;
	components.push_back(comp);
}

void ComponentGroup::AddNewElement(int label) {
	Component& comp = components[label - 1];
	comp.size++;
}

void ComponentGroup::Merge(int label_x, int label_y) {
	Component& root_x = Find(components[label_x - 1]);
	Component& root_y = Find(components[label_y - 1]);

	if (root_x.label == root_y.label) return;

	if (root_x.size < root_y.size) {
		root_x.parent = root_y.label;
		root_y.size += root_x.size;
	}
	else {
		root_y.parent = root_x.label;
		root_x.size += root_y.size;
	}

}