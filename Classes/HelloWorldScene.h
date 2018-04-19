#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Hearts.h"

// Forward declarations
class Turn;
struct Card;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(HelloWorld);

protected:    
    virtual bool init();

public:
	void FinishedDealingCards();
	void FinishedPlayingCard(uint8_t i_player_id, const Card& i_card);
	void FinishedTurn(const Turn& i_turn);
	void FinishedRound();

private:
	void StartRound(float dt);
	void StartTurn(float dt);
	void PlayCard(float dt);

	void InitCardSprites();
	void UpdateCardSpritesForPlayer(uint8_t i_player_id);
	cocos2d::Sprite* GetSpriteForCard(const Card& i_card) const;
	cocos2d::Vec2 GetHandPositionForCard(uint8_t i_player_id, uint8_t i_card_id) const;

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
	cocos2d::Vector<cocos2d::Sprite*>			card_sprites_;

};

#endif // __HELLOWORLD_SCENE_H__
