/*
 * Copyright (c) 2015 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <QTimer>
#include <QMessageBox>
#include <QApplication>

#include "GamepadManager.h"

//------------------------------------------------------------------------------
// SDL DEFINITIONS
//------------------------------------------------------------------------------

#define SDL_MAIN_HANDLED

//------------------------------------------------------------------------------
// CONSTRUCTOR/DESTRUCTOR CODE
//------------------------------------------------------------------------------

GamepadManager* GamepadManager::s_instance = Q_NULLPTR;

GamepadManager::GamepadManager()
{
    SDL_JoystickEventState (SDL_ENABLE);
    SDL_SetHint (SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (SDL_Init (SDL_INIT_JOYSTICK) != 0) {
        QMessageBox::critical (Q_NULLPTR, tr ("Fatal Error!"),
                               tr ("SDL Init Error: %1").arg (SDL_GetError()));

        exit (EXIT_FAILURE);
    }
}

GamepadManager::~GamepadManager()
{
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
        SDL_GameControllerClose (SDL_GameControllerOpen (i));

    SDL_Quit();
}

GamepadManager* GamepadManager::getInstance()
{
    if (s_instance == Q_NULLPTR)
        s_instance = new GamepadManager;

    return s_instance;
}

//------------------------------------------------------------------------------
// PROVIDE INFORMATION ABOUT A JOYSTICK
//------------------------------------------------------------------------------

int GamepadManager::getNumHats (int js)
{
    return SDL_JoystickNumHats (SDL_JoystickOpen (js));
}

int GamepadManager::getNumAxes (int js)
{
    return SDL_JoystickNumAxes (SDL_JoystickOpen (js));
}

int GamepadManager::getNumButtons (int js)
{
    return SDL_JoystickNumButtons (SDL_JoystickOpen (js));
}

QString GamepadManager::getAxisName (int axis)
{
    return QString ("Axis %1").arg (axis);
}

QString GamepadManager::getButtonName (int button)
{
    return QString ("Button %1").arg (button);
}

QString GamepadManager::getJoystickName (int js)
{
    return SDL_JoystickNameForIndex (js);
}

QStringList GamepadManager::joystickList()
{
    QStringList list;

    for (int i = 0; i < SDL_NumJoysticks(); ++i)
        list.append (getJoystickName (i));

    return list;
}

//------------------------------------------------------------------------------
// INIT-RELATED FUNCTIONS
//------------------------------------------------------------------------------

void GamepadManager::init()
{
    m_time = 50;
    m_tracker = -1;

    QTimer::singleShot (500, Qt::CoarseTimer, this, SLOT (readSdlEvents()));
}

void GamepadManager::setUpdateInterval (int time)
{
    if (time >= 0)
        m_time = time;
}

//------------------------------------------------------------------------------
// FUNCTIONS THAT CHANGE SOMETHING ON THE JOYSTICK
//------------------------------------------------------------------------------

void GamepadManager::rumble (int js, int time)
{
    SDL_InitSubSystem (SDL_INIT_HAPTIC);
    SDL_Haptic* haptic = SDL_HapticOpen (js);

    if (haptic != Q_NULLPTR) {
        SDL_HapticRumbleInit (haptic);
        SDL_HapticRumblePlay (haptic, 1, time);
    }
}

//------------------------------------------------------------------------------
// FUNCTIONS THAT GENERATE JOYSTICK INFORMATION STRUCTURES
//------------------------------------------------------------------------------

GM_Hat GamepadManager::getHat (const SDL_Event* event)
{
    GM_Hat hat;

    hat.id = event->jhat.hat;
    hat.angle = event->jhat.value;
    hat.joystick = getJoystick (event);

    return hat;
}

GM_Axis GamepadManager::getAxis (const SDL_Event* event)
{
    GM_Axis axis;

    axis.id = event->jaxis.axis;
    axis.joystick = getJoystick (event);
    axis.value = (double) (event->jaxis.value) / 32767;

    return axis;
}

GM_Button GamepadManager::getButton (const SDL_Event* event)
{
    GM_Button button;

    button.id = event->jbutton.button;
    button.joystick = getJoystick (event);
    button.pressed = event->jbutton.state == SDL_PRESSED;

    return button;
}

GM_Joystick GamepadManager::getJoystick (const SDL_Event* event)
{
    GM_Joystick stick;

    stick.id = getDynamicId (event->jdevice.which);
    stick.numAxes = getNumAxes (event->jdevice.which);
    stick.numButtons = getNumButtons (event->jdevice.which);
    stick.displayName = getJoystickName (event->jdevice.which);

    return stick;
}

//------------------------------------------------------------------------------
// UGLY HACKS
//------------------------------------------------------------------------------

int GamepadManager::getDynamicId (int id)
{
    id = m_tracker - (id + 1);
    if (id < 0) id = abs (id);
    if (id >= SDL_NumJoysticks()) id -= 1;

    return id;
}

//------------------------------------------------------------------------------
// SDL LOOP
//------------------------------------------------------------------------------

void GamepadManager::readSdlEvents()
{
    SDL_Event event;
    while (SDL_PollEvent (&event)) {
        switch (event.type) {
        case SDL_JOYDEVICEADDED:
            ++m_tracker;
            emit countChanged (joystickList());
            emit countChanged (SDL_NumJoysticks());
            break;
        case SDL_JOYDEVICEREMOVED:
            emit countChanged (joystickList());
            emit countChanged (SDL_NumJoysticks());
            break;
        case SDL_JOYAXISMOTION:
            onAxisEvent (&event);
            break;
        case SDL_JOYBUTTONUP:
            onButtonEvent (&event);
            break;
        case SDL_JOYBUTTONDOWN:
            onButtonEvent (&event);
            break;
        case SDL_JOYHATMOTION:
            onHatEvent (&event);
            break;
        }
    }

    QTimer::singleShot (m_time, this, SLOT (readSdlEvents()));
}

//------------------------------------------------------------------------------
// FUNCTIONS THAT RESPOND TO SDL EVENTS
//------------------------------------------------------------------------------

void GamepadManager::onHatEvent (const SDL_Event* event)
{
    emit hatEvent (getHat (event));
}

void GamepadManager::onAxisEvent (const SDL_Event* event)
{
    emit axisEvent (getAxis (event));
}

void GamepadManager::onButtonEvent (const SDL_Event* event)
{
    emit buttonEvent (getButton (event));
}
