/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2011 Yannick Loriot.
 * http://yannickloriot.com
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
 *
 *
 * converted to c++ / cocos2d-x by Angus C
 */

#include "CCControl.h"
#include "CCDirector.h"
#include "menu_nodes/CCMenu.h"
#include "event_dispatcher/CCTouch.h"
#include "CCInvocation.h"
#include "event_dispatcher/CCEventDispatcher.h"
#include "event_dispatcher/CCTouchEventListener.h"

NS_CC_EXT_BEGIN

Control::Control()
: _enabled(false)
, _selected(false)
, _highlighted(false)
, _hasVisibleParents(false)
, _dispatchTable(NULL)
, _isOpacityModifyRGB(false)
, _state(State::NORMAL)
{

}

Control* Control::create()
{
    Control* pRet = new Control();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool Control::init()
{
    if (Layer::init())
    {
        //this->setTouchEnabled(true);
        //_isTouchEnabled=true;
        // Initialise instance variables
        _state=Control::State::NORMAL;
        setEnabled(true);
        setSelected(false);
        setHighlighted(false);

        // Initialise the tables
        _dispatchTable = new Dictionary();
        _dispatchTable->init();
        
        return true;
    }
    else
    {
        return false;
    }
}

Control::~Control()
{
    CC_SAFE_RELEASE(_dispatchTable);
}

////Menu - Events
//void Control::registerWithTouchDispatcher()
//{
//    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
//}

void Control::onEnter()
{
    Layer::onEnter();

    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
}

void Control::onExit()
{
    Layer::onExit();
}

void Control::sendActionsForControlEvents(EventType controlEvents)
{
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            // Call invocations
            // <Invocation*>
            Array* invocationList = this->dispatchListforControlEvent((Control::EventType)(1<<i));
            Object* pObj = NULL;
            CCARRAY_FOREACH(invocationList, pObj)
            {
                Invocation* invocation = static_cast<Invocation*>(pObj);
                invocation->invoke(this);
            }
            //Call ScriptFunc
            if (kScriptTypeLua == _scriptType)
            {
                cocos2d::BasicScriptData data(this,(void*)&controlEvents);
                cocos2d::ScriptEvent event(cocos2d::kControlEvent,(void*)&data);
                cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
            }
        }
    }
}
void Control::addTargetWithActionForControlEvents(Object* target, Handler action, EventType controlEvents)
{
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            this->addTargetWithActionForControlEvent(target, action, (EventType)(1<<i));
        }
    }
}



/**
 * Adds a target and action for a particular event to an internal dispatch 
 * table.
 * The action message may optionnaly include the sender and the event as 
 * parameters, in that order.
 * When you call this method, target is not retained.
 *
 * @param target The target object that is, the object to which the action 
 * message is sent. It cannot be nil. The target is not retained.
 * @param action A selector identifying an action message. It cannot be NULL.
 * @param controlEvent A control event for which the action message is sent.
 * See "CCControlEvent" for constants.
 */
void Control::addTargetWithActionForControlEvent(Object* target, Handler action, EventType controlEvent)
{    
    // Create the invocation object
    Invocation *invocation = Invocation::create(target, action, controlEvent);

    // Add the invocation into the dispatch list for the given control event
    Array* eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    eventInvocationList->addObject(invocation);    
}

void Control::removeTargetWithActionForControlEvents(Object* target, Handler action, EventType controlEvents)
{
     // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            this->removeTargetWithActionForControlEvent(target, action, (EventType)(1 << i));
        }
    }
}

void Control::removeTargetWithActionForControlEvent(Object* target, Handler action, EventType controlEvent)
{
    // Retrieve all invocations for the given control event
    //<Invocation*>
    Array *eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    
    //remove all invocations if the target and action are null
    //TODO: should the invocations be deleted, or just removed from the array? Won't that cause issues if you add a single invocation for multiple events?
    bool bDeleteObjects=true;
    if (!target && !action)
    {
        //remove objects
        eventInvocationList->removeAllObjects();
    } 
    else
    {
            //normally we would use a predicate, but this won't work here. Have to do it manually
            Object* pObj = NULL;
            CCARRAY_FOREACH(eventInvocationList, pObj)
            {
                Invocation *invocation = static_cast<Invocation*>(pObj);
                bool shouldBeRemoved=true;
                if (target)
                {
                    shouldBeRemoved=(target==invocation->getTarget());
                }
                if (action)
                {
                    shouldBeRemoved=(shouldBeRemoved && (action==invocation->getAction()));
                }
                // Remove the corresponding invocation object
                if (shouldBeRemoved)
                {
                    eventInvocationList->removeObject(invocation, bDeleteObjects);
                }
            }
    }
}


//CRGBA protocol
void Control::setOpacityModifyRGB(bool bOpacityModifyRGB)
{
    _isOpacityModifyRGB=bOpacityModifyRGB;
    Object* child;
    Array* children=getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);        
        if (pNode)
        {
            pNode->setOpacityModifyRGB(bOpacityModifyRGB);
        }
    }
}

bool Control::isOpacityModifyRGB() const
{
    return _isOpacityModifyRGB;
}


Point Control::getTouchLocation(Touch* touch)
{
    Point touchLocation = touch->getLocation();            // Get the touch position
    touchLocation = this->convertToNodeSpace(touchLocation);  // Convert to the node space of this class
    
    return touchLocation;
}

bool Control::isTouchInside(Touch* touch)
{
    Point touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    Rect bBox = getBoundingBox();
    return bBox.containsPoint(touchLocation);
}

Array* Control::dispatchListforControlEvent(EventType controlEvent)
{
    Array* invocationList = static_cast<Array*>(_dispatchTable->objectForKey((int)controlEvent));

    // If the invocation list does not exist for the  dispatch table, we create it
    if (invocationList == NULL)
    {
        invocationList = Array::createWithCapacity(1);
        _dispatchTable->setObject(invocationList, (int)controlEvent);
    }    
    return invocationList;
}

void Control::needsLayout()
{
}

void Control::setEnabled(bool bEnabled)
{
    _enabled = bEnabled;
    if(_enabled) {
        _state = Control::State::NORMAL;
    } else {
        _state = Control::State::DISABLED;
    }

    this->needsLayout();
}

bool Control::isEnabled() const
{
    return _enabled;
}

void Control::setSelected(bool bSelected)
{
    _selected = bSelected;
    this->needsLayout();
}

bool Control::isSelected() const
{
    return _selected;
}

void Control::setHighlighted(bool bHighlighted)
{
    _highlighted = bHighlighted;
    this->needsLayout();
}

bool Control::isHighlighted() const
{
    return _highlighted;
}

bool Control::hasVisibleParents() const
{
    auto parent = this->getParent();
    for( auto c = parent; c != NULL; c = c->getParent() )
    {
        if( !c->isVisible() )
        {
            return false;
        }
    }
    return true;
}
NS_CC_EXT_END
