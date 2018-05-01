/* This file is part of the KDE project
 * Copyright (C) 2012 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KIS_ABSTRACT_INPUT_ACTION_H
#define KIS_ABSTRACT_INPUT_ACTION_H

#include <QHash>
#include <QPoint>
#include "kritaui_export.h"

class QPointF;
class QEvent;
class KisInputManager;

/**
 * \brief Abstract base class for input actions.
 *
 * Input actions represent actions to be performed when interacting
 * with the canvas. They are managed by KisInputManager and activated
 * when KisKeyShortcut or KisStrokeShortcut detects it matches a certain
 * set of inputs.
 *
 * The begin() method uses an index for the type of behaviour to activate.
 * This index can be used to trigger behaviour when different events occur.
 *
 * The events can be of two types:
 * 1) Key events. The input manager calls begin() and end() sequentially
 *    with an \p index parameter to begin() representing the type of
 *    action that should be performed. The \p event parameter of both
 *    calls in null.
 * 2) Stroke events. The input manager calls begin() and end() on the
 *    corresponding mouse down and up events. The \p event parameter
 *    will be of QMouseEvent type, representing the event happened.
 *    All the mouse move events between begin() and end() will be
 *    redirected to the inputEvent() method.
 */
class KRITAUI_EXPORT KisAbstractInputAction
{
public:
    /**
     * Constructor.
     *
     * \param manager The InputManager this action belongs to.
     */
    explicit KisAbstractInputAction(const QString &id);
    /**
     * Destructor.
     */
    virtual ~KisAbstractInputAction();

    /**
     * The method is called when the action is yet to be started,
     * that is, e.g. the user has pressed all the modifiers for the
     * action but hasn't started painting yet. This method is a right
     * place to show the user what is going to happen, e.g. change the
     * cursor.
     */
    virtual void activate(int shortcut);

    /**
     * The method is called when the action is not a candidate for
     * the starting anymore. The action should revert everything that
     * was done in activate() method.
     *
     * \see activate()
     */
    virtual void deactivate(int shortcut);

    /**
     * Begin the action.
     *
     * \param shortcut The index of the behaviour to trigger.
     * \param event The mouse event that has triggered this action.
     *              Is null for keyboard-activated actions.
     */
    virtual void begin(int shortcut, QEvent *event);
    /**
     * End the action.
     * \param event The mouse event that has finished this action.
     *              Is null for keyboard-activated actions.
     */
    virtual void end(QEvent *event);
    /**
     * Process an input event.
     *
     * By default handles MouseMove events and passes the data to
     * a convenience cursorMoved() method
     *
     * \param event An event to process.
     */
    virtual void inputEvent(QEvent* event);

    /**
     * Returns true if the action can handle HiRes flow of move events
     * which is generated by the tablet. If the function returns
     * false, some of the events will be dropped or postponed. For
     * most of the actions in Krita (except of real painting) it is
     * perfectly acceptable, so 'false' is the default value.
     */
    virtual bool supportsHiResInputEvents() const;

    /**
     * The indexes of shortcut behaviours available.
     */
    virtual QHash<QString, int> shortcutIndexes() const;

    /**
     * The id of this action.
     */
    virtual QString id() const;

    /**
     * The translated name of this action.
     */
    virtual QString name() const;

    /**
     * A short description of this action.
     */
    virtual QString description() const;

    /**
     * The priority for this action.
     *
     * Priority determines how "important" the action is and is used
     * to resolve conflicts when multiple actions can be activated.
     */
    virtual int priority() const;

    /**
     * Returns true if an action can run with any modifiers pressed
     * (the shortcut's modifiers list must be empty for that). That is
     * used for making one type of actions default one.
     */
    virtual bool canIgnoreModifiers() const;

    /**
     * Return true when the specified shortcut is required for basic
     * user interaction. This is used by the configuration system to
     * prevent basic actions like painting from being removed.
     *
     * \param shortcut The shortcut index to check.
     * \return True if the shortcut is required, false if not.
     */
    virtual bool isShortcutRequired(int shortcut) const;

    /**
     * Some of the actions are available in particular sutiations
     * only.  E.g. switch frame action is available iff a animated
     * layer is selected. If isAvailable() returns true then the
     * action will *not* be triggered by the shortcut matcher.
     */
    virtual bool isAvailable() const;

protected:
    /**
     * The input manager this action belongs to.
     */
    KisInputManager *inputManager() const;
    /**
     * Set the name of this action.
     *
     * \param name The new name.
     */
    void setName(const QString &name);
    /**
     * Set the description of this action.
     *
     * \param description The new description.
     */
    void setDescription(const QString &description);
    /**
     * Set the available indexes of shortcut behaviours.
     *
     * \param indexes The new indexes.
     */
    void setShortcutIndexes(const QHash<QString, int> &indexes);

    /**
     * Convenience method for handling cursor movement for tablet, mouse and touch.
     * The default implementation of inputEvent calls this function.
     */
    virtual void cursorMoved(const QPointF &lastPos, const QPointF &pos);

    /**
     * Convenience method to extract the position from a cursor movement event.
     *
     * \param event A mouse or tablet event.
     */
    QPoint eventPos(const QEvent *event);

    /**
     * Convenience method to extract the floating point position from a
     * cursor movement event.
     *
     * \param event A mouse or tablet event.
     */
    QPointF eventPosF(const QEvent *event);

private:
    friend class KisInputManager;
    static void setInputManager(KisInputManager *manager);

    class Private;
    Private * const d;
};

#endif // KIS_ABSTRACT_INPUT_ACTION_H