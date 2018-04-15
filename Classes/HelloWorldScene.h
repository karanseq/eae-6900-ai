#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Cards.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloWorld);

private:
	void initBootstrap();
	void printDeck();

private:
	Deck deck_;

};

#endif // __HELLOWORLD_SCENE_H__
