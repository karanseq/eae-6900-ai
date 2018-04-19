#include "HelloWorldScene.h"

// Engine includes
#include "SimpleAudioEngine.h"

// Game includes
#include "Cards.h"
#include "Player.h"

USING_NS_CC;

constexpr char* TTF_FONT_PATH = "fonts/Marker Felt.ttf";
constexpr float DELAY_BEFORE_STARTING_ROUND = 0.1f;//1.0f;
constexpr float DELAY_BEFORE_STARTING_TURN = 0.1f;//1.5f;
constexpr float DELAY_BEFORE_PLAYING_CARD = 0.1f;

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

	InitCardSprites();

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
	//scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartRound), DELAY_BEFORE_STARTING_ROUND);
	Director::getInstance()->end();
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

void HelloWorld::InitCardSprites()
{
	card_sprites_.reserve(size_t(ECardSuit::Invalid) * size_t(ECardRank::Invalid));
	card_sprites_.pushBack(Sprite::create("cards/2_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/3_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/4_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/5_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/6_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/7_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/8_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/9_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/10_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/jack_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/queen_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/king_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/ace_of_clubs.png"));
	card_sprites_.pushBack(Sprite::create("cards/2_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/3_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/4_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/5_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/6_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/7_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/8_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/9_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/10_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/jack_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/queen_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/king_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/ace_of_diamonds.png"));
	card_sprites_.pushBack(Sprite::create("cards/2_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/3_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/4_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/5_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/6_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/7_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/8_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/9_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/10_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/jack_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/queen_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/king_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/ace_of_hearts.png"));
	card_sprites_.pushBack(Sprite::create("cards/2_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/3_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/4_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/5_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/6_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/7_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/8_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/9_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/10_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/jack_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/queen_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/king_of_spades.png"));
	card_sprites_.pushBack(Sprite::create("cards/ace_of_spades.png"));

	for (Sprite*& card_sprite : card_sprites_)
	{
		card_sprite->setScale(0.5f);
	}
}

Sprite* HelloWorld::GetSpriteForCard(const Card& i_card) const
{
	return card_sprites_.at(size_t(i_card.suit) * size_t(ECardRank::Invalid) + size_t(i_card.rank));
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

	//auto sprite = Sprite::create("cards/queen_of_spades2.png");
	//if (sprite == nullptr)
	//{
	//	problemLoading("'queen_of_spades2.png'");
	//}
	//else
	//{
	//	sprite->setPosition(Vec2(visible_size_.width / 2 + origin_.x, visible_size_.height / 2 + origin_.y));
	//	this->addChild(sprite, 0);
	//}
}
