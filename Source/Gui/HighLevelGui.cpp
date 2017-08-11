/*
 * HighLevelGui.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#include "HighLevelGui.h"

#include <algorithm>
#include <map>

#include "Atoms/Atom.h"
#include "Global.h"
#include "Gui/Colours.h"
#include "Technical/Synth.h"

namespace AtomSynth {

#define ATOM_WIDTH 160
#define ATOM_HEIGHT 80
#define TAB_WIDTH 16
#define TAB_HEIGHT 20
#define LARGE_TAB_WIDTH (TAB_WIDTH + 8)
#define LARGE_TAB_HUMP 4
#define LARGE_TAB_HEIGHT (TAB_HEIGHT + LARGE_TAB_HUMP)

AtomNetworkWidget::AtomNetworkWidget() :
		Component(),
		m_currentAtom(nullptr),
		m_xOffset(0),
		m_yOffset(0),
		m_currentTab(-1),
		m_px(0),
		m_py(0),
		m_dragStatus(DragStatus::MOVING) {

}

AtomNetworkWidget::~AtomNetworkWidget() {

}

#define ATOM_WIDTH 160
#define ATOM_HEIGHT 80
#define TAB_WIDTH 16
#define TAB_HEIGHT 20
#define TAB_SEPERATION 2
#define LARGE_TAB_WIDTH (TAB_WIDTH + 12)
#define LARGE_TAB_HUMP 6
#define LARGE_TAB_HEIGHT (TAB_HEIGHT + LARGE_TAB_HUMP)
#define LARGE_TAB_SEPERATION 4
#define TAB_EXTRA (LARGE_TAB_SEPERATION -  TAB_SEPERATION)
#define LABEL_HEIGH 30

void AtomNetworkWidget::drawAtomController(Graphics & g, AtomSynth::AtomController * c) {
	int x = c->getX() - m_xOffset, y = c->getY() - m_yOffset;
	int w = ATOM_WIDTH, h = ATOM_HEIGHT;
	w = std::max(w, LARGE_TAB_SEPERATION * 2 + TAB_WIDTH * c->getNumAutomationInputs() + LARGE_TAB_WIDTH * c->getNumPrimaryInputs() + ((c->getAutomationEnabled()) ? -TAB_SEPERATION : 0));
	w = std::max(w, LARGE_TAB_SEPERATION * 2 + LARGE_TAB_WIDTH * c->getNumOutputs());

	g.setFont(C::LARGE_FONT);
	g.setColour(ATOM_BG);
	g.fillRoundedRectangle(x, y, w, h, 4.0);
	g.setColour(ATOM_TEXT_BG);
	g.fillRoundedRectangle(x + 4, y + (h / 2) - (LABEL_HEIGH / 2), w - 8, LABEL_HEIGH, 4);
	g.setColour(ATOM_BG);
	g.setFont(20.0);
	g.drawFittedText(c->getName(), x + 8, y, w - 16, h, Justification::centred, 1, 0.8);

	if (c->getNumOutputs() > 0) {
		g.fillRoundedRectangle(x, y + ATOM_HEIGHT - 4, LARGE_TAB_SEPERATION + LARGE_TAB_WIDTH * c->getNumOutputs(), LARGE_TAB_HUMP + LARGE_TAB_SEPERATION, 4.0);
		g.setColour(CONNECTION_TYPE_WHITE);
		for (int i = 0; i < c->getNumOutputs(); i++) {
			g.fillRoundedRectangle(x + LARGE_TAB_SEPERATION + LARGE_TAB_WIDTH * i, y + ATOM_HEIGHT - LARGE_TAB_HEIGHT + LARGE_TAB_HUMP, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, LARGE_TAB_HEIGHT, 4.0);
			g.fillRect(x + LARGE_TAB_SEPERATION + LARGE_TAB_WIDTH * i, y + ATOM_HEIGHT + LARGE_TAB_HUMP - 4, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, 4);
		}
	}

	if (c->getNumPrimaryInputs() > 0) {
		g.setColour(ATOM_BG);
		g.fillRoundedRectangle(x, y - LARGE_TAB_HUMP, LARGE_TAB_SEPERATION + LARGE_TAB_WIDTH * c->getNumPrimaryInputs(), LARGE_TAB_HUMP + LARGE_TAB_SEPERATION, 4.0);
		g.setColour(CONNECTION_TYPE_WHITE);
		for (int i = 0; i < c->getNumPrimaryInputs(); i++) {
			g.fillRoundedRectangle(x + LARGE_TAB_SEPERATION, y - LARGE_TAB_HUMP, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, LARGE_TAB_HEIGHT, 4.0);
			g.fillRect(x + LARGE_TAB_SEPERATION, y - LARGE_TAB_HUMP, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, 4);
			x += LARGE_TAB_WIDTH;
		}
	}

	for (int i = 0; i < c->getNumAutomationInputs(); i++) {
		g.setColour(ATOM_COLOURS[i + 2]);
		g.fillRoundedRectangle(x + LARGE_TAB_SEPERATION + i * TAB_WIDTH, y, TAB_WIDTH - TAB_SEPERATION, TAB_HEIGHT, 4.0);
		g.fillRect(x + LARGE_TAB_SEPERATION + i * TAB_WIDTH, y, TAB_WIDTH - TAB_SEPERATION, 4);
	}

	std::vector<std::string>& inputIcons = c->getInputIcons();
	int size;
	x = c->getX() - m_xOffset + LARGE_TAB_SEPERATION;
	for(int i = 0; i < inputIcons.size(); i++) {
		const char * data = BinaryData::getNamedResource(inputIcons[i].c_str(), size);
		g.drawImageAt(ImageCache::getFromMemory(data, size), x + 1, y - LARGE_TAB_HUMP + 1);
		x += LARGE_TAB_WIDTH;
	}
	std::vector<std::string>& outputIcons = c->getOutputIcons();
	x = c->getX() - m_xOffset + LARGE_TAB_SEPERATION;
	for(int i = 0; i < outputIcons.size(); i++) {
		const char * data = BinaryData::getNamedResource(outputIcons[i].c_str(), size);
		g.drawImageAt(ImageCache::getFromMemory(data, size), x + 1, y + ATOM_HEIGHT + LARGE_TAB_HUMP - 22 - 1);
		x += LARGE_TAB_WIDTH;
	}
}

void AtomNetworkWidget::paint(Graphics & g) {
	g.setColour(ATOM_BG);
	int x = -m_xOffset, y = -m_yOffset;
	switch (m_dragStatus) {
	case DragStatus::IN_TO_OUT:
		if (m_currentTab < m_currentAtom->getNumPrimaryInputs()) {
			x += m_currentAtom->getX() + 4 + m_currentTab * LARGE_TAB_WIDTH;
			x += (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
			y += m_currentAtom->getY() - LARGE_TAB_HUMP;
		} else {
			x += m_currentAtom->getX() + 4 + m_currentAtom->getNumPrimaryInputs() * LARGE_TAB_WIDTH;
			x += (m_currentTab - m_currentAtom->getNumPrimaryInputs()) * TAB_WIDTH;
			x += (TAB_WIDTH - TAB_SEPERATION) / 2;
			y += m_currentAtom->getY();
		}
		g.drawLine(x, y, m_px, m_py);
		break;

	case DragStatus::OUT_TO_IN:
		x += m_currentAtom->getX() + 4 + m_currentTab * LARGE_TAB_WIDTH;
		x += (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
		y += m_currentAtom->getY() + ATOM_HEIGHT + LARGE_TAB_HUMP;
		g.drawLine(x, y, m_px, m_py);
		break;
	}

	//Draw connections
	for (AtomSynth::AtomController * controller : Synth::getInstance()->getAtomManager().getAtoms()) {
		int index = 0, x = controller->getX() + 4 - m_xOffset, y = controller->getY() - m_yOffset, ix = 0, iy = 0;
		for (std::pair<AtomSynth::AtomController *, int> input : controller->getAllInputs()) {
			if (input.first != nullptr) {
				ix = input.first->getX() + 4 - m_xOffset;
				ix += input.second * LARGE_TAB_WIDTH;
				iy = input.first->getY() - m_yOffset;
				iy += ATOM_HEIGHT + LARGE_TAB_HUMP;
				ix += (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
				if (index < controller->getNumPrimaryInputs()) {
					g.setColour(CONNECTION_TYPE_WHITE);
					g.fillRoundedRectangle(x, y - LARGE_TAB_HUMP - 8, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, 16, 8.0);
					x += (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
					g.drawLine(ix, iy + 4, x, y - 4 - LARGE_TAB_HUMP, 6);
					x += (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
					x += LARGE_TAB_SEPERATION;
				} else {
					g.setColour(ATOM_COLOURS[index - controller->getNumPrimaryInputs() + 2]);
					g.fillEllipse(x, y - (TAB_WIDTH - TAB_SEPERATION) / 2, TAB_WIDTH - TAB_SEPERATION, TAB_WIDTH - TAB_SEPERATION);
					x += (TAB_WIDTH - TAB_SEPERATION) / 2;
					g.drawLine(ix, iy + 4, x, y - 2, 4);
					x += (TAB_WIDTH - TAB_SEPERATION) / 2;
					x += TAB_SEPERATION;
				}
				g.setColour(CONNECTION_TYPE_WHITE);
				ix -= (LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION) / 2;
				g.fillRoundedRectangle(ix, iy - 8, LARGE_TAB_WIDTH - LARGE_TAB_SEPERATION, 16, 8.0);
			} else {
				if (index < controller->getNumPrimaryInputs()) {
					x += LARGE_TAB_WIDTH;
				} else {
					x += TAB_WIDTH;
				}
			}
			index++;
		}
	}

	//Draw controllers
	for (AtomSynth::AtomController * controller : Synth::getInstance()->getAtomManager().getAtoms()) {
		drawAtomController(g, controller);
	}
}

void AtomNetworkWidget::mouseDown(const MouseEvent & event) {
	m_px = event.x;
	m_py = event.y;
	m_dragStatus = DragStatus::MOVING;
	int x, y, w;
	for (int i = Synth::getInstance()->getAtomManager().getAtoms().size() - 1; i >= 0; i--) {
		AtomController * controller = Synth::getInstance()->getAtomManager().getAtoms()[i];
		AtomController * c = controller;
		x = event.x - controller->getX() + m_xOffset;
		y = event.y - controller->getY() + m_yOffset;
		w = ATOM_WIDTH;
		w = std::max(w, LARGE_TAB_SEPERATION * 2 + TAB_WIDTH * c->getNumAutomationInputs() + LARGE_TAB_WIDTH * c->getNumPrimaryInputs() + ((c->getAutomationEnabled()) ? -TAB_SEPERATION : 0));
		w = std::max(w, LARGE_TAB_SEPERATION * 2 + LARGE_TAB_WIDTH * c->getNumOutputs());

		if ((x >= 0) && (y >= -LARGE_TAB_HUMP) && (x <= w) && (y <= ATOM_HEIGHT + LARGE_TAB_HUMP)) {
			AtomController * oldController = m_currentAtom;
			m_currentAtom = controller;
			if (ModifierKeys::getCurrentModifiersRealtime().isRightButtonDown()) {
				m_currentAtom->markForDeletion();
				Synth::getInstance()->getAtomManager().updateExecutionOrder();
				m_currentAtom = nullptr;
				for (Listener * listener : m_listeners) {
					listener->currentAtomChanged(oldController, m_currentAtom);
				}
				repaint();
				return;
			}
			x -= 4;
			//Input pins
			if (y <= TAB_HEIGHT) {
				if (x <= m_currentAtom->getNumPrimaryInputs() * LARGE_TAB_WIDTH) {
					x /= LARGE_TAB_WIDTH;
				} else {
					x -= m_currentAtom->getNumPrimaryInputs() * LARGE_TAB_WIDTH;
					x /= TAB_WIDTH;
					x += m_currentAtom->getNumPrimaryInputs();
				}
				if (x < m_currentAtom->getNumInputs()) {
					//If the input pin is not used, do a normal input to output link
					if (m_currentAtom->getInput(x).first == nullptr) {
						m_dragStatus = DragStatus::IN_TO_OUT;
						m_currentTab = x;
					}
					//Otherwise, setup an output to input link with the previous link's parent as the root
					else {
						m_dragStatus = DragStatus::OUT_TO_IN;
						m_currentTab = m_currentAtom->getInput(x).second;
						AtomSynth::AtomController * previous = m_currentAtom;
						m_currentAtom = m_currentAtom->getInput(x).first;
						previous->unlinkInput(x);
					}
				}
			} else if (y >= (ATOM_HEIGHT - TAB_HEIGHT)) {
				x /= LARGE_TAB_WIDTH;
				if (x < m_currentAtom->getNumOutputs()) {
					m_dragStatus = DragStatus::OUT_TO_IN;
					m_currentTab = x;
					if (m_currentTab >= m_currentAtom->getNumOutputs()) {
						m_dragStatus = DragStatus::MOVING;
					}
				}
			}
			repaint();
			for (Listener * listener : m_listeners) {
				listener->currentAtomChanged(oldController, m_currentAtom);
			}
			return;
		}
	}
	for (Listener * listener : m_listeners) {
		listener->currentAtomChanged(m_currentAtom, nullptr);
	}
	m_currentAtom = nullptr;
	repaint();
}

void AtomNetworkWidget::mouseDrag(const MouseEvent & event) {
	int dx = event.x - m_px, dy = event.y - m_py;
	m_px += dx;
	m_py += dy;

	if (m_dragStatus == DragStatus::MOVING) {
		if (m_currentAtom == nullptr) {
			m_xOffset -= dx;
			m_yOffset -= dy;
		} else {
			m_currentAtom->setPosition(m_currentAtom->getX() + dx, m_currentAtom->getY() + dy);
		}
	}
	repaint();
}

void AtomNetworkWidget::mouseUp(const MouseEvent & event) {
	AtomSynth::AtomController * dropped = nullptr;
	int tab = 0;
	DragStatus dragStatus = m_dragStatus;
	m_dragStatus = DragStatus::MOVING;
	if ((dragStatus == DragStatus::IN_TO_OUT) || (dragStatus == DragStatus::OUT_TO_IN)) {
		repaint();
		for (AtomSynth::AtomController * controller : Synth::getInstance()->getAtomManager().getAtoms()) {
			int x = event.x - controller->getX() + m_xOffset, y = event.y - controller->getY() + m_yOffset;
			if ((x >= 0) && (y >= -LARGE_TAB_HUMP) && (x <= ATOM_WIDTH * 2) && (y <= ATOM_HEIGHT + LARGE_TAB_HUMP)) {
				if ((dragStatus == DragStatus::IN_TO_OUT) && (y >= ATOM_HEIGHT - TAB_HEIGHT)) {
					dropped = controller;
					x -= 4;
					x /= LARGE_TAB_WIDTH;
					tab = x;
					break;
				} else if ((dragStatus == DragStatus::OUT_TO_IN) && (y <= TAB_HEIGHT)) {
					dropped = controller;
					x -= 4;
					if (x < dropped->getNumPrimaryInputs() * LARGE_TAB_WIDTH) {
						x /= LARGE_TAB_WIDTH;
					} else {
						x -= dropped->getNumPrimaryInputs() * LARGE_TAB_WIDTH;
						x /= TAB_WIDTH;
						x += dropped->getNumPrimaryInputs();
					}
					tab = x;
					break;
				}
			}
		}

		if ((dropped == nullptr) || (dropped == m_currentAtom))
			return;

		if (dragStatus == DragStatus::IN_TO_OUT) {
			if (tab < dropped->getNumOutputs()) {
				m_currentAtom->linkInput(m_currentTab, dropped, tab);
			}
		} else {
			if (tab < dropped->getNumInputs()) {
				dropped->linkInput(tab, m_currentAtom, m_currentTab);
			}
		}
		Synth::getInstance()->getAtomManager().updateExecutionOrder();
	}
}

void AtomNetworkWidget::setCurrentAtom(AtomController * current) {
	m_currentAtom = current;
}

Image PresetBrowser::s_closedFolder;
Image PresetBrowser::s_openFolder;
Image PresetBrowser::s_preset;

void PresetBrowser::setRoot(File newRoot) {
	m_root = *new GuiElement();
	m_root.m_path = newRoot;
	m_root.m_isFolder = newRoot.isDirectory();
	m_root.m_name = newRoot.getFileNameWithoutExtension().toStdString();
	m_root.m_expanded = true;
	m_root.m_depth = -1;
	m_root.m_parent = nullptr;
	updateList();
}

void PresetBrowser::update(GuiElement & toUpdate) {
	if (toUpdate.m_isFolder) {
		DirectoryIterator iter(toUpdate.m_path, false, "*", File::TypesOfFileToFind::findFilesAndDirectories);
		File & file = *new File();
		GuiElement * element;
		while (iter.next()) {
			file = iter.getFile();
			element = new GuiElement();
			element->m_path = file;
			element->m_isFolder = file.isDirectory();
			element->m_name = file.getFileNameWithoutExtension().toStdString();
			element->m_depth = toUpdate.m_depth + 1;
			element->m_parent = &toUpdate;
			element->m_expanded = false;
			toUpdate.m_contains.push_back(element);
			if (element->m_isFolder) {
				update(*element);
			}
		}
	}
}

void PresetBrowser::updateList() {
	int size = 0;
	const char * data;
	data = BinaryData::getNamedResource("openFolder", size);
	s_openFolder = ImageCache::getFromMemory(data, size);
	data = BinaryData::getNamedResource("closedFolder", size);
	s_closedFolder = ImageCache::getFromMemory(data, size);
	data = BinaryData::getNamedResource("preset", size);
	s_preset = ImageCache::getFromMemory(data, size);
	m_root.m_contains.clear();
	update(m_root);
}

void PresetBrowser::setSavePath(File savePath) {
	GuiElement * found = find(savePath, &m_root);
	if (found != nullptr) {
		m_savePath = found;
	}
}

bool PresetBrowser::selectFile(File toSelect) {
	m_selected = find(toSelect, &m_root);
	if (m_selected == nullptr) {
		return false;
	}
	expandParent(m_selected);
	for (Listener * listener : m_listeners) {
		listener->selectedFileChanged(this);
	}
	repaint();
}

bool PresetBrowser::addFile(std::string name, bool select) {
	GuiElement * parent = m_selected;
	if (!parent->m_isFolder) {
		parent = parent->m_parent;
	}
	File path = File(parent->m_path.getFullPathName() + "/" + name);
	if (path.exists()) {
		if (select) {
			selectFile(path);
		}
		return false;
	}
	path.create();
	GuiElement * added = new GuiElement();
	added->m_depth = parent->m_depth + 1;
	added->m_name = path.getFileNameWithoutExtension().toStdString();
	added->m_path = path;
	added->m_isFolder = false;
	added->m_parent = parent;
	parent->m_contains.push_back(added);
	if (select) {
		m_selected = added;
		expandParent(added);
		for (Listener * listener : m_listeners) {
			listener->selectedFileChanged(this);
		}
	}
	repaint();
}

bool PresetBrowser::saveFile() {
	if (m_savePath == &m_root) {
		if (addFile(m_saveName + ".aspf", true)) {
			m_savePath = m_selected;
			return true;
		}
		return false;
	} else {
		if (m_savePath != nullptr) {
			m_selected = m_savePath;
			expandParent(m_selected);
			for (Listener * listener : m_listeners) {
				listener->selectedFileChanged(this);
			}
			repaint();
			return true;
		}
		return false;
	}
}

void PresetBrowser::setSaveName(std::string name, bool force) {
	if (m_savePath != &m_root) {
		std::string path = m_savePath->m_path.getParentDirectory().getFullPathName().toStdString() + "/" + name + ".aspf";
		if (!juce::File(path).exists()) {
			m_savePath->m_path.moveFileTo(File(path));
			m_savePath->m_path = juce::File(path);
			m_savePath->m_name = juce::File(path).getFileNameWithoutExtension().toStdString();
		} else if (force) {
			m_savePath->m_path = juce::File(path);
			m_savePath->m_name = juce::File(path).getFileNameWithoutExtension().toStdString();
		}
	}
	m_saveName = name;
	repaint();
}

void PresetBrowser::rename(std::string name) {
	if ((m_selected != nullptr) && (name != "") && (m_selected != &m_root)) {
		std::string path = m_selected->m_path.getParentDirectory().getFullPathName().toStdString();
		path += "/" + name;
		juce::File file(path);
		if (!juce::File(path).exists()) {
			m_selected->m_path.moveFileTo(file);
			m_selected->m_path = file;
			m_selected->m_name = file.getFileNameWithoutExtension().toStdString();
			if (m_selected == m_savePath) {
				m_saveName = m_selected->m_name;
			}
			repaint();
		}
	}
}

void PresetBrowser::remove() {
	if ((m_selected != nullptr) && (m_selected != &m_root)) {
		if ((m_savePath == m_selected) || (m_savePath->m_path.isAChildOf(m_selected->m_path))) {
			m_savePath = &m_root;
		}
		int index = 0;
		m_selected->m_path.deleteRecursively();
		for (GuiElement * element : m_selected->m_parent->m_contains) {
			if (element == m_selected) {
				break;
			}
			index++;
		}
		std::vector<GuiElement *>::iterator iter = m_selected->m_parent->m_contains.begin() + index;
		std::vector<GuiElement *> & vec = m_selected->m_parent->m_contains;
		vec.erase(iter);
		delete (m_selected);
		m_selected = &m_root;
		repaint();
	}
}

void PresetBrowser::addFolder() {
	if ((m_selected != nullptr) && (m_selected->m_isFolder)) {
		GuiElement * element = new GuiElement();
		int index = 1;
		juce::File path = juce::File(m_selected->m_path.getFullPathName() + "/New Folder");
		/*while(path.exists())
		 {
		 index++;
		 path = File(m_selected->m_path.getFullPathName() + "/New Folder " + to_string(index));
		 }*/
		path.createDirectory();
		element->m_name = path.getFileNameWithoutExtension().toStdString();
		element->m_path = path;
		element->m_depth = m_selected->m_depth + 1;
		element->m_isFolder = true;
		element->m_expanded = false;
		element->m_parent = m_selected;
		m_selected->m_expanded = true;
		m_selected->m_contains.push_back(element);
		m_selected = element;
		for (Listener * listener : m_listeners) {
			listener->selectedFileChanged(this);
		}
		repaint();
	}
}

PresetBrowser::GuiElement * PresetBrowser::find(int & index, GuiElement * element) {
	index--;
	if (index == 0) {
		return element;
	}
	if (element->m_isFolder && element->m_expanded) {
		for (GuiElement * subFile : element->m_contains) {
			GuiElement * result = find(index, subFile);
			if (result != nullptr) {
				return result;
			}
		}
	}
	return nullptr;
}

PresetBrowser::GuiElement * PresetBrowser::find(juce::File path, GuiElement * root) {
	if (root->m_path.getFullPathName() == path.getFullPathName()) {
		return root;
	} else {
		if (root->m_isFolder) {
			for (GuiElement * element : root->m_contains) {
				GuiElement * ptr = find(path, element);
				if (ptr != nullptr) {
					return ptr;
				}
			}
		}
	}
	return nullptr;
}

void PresetBrowser::expandParent(GuiElement * leaf) {
	if (leaf->m_parent != &m_root) {
		leaf->m_parent->m_expanded = true;
		expandParent(leaf->m_parent);
	}
}

#define FILE_HEIGHT 24
int PresetBrowser::paintElement(Graphics & g, GuiElement & toPaint, int yOffset) {
	int x = toPaint.m_depth * 8;
	g.setColour(MID_LAYER);
	g.fillRoundedRectangle(2 + x, yOffset, getWidth() - 4 - x, FILE_HEIGHT, 4.0);
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(4 + x, yOffset + 2, FILE_HEIGHT - 4, FILE_HEIGHT - 4, 4.0);
	g.fillRoundedRectangle(2 + x + FILE_HEIGHT, yOffset + 2, getWidth() - 6 - FILE_HEIGHT - x, FILE_HEIGHT - 4, 4.0);
	g.setColour((&toPaint == m_selected) ? FORE_LAYER : MID_LAYER);
	g.drawFittedText(toPaint.m_name, x + 6 + FILE_HEIGHT, yOffset, getWidth() - 6 - FILE_HEIGHT - x, FILE_HEIGHT, Justification::centredLeft, 1, 1.0);
	for (int i = 0; i < toPaint.m_depth; i++) {
		g.fillRect((i * 8) + 6, yOffset - 2, 2, FILE_HEIGHT + 4);
	}
	int i = toPaint.m_depth - 1;
	if (i >= 0) {
		g.fillRect((i * 8) + 8, yOffset + (FILE_HEIGHT / 2), 2, 2);
	}
	if (toPaint.m_isFolder) {
		if (toPaint.m_expanded) {
			g.drawImageAt(s_openFolder, x + 4, yOffset + 2);
			yOffset += FILE_HEIGHT + 2;
			for (GuiElement * element : toPaint.m_contains) {
				yOffset = paintElement(g, *element, yOffset);
			}
		} else {
			g.drawImageAt(s_closedFolder, x + 4, yOffset + 2);
			yOffset += FILE_HEIGHT + 2;
		}
	} else {
		g.drawImageAt(s_preset, x + 4, yOffset + 2);
		yOffset += FILE_HEIGHT + 2;
	}
	return yOffset;
}

void PresetBrowser::paint(Graphics & g) {
	g.fillAll(MID_LAYER);
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0);
	int offset = 2;
	if (m_root.m_isFolder) {
		for (GuiElement * element : m_root.m_contains) {
			offset = paintElement(g, *element, offset);
		}
	}
}

void PresetBrowser::mouseDown(const MouseEvent& event) {
	int y = event.getMouseDownY() / (FILE_HEIGHT + 2), x = event.getMouseDownX();
	y += 2;
	m_selected = find(y, &m_root);
	if (m_selected == nullptr) {
		m_selected = &m_root;
	}
	int tx = m_selected->m_depth * 8 + 2;
	if (m_selected->m_isFolder && (x > tx) && ((x - tx) < FILE_HEIGHT + 4)) {
		m_selected->m_expanded = !m_selected->m_expanded;
	}

	for (Listener * listener : m_listeners) {
		listener->selectedFileChanged(this);
	}
	repaint();
}

void PropertiesSidepane::UpdateContentTimer::timerCallback() {
	m_parent->m_lastSaved->setText("Last saved " + std::to_string(Synth::getInstance()->getSaveManager().getTimeSinceSave() / 1000) +
			" seconds ago.", NotificationType::dontSendNotification);
	m_parent->m_name.setText(Synth::getInstance()->getSaveManager().getPatchName(), false);
}

PropertiesSidepane::PropertiesSidepane():
		Rectangle(),
		m_updateContentTimer(this) {
	setColour(MID_LAYER);

	addAndMakeVisible(m_name);
	m_name.setBounds(CB(0, 0.5, 6, 1));
	m_name.setText("Unnamed");
	m_name.addListener(this);
	addAndMakeVisible(m_name.createLabel("Patch Name"));

	addAndMakeVisible(m_saveNow);
	m_saveNow.setBounds(CB(6, 0.5, 6, 1));
	m_saveNow.setText("Save Now");
	m_saveNow.addListener(this);
	m_lastSaved = m_saveNow.createLabel("Last saved never.");
	addAndMakeVisible(m_lastSaved);

	m_updateContentTimer.startTimer(500);
}

PropertiesSidepane::~PropertiesSidepane() {

}

void PropertiesSidepane::textEntryChanged(TextEntry *entry) {
	if(entry == &m_name) {
		Synth::getInstance()->getSaveManager().setPatchName(m_name.getText());
	}
}

void PropertiesSidepane::textButtonPressed(TextButton *button) {
	if(button == &m_saveNow) {
		Synth::getInstance()->getSaveManager().saveNow();
	}
}

void AtomSynthEditor::switchView(std::string name) {
	if (m_sidepane == Sidepane::EDITOR) {
		if (m_network.getCurrentAtom() != nullptr) {
			m_network.getCurrentAtom()->getGui().setVisible(false);
		} else {
			m_properties.setVisible(false);
		}
	} else if (m_sidepane == Sidepane::ADD) {
		m_addAtom.setVisible(false);
	}

	if (name == "network") {
		m_sidepane = Sidepane::NOTHING;
		m_buttons.setBounds(0, 0, CD(4), CD(1));
		m_toggleSidepane.setIcon("edit");
	} else {
		if (name == "edit") {
			m_sidepane = Sidepane::EDITOR;
			if (m_network.getCurrentAtom() != nullptr) {
				m_network.getCurrentAtom()->getGui().setVisible(true);
			} else {
				m_properties.setVisible(true);
			}
		} else if (name == "add") {
			m_sidepane = Sidepane::ADD;
			m_addAtom.setVisible(true);
		}
		m_buttons.setBounds(C::SPACING + C::HEADER_WIDTH, 0, CD(4), CD(1));
		m_toggleSidepane.setIcon("network");
	}
	repaint();
}

void AtomSynthEditor::createNewAtom(std::string name) {
	for (AtomController * controller : Synth::getInstance()->getAtomManager().getAvailableAtoms()) {
		if (controller->getName() == name) {
			AtomController * newController = controller->createNewInstance();
			newController->setPosition(m_network.getXOffset() + C::GUI_WIDTH + C::SPACING, m_network.getYOffset() + C::SPACING);
			Synth::getInstance()->getAtomManager().addAtom(newController);
			currentAtomChanged(m_network.getCurrentAtom(), newController);
			m_network.setCurrentAtom(newController);
			m_network.repaint();
			repaint();
			return;
		}
	}
}

AtomSynthEditor::AtomSynthEditor() :
		Component() {
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(C::VST_WIDTH, C::VST_HEIGHT);
	Synth::getInstance()->getGuiManager().setReloadGuis();

	//Atom network widget
	addAndMakeVisible(m_network, -1000);
	m_network.setBounds(0, 0, C::VST_WIDTH, C::VST_HEIGHT);
	m_network.addListener(this);

	//Properties view
	addAndMakeVisible(&m_properties);
	m_properties.setBounds(0, C::SPACING + C::HEADER_HEIGHT + C::SPACING, C::GUI_WIDTH, C::GUI_HEIGHT);

	String seperator;
#ifdef OS_WIN
	seperator = "\\";
#else
	seperator = "/";
#endif
	addAndMakeVisible(m_buttons, 1000);
	m_buttons.setBounds(C::SPACING + C::HEADER_WIDTH, 0, CD(4), CD(1));

	//Navigation buttons
	m_buttons.addAndMakeVisible(m_toggleSidepane);
	m_toggleSidepane.setBounds(CB(0, 0, 1, 1));
	m_toggleSidepane.setIcon("network");
	m_toggleSidepane.addListener(this);

	m_buttons.addAndMakeVisible(m_open);
	m_open.setBounds(CB(2, 0, 1, 1));
	m_open.setIcon("open");
	m_open.addListener(this);

	m_buttons.addAndMakeVisible(m_add);
	m_add.setBounds(CB(3, 0, 1, 1));
	m_add.setIcon("add");
	m_add.addListener(this);

	//Add view
	addAndMakeVisible(&m_addAtom);
	m_addAtom.setBounds(0, C::HEADER_HEIGHT + C::SPACING, C::GUI_WIDTH, C::GUI_HEIGHT);
	m_addAtom.setColour(MID_LAYER);
	m_addAtom.setVisible(false);
	std::map<std::string, std::vector<AtomController *> *> categories;
	for (AtomController * controller : Synth::getInstance()->getAtomManager().getAvailableAtoms()) {
		if (categories[controller->getCategory()] == nullptr) {
			categories[controller->getCategory()] = new std::vector<AtomController *>();
		}
		categories[controller->getCategory()]->push_back(controller);
	}
	double x = -4.0, y = 0.0;
	for (std::pair<std::string, std::vector<AtomController *> *> category : categories) {
		Label * label = new Label();
		m_addAtom.addAndMakeVisible(label);
		label->setBounds(CB(0, y, 12, 0.5));
		label->setText(String(category.first), NotificationType::dontSendNotification);
		y += 0.5;
		for (AtomController * controller : *category.second) {
			x += 4.0;
			if (x == 12.0) {
				x = 0.0;
				y += 1.0;
			}
			TextButton * button = new TextButton();
			m_addAtom.addAndMakeVisible(button);
			button->setBounds(CB(x, y, 4, 1));
			button->setText(controller->getName());
			button->addListener(this);
			info(controller->getName());
		}
		y += 1.0;
		x = -4.0;
	}

	setWantsKeyboardFocus(true);
	//grabKeyboardFocus();
	switchView("edit");

	//Act like a new preset was just loaded.
	presetLoaded();
}

AtomSynthEditor::~AtomSynthEditor() {

}

void AtomSynthEditor::presetLoaded() {
	int minx = 0, miny = 0;
	for (AtomController * controller : Synth::getInstance()->getAtomManager().getAtoms()) {
		minx = std::min(minx, controller->getX());
		miny = std::min(miny, controller->getY());
	}
	m_network.setOffset(-(minx + C::GUI_WIDTH + C::SPACING), -(miny + C::SPACING));
}

void AtomSynthEditor::imageButtonPressed(AtomSynth::ImageButton * button) {
	std::string name = button->getIconName();
	if ((name == "edit") || (name == "network") || (name == "add")) {
		switchView(name);
	} else if (button->getIconName() == "delete") {
		//m_presetBrowser.remove();
	} else if (button->getIconName() == "addFolder") {
		//m_presetBrowser.addFolder();
	} else if (button->getIconName() == "save") {
		info(Synth::getInstance()->getAtomManager().saveSaveState().exportString());
	} else if (name == "open") {
		FileChooser chooser("Pick A Patch",
				File::getCurrentWorkingDirectory().getChildFile("Patches"),
				"*.ssf", false);
		chooser.browseForFileToOpen();
		Synth::getInstance()->getSaveManager().load(chooser.getResult());
	}
}

void AtomSynthEditor::multiButtonPressed(MultiButton * button) {
	/*
	 std::string name;
	 switch(button->getSelectedLabel())
	 {
	 case 0:
	 m_saveLoadButton.setIcon("edit");
	 m_addDeleteButton.setIcon("network");
	 name = m_presetBrowser.getSelectedFile().getFileNameWithoutExtension().toStdString();
	 if(name == "AtomSynthPresets")
	 {
	 m_synthNameEntry.setText("", false);
	 }
	 else
	 {
	 m_synthNameEntry.setText(m_presetBrowser.getSelectedFile().getFileNameWithoutExtension().toStdString(), false);
	 }
	 break;
	 case 1:
	 m_saveLoadButton.setIcon("save");
	 m_addDeleteButton.setIcon("addFolder");
	 m_synthNameEntry.setText(m_presetBrowser.getSaveName());
	 break;
	 case 2:
	 m_saveLoadButton.setIcon("open");
	 m_addDeleteButton.setIcon("delete");
	 name = m_presetBrowser.getSelectedFile().getFileNameWithoutExtension().toStdString();
	 if(name == "AtomSynthPresets")
	 {
	 m_synthNameEntry.setText("", false);
	 }
	 else
	 {
	 m_synthNameEntry.setText(m_presetBrowser.getSelectedFile().getFileNameWithoutExtension().toStdString(), false);
	 }
	 break;
	 }*/
}

void AtomSynthEditor::textEntryChanged(TextEntry * entry) {
	/*if(m_synthSaveLoadButton.getSelectedLabel() == 1)
	 {
	 m_presetBrowser.setSaveName(entry->getText());
	 }
	 else
	 {
	 if(m_presetBrowser.getSelectedFile().isDirectory())
	 {
	 m_presetBrowser.rename(entry->getText());
	 }
	 else
	 {
	 m_presetBrowser.rename(entry->getText() + ".aspf");
	 }
	 }*/
}

const int units[80] = { 12, -1, 14, 16, -1, -1, 13, 15, -1, 18, 20, 22, -1, 25, 27, 59, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, 31, -1, -1, -1, -1, 07, 04, 03, 16, -1, 06, 8, 24, 10, -1, 13, 11, 9, 26, 28, 12, 17, 01, 19, 23, 05, 14, 02, 21, 00, -1 };

#define SEMITONE_DETUNE 1.05946309436
bool AtomSynthEditor::keyPressed(const juce::KeyPress & key, juce::Component * originatingComponent) {
	return true;
}

bool AtomSynthEditor::keyStateChanged(bool isKeyDown, Component * originatingComponent) {
	for (int i = 44; i < 124; i++) {
		if (units[i - 44] != -1) {
			double frequency = 440.0 * pow(SEMITONE_DETUNE, units[i - 44] - 9);
			bool pressed = KeyPress::isKeyCurrentlyDown(i);
			bool active = Synth::getInstance()->getNoteManager().getIsFrequencyActive(frequency);
			if (pressed != active) {
				if (pressed) {
					Synth::getInstance()->getNoteManager().addFrequency(frequency);
				} else {
					Synth::getInstance()->getNoteManager().removeFrequency(frequency);
				}
			}
		}
	}
	return true;
}

void AtomSynthEditor::selectedFileChanged(PresetBrowser* browser) {
	if (m_synthSaveLoadButton.getSelectedLabel() != 1) {
		m_synthNameEntry.setText(browser->getSelectedFile().getFileNameWithoutExtension().toStdString());
	}
}

//==============================================================================
void AtomSynthEditor::paint(Graphics & g) {
	if (Synth::getInstance()->getGuiManager().shouldReloadGuis()) {
		for (AtomController * controller : AtomSynth::Synth::getInstance()->getAtomManager().getAtoms()) {
			controller->getGui().setParent(this);
		}
		Synth::getInstance()->getGuiManager().setReloadGuis(false);
	}

	g.fillAll(ATOM_CANVAS);

	if ((m_sidepane == Sidepane::ADD) || ((m_sidepane == Sidepane::EDITOR) && (m_network.getCurrentAtom() == nullptr))) {
		g.setColour(MID_LAYER);
		//g.fillRect(0, 0, C::GUI_WIDTH, C::VST_HEIGHT);
	}
}

void AtomSynthEditor::paintOverChildren(Graphics & g) {
	if (m_sidepane == Sidepane::NOTHING) {
		g.setColour(MID_LAYER);
		g.fillRoundedRectangle(-C::CORNER_SIZE, -C::CORNER_SIZE, C::BUTTON_TRAY_WIDTH + C::CORNER_SIZE, C::SPACING + C::HEADER_HEIGHT + C::SPACING + C::CORNER_SIZE, C::CORNER_SIZE);
	} else {
		g.setColour(MID_LAYER);
		g.fillRect(0, 0, C::GUI_WIDTH, C::SPACING + C::HEADER_HEIGHT + C::SPACING);
		g.setColour(BACK_LAYER);
		g.fillRoundedRectangle(C::SPACING, C::SPACING, C::HEADER_WIDTH, C::HEADER_HEIGHT, C::CORNER_SIZE);
		g.setColour(MID_LAYER);
		g.setFont(C::LARGE_FONT);
		std::string title;
		if (m_sidepane == Sidepane::EDITOR) {
			if (m_network.getCurrentAtom() != nullptr) {
				title = m_network.getCurrentAtom()->getName();
			} else {
				title = "Patch Properties";
			}
		} else if (m_sidepane == Sidepane::ADD) {
			title = "Add Atom";
		}
		g.drawFittedText(title, C::SPACING, C::SPACING, C::HEADER_WIDTH, C::HEADER_HEIGHT, Justification::centred, 1, C::MIN_TEXT_SCALE);
	}

	//Don't do this at home, kids.
	Image i = Image(Image::ARGB, m_buttons.getWidth(), m_buttons.getHeight(), true);
	Graphics * ig = new Graphics(i);
	m_buttons.paintEntireComponent(*ig, true);
	g.drawImageAt(i, m_buttons.getX(), m_buttons.getY(), false);
}

void AtomSynthEditor::updateScreen() {
	repaint();
}

void AtomSynthEditor::currentAtomChanged(AtomController * oldController, AtomController * newController) {
	if (m_sidepane == Sidepane::ADD) {
		m_sidepane = Sidepane::EDITOR;
		m_addAtom.setVisible(false);
		repaint();
	}
	if (m_sidepane == Sidepane::EDITOR) {
		if(oldController == nullptr) {
			m_properties.setVisible(false);
		} else {
			oldController->getGui().setVisible(false);
		}
	}
	if (m_sidepane == Sidepane::EDITOR) {
		if(newController == nullptr) {
			m_properties.setVisible(true);
		} else {
			newController->getGui().setVisible(true);
			newController->getGui().setAlwaysOnTop(true);
		}
	}
}

void AtomSynthEditor::textButtonPressed(TextButton * button) {
	createNewAtom(button->getText());
}

} /* namespace AtomSynth */
