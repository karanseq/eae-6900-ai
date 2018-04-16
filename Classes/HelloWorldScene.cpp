#include "HelloWorldScene.h"

// Engine includes
#include "SimpleAudioEngine.h"

// Game includes
#include "Cards.h"
#include "Player.h"

USING_NS_CC;

constexpr char* TTF_FONT_PATH = "fonts/Marker Felt.ttf";
constexpr float DELAY_BEFORE_STARTING_ROUND = 0.25f;//1.0f;
constexpr float DELAY_BEFORE_STARTING_TURN = 0.25;//1.5f;
constexpr float DELAY_BEFORE_PLAYING_CARD = 0.25f;

const Vec2 PLAYER0_PLAYED_CARD_POSITION = Vec2(640.0f, 200.0f);
const Vec2 PLAYER1_PLAYED_CARD_POSITION = Vec2(320.0f, 400.0f);
const Vec2 PLAYER2_PLAYED_CARD_POSITION = Vec2(640.0f, 200.0f);
const Vec2 PLAYER3_PLAYED_CARD_POSITION = Vec2(960.0f, 400.0f);

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	initBootstrap();

	hearts_.Init(this);
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartRound), DELAY_BEFORE_STARTING_ROUND);

    return true;
}

void HelloWorld::FinishedDealingCards()
{
	//CCLOG(__FUNCTION__);
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartTurn), DELAY_BEFORE_STARTING_TURN);
}

void HelloWorld::FinishedPlayingCard(const Card& i_card)
{
	//CCLOG(__FUNCTION__);
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::PlayCard), DELAY_BEFORE_PLAYING_CARD);
}

void HelloWorld::FinishedTurn(const Turn& i_turn)
{
	//CCLOG(__FUNCTION__);
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartTurn), DELAY_BEFORE_STARTING_TURN);
}

void HelloWorld::FinishedRound()
{
	//CCLOG(__FUNCTION__);
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartRound), DELAY_BEFORE_STARTING_ROUND);
}

void HelloWorld::StartRound(float dt)
{
	//CCLOG(__FUNCTION__);
	hearts_.StartRound();
}

void HelloWorld::StartTurn(float dt)
{
	//CCLOG(__FUNCTION__);
	hearts_.StartTurn();
}

void HelloWorld::PlayCard(float dt)
{
	//CCLOG(__FUNCTION__);
	hearts_.PlayCard();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::initBootstrap()
{
	visible_size_ = Director::getInstance()->getVisibleSize();
	origin_ = Director::getInstance()->getVisibleOrigin();

	MenuItemImage* closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
	);

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin_.x + visible_size_.width - closeItem->getContentSize().width / 2;
		float y = origin_.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF("Hearts", TTF_FONT_PATH, 24);
	if (label == nullptr)
	{
		problemLoading(TTF_FONT_PATH);
	}
	else
	{
		label->setPosition(Vec2(origin_.x + visible_size_.width / 2,
			origin_.y + visible_size_.height - label->getContentSize().height));

		this->addChild(label, 1);
	}

	auto sprite = Sprite::create("cards/queen_of_spades2.png");
	if (sprite == nullptr)
	{
		problemLoading("'queen_of_spades2.png'");
	}
	else
	{
		sprite->setPosition(Vec2(visible_size_.width / 2 + origin_.x, visible_size_.height / 2 + origin_.y));
		this->addChild(sprite, 0);
	}
}
