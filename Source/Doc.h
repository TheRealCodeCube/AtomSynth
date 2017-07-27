/*
 * Doc.h
 *
 *  Created on: Jul 24, 2017
 *      Author: josh
 */

#ifndef SOURCE_DOC_H_
#define SOURCE_DOC_H_

/**
 * @defgroup Adsp
 * Various utilities for digital signal processing.
 */

//The following is because the documentation for Juce is not included, and I don't like explaining the built in Juce methods every time I use them.
class Graphics;
class MouseEvent;
class KeyPress;
enum FocusChangeType { };
/**
 * Juce component.
 */
class Component {
public:
	/**
	 * Juce paint function.
	 * @param g Juce Graphics object.
	 */
	virtual void paint(Graphics & g);
	/**
	 * Juce paintOverChildren function.
	 * @param g Juce Graphics object.
	 */
	virtual void paintOverChildren(Graphics & g);
	/**
	 * Juce mouseDown event.
	 * @param event Juce MouseEvent object.
	 */
	virtual void mouseDown(const MouseEvent & event);
	/**
	 * Juce mouseDrag event.
	 * @param event Juce MouseEvent object.
	 */
	virtual void mouseDrag(const MouseEvent & event);
	/**
	 * Juce mouseUp event.
	 * @param event Juce MouseEvent object.
	 */
	virtual void mouseUp(const MouseEvent & event);

	/**
	 * Juce key pressed event.
	 * @param key The key that was pressed.
	 * @param originatingComponent IDK.
	 * @return Whether or not the key event should be consumed.
	 */
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent);
	/**
	 * Juce keyPressed event.
	 * @param key Juce KeyPress object.
	 * @return True if the key event shoulod be consumed.
	 */
	virtual bool keyPressed(const KeyPress & key);
	/**
	 * Does... something.
	 * @param isKeyDown A thing.
	 * @param originatingComponent A variable.
	 * @return A boolean.
	 */
	virtual bool keyStateChanged(bool isKeyDown, Component * originatingComponent);
	/**
	 * Juce focusLost event.
	 * @param cause Enum stating the cause of losing focus.
	 */
	virtual void focusLost(FocusChangeType cause);
};

#endif /* SOURCE_DOC_H_ */
