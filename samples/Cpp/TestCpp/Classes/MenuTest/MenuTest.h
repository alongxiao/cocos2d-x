#ifndef _MENU_TEST_H_
#define _MENU_TEST_H_

////----#include "cocos2d.h"
#include "../testBasic.h"

class MenuLayerMainMenu : public Layer
{
protected:
    MenuItem*    _disabledItem;

public:
    MenuLayerMainMenu(void);
    ~MenuLayerMainMenu();

public:
    virtual bool onTouchBegan(Touch *touch, Event * event);
    virtual void onTouchEnded(Touch *touch, Event * event);
    virtual void onTouchCancelled(Touch *touch, Event * event);
    virtual void onTouchMoved(Touch *touch, Event * event);

    void allowTouches(float dt);
    void menuCallback(Object* sender);
    void menuCallbackConfig(Object* sender);
    void menuCallbackDisabled(Object* sender);
    void menuCallback2(Object* sender);
    void menuCallbackPriorityTest(Object* sender);
    void menuCallbackBugsTest(Object *pSender);
    void onQuit(Object* sender);
    void menuMovingCallback(Object *pSender);

    //CREATE_NODE(MenuLayer1);
};

class MenuLayer2 : public Layer
{
protected:
    Point        _centeredMenu;
    bool                _alignedH; 

    void alignMenusH();
    void alignMenusV();

public:
    MenuLayer2(void);
    ~MenuLayer2();

public:
    void menuCallback(Object* sender);
    void menuCallbackOpacity(Object* sender);
    void menuCallbackAlign(Object* sender);

    //CREATE_NODE(MenuLayer2);
};

class MenuLayer3 : public Layer
{
protected:
    MenuItem*    _disabledItem;

public:
    MenuLayer3(void);
    ~MenuLayer3();

    //CREATE_NODE(MenuLayer3);
};

class MenuLayer4 : public Layer
{
public:
    MenuLayer4(void);
    ~MenuLayer4();

public:
    void menuCallback(Object* sender);
    void backCallback(Object* sender);

    //CREATE_NODE(MenuLayer4);
};

class MenuLayerPriorityTest : public Layer
{
public:
    MenuLayerPriorityTest();
    ~MenuLayerPriorityTest();

    void menuCallback(Object* sender);
private:
    Menu* _menu1;
    Menu* _menu2;
    bool _priority;
};

class BugsTest : public Layer
{
public:
    BugsTest();
    
    void issue1410MenuCallback(Object *pSender);
    void issue1410v2MenuCallback(Object *pSender);
    void backMenuCallback(Object *pSender);
};

class RemoveMenuItemWhenMove : public Layer
{
public:
    RemoveMenuItemWhenMove();
    ~RemoveMenuItemWhenMove();
    
    virtual void onRegisterTouchListener() override;
//    virtual void registerWithTouchDispatcher(void);
    virtual bool onTouchBegan(Touch  *touch, Event  *event);
    virtual void onTouchMoved(Touch  *touch, Event  *event);
    
    void goBack(Object *pSender);
    
private:
    MenuItemFont *item;
};


class MenuTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

#endif
