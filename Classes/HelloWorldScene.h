#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Hearts.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(HelloWorld);

protected:    
    virtual bool init();
    
private:
	void DealCards(float dt);

public:
	inline const cocos2d::Size& GetVisibleSize() const { return visible_size_; }
	inline const cocos2d::Vec2& GetOrigin() const { return origin_; }

private:
	cocos2d::Size								visible_size_;
	cocos2d::Vec2								origin_;

private:
	void initBootstrap();
    void menuCloseCallback(cocos2d::Ref* pSender);

private:
	Hearts										hearts_;

};

#endif // __HELLOWORLD_SCENE_H__
