/*
 * GuiManager.cpp
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#include "GuiManager.h"

#include <chrono>

namespace AtomSynth {

MessageOverlayRepaintThread::MessageOverlayRepaintThread(MessageOverlay* parent):
		Thread("messageOverlayRepaintThread"),
		m_parent(parent) {

}

MessageOverlayRepaintThread::~MessageOverlayRepaintThread() {

}

void MessageOverlayRepaintThread::run() {
	while(!threadShouldExit()) {
		m_parent->repaintAsync();
		sleep(100);
	}
}

MessageOverlay::MessageOverlay():
		m_repaintThread(this) {
	m_repaintThread.startThread();
}

MessageOverlay::~MessageOverlay() {
	m_repaintThread.stopThread(110);
}

void MessageOverlay::setParent(GuiManager* parent) {
	m_parent = parent;
}

void MessageOverlay::paintOverChildren(Graphics& g) {
	if(m_parent == nullptr) return;
	m_parent->clearOldMessages();
	g.setColour(FORE_LAYER);
	g.setFont(C::DEFAULT_FONT);
	double y = getHeight();
	for(auto& message : m_parent->getMessages()) {
		y -= C::FONT_SIZE + C::SPACING;
		float alpha = std::min(double(message.second - m_parent->getTime()) / 1000.0, 1.0);
		g.setColour(Colour((uint8) 255, 255, 255, alpha));
		g.drawFittedText(message.first, C::GUI_WIDTH + C::SPACING, y, 999, C::FONT_SIZE, Justification::centredLeft, 1, C::MIN_TEXT_SCALE);
	}
}

void GuiManager::setup() {
	setReloadGuis();
}

unsigned int GuiManager::getTime() {
	//Yuck.
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

GuiManager::GuiManager() {

}

GuiManager::~GuiManager() {

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
void GuiManager::setRootComponent(Component* newRootComponent) {
	if(m_rootComponent != nullptr) {
		m_rootComponent = nullptr;
	}
	if(newRootComponent == nullptr) return;
	m_rootComponent = newRootComponent;
	Component * oldParent = m_rootComponent->getParentComponent();
	MessageOverlay * newParent = new MessageOverlay();
	newParent->setParent(this);
	newParent->setBounds(m_rootComponent->getBounds());
	newParent->addAndMakeVisible(m_rootComponent);
	m_rootComponent = newParent;
	oldParent->addAndMakeVisible(newParent);
}
void GuiManager::repaintRootComponent() {
	if(m_rootComponent != nullptr) {
		m_rootComponent->repaint();
	}
}

void GuiManager::mouseDown(const MouseEvent & event) {
	if ((m_rootComponent != nullptr) && (m_currentRightClickMenu != nullptr)) {
		if (!((event.originalComponent == m_currentRightClickMenu) || (m_currentRightClickMenu->isParentOf(event.originalComponent)))) {
			closeRightClickMenu();
		}
	}
}

void GuiManager::addMessage(std::string message) {
	m_messages.push_back(std::pair<std::string, unsigned int>(message, getTime() + 5 * 1000));
}

void GuiManager::clearOldMessages() {
	for(int i = m_messages.size() - 1; i >= 0; i--) {
		if(m_messages[i].second <= getTime()) {
			m_messages.erase(m_messages.begin() + i);
		}
	}
}

} /* namespace AtomSynth */
