/*
 * GuiManager.cpp
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#include "GuiManager.h"

namespace AtomSynth {

GuiManager::GuiManager() {

}

GuiManager::~GuiManager() {

}

void GuiManager::setup() {
	setReloadGuis();
}

void GuiManager::openRightClickMenu(Component* newRightClickMenu) {
	if (m_rootComponent == nullptr) {
		return;
	}

	if (m_currentRightClickMenu != nullptr) {
		closeRightClickMenu();
	}

	if (newRightClickMenu != nullptr) {
		m_currentRightClickMenu = newRightClickMenu;
		m_rootComponent->addAndMakeVisible(m_currentRightClickMenu);
		m_currentRightClickMenu->setAlwaysOnTop(true);
	}
}

void GuiManager::closeRightClickMenu() {
	if ((m_rootComponent == nullptr) || (m_currentRightClickMenu == nullptr)) {
		return;
	}

	m_rootComponent->removeChildComponent(m_currentRightClickMenu);
	delete (m_currentRightClickMenu);
	m_currentRightClickMenu = nullptr;
}

void GuiManager::mouseDown(const MouseEvent & event) {
	if ((m_rootComponent != nullptr) && (m_currentRightClickMenu != nullptr)) {
		if (!((event.originalComponent == m_currentRightClickMenu) || (m_currentRightClickMenu->isParentOf(event.originalComponent)))) {
			closeRightClickMenu();
		}
	}
}

} /* namespace AtomSynth */
