#include "HelloWorldScene.h"

// Engine includes
#include "SimpleAudioEngine.h"

// Game includes
#include "Cards.h"
#include "Player.h"

USING_NS_CC;

constexpr char* TTF_FONT_PATH = "fonts/Marker Felt.ttf";
constexpr float DELAY_BEFORE_STARTING_ROUND = 2.5f;//0.1f;//1.0f;
constexpr float DELAY_BEFORE_STARTING_TURN = 1.5f;//0.1f;//1.5f;
constexpr float DELAY_BEFORE_PLAYING_CARD = 0.5f;
constexpr float CARD_MOVE_DURATION = 0.25f;

const Vec2 PLAYER_HAND_START_POSITIONS[Player::NUM_PLAYERS] = {
	Vec2(320.0f, 0.0f),
	Vec2(1280.0f, 200.0f),
	Vec2(960.0f, 800.0f),
	Vec2(0.0f, 600.0f)
};

const Vec2 PLAYER_HAND_MIDDLE_POSITIONS[Player::NUM_PLAYERS] = {
	Vec2(640.0f, 0.0f),
	Vec2(1280.0f, 400.0f),
	Vec2(640.0f, 800.0f),
	Vec2(0.0f, 400.0f)
};

const Vec2 PLAYER_PLAYED_CARD_POSITIONS[Player::NUM_PLAYERS] = {
	Vec2(640.0f, 250.0f),
	Vec2(960.0f, 400.0f),
	Vec2(640.0f, 550.0f),
	Vec2(320.0f, 400.0f)
};

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
	StartRound(0.0f);

    return true;
}

void HelloWorld::FinishedDealingCards()
{
	//CCLOG(__FUNCTION__);
	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		UpdateCardSpritesForPlayer(i);
	}

	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartTurn), DELAY_BEFORE_STARTING_TURN);
}

void HelloWorld::FinishedPlayingCard(uint8_t i_player_id, const Card& i_card)
{
	//CCLOG(__FUNCTION__);
	if (i_player_id < Player::NUM_PLAYERS &&
		i_card != Card::INVALID)
	{
		// Get data about the card
		const uint8_t card_sprite_index = Deck::GetCardIndex(i_card);
		Sprite* card_sprite = card_sprites_.at(card_sprite_index);

		// Update the card
		EaseSineOut* move_to = EaseSineOut::create(MoveTo::create(CARD_MOVE_DURATION, PLAYER_PLAYED_CARD_POSITIONS[i_player_id]));
		EaseSineOut* rotate_to = EaseSineOut::create(RotateTo::create(CARD_MOVE_DURATION, 0.0f));
		Spawn* move_and_rotate = Spawn::createWithTwoActions(move_to, rotate_to);
		card_sprite->runAction(move_and_rotate);

		// Update the other cards
		UpdateCardSpritesForPlayer(i_player_id);
	}

	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::PlayCard), DELAY_BEFORE_PLAYING_CARD);
}

void HelloWorld::FinishedTurn(const Turn& i_turn)
{
	//CCLOG(__FUNCTION__);
	const std::vector<Card>& cards_played = i_turn.GetCardsPlayed();
	const uint8_t loser_id = i_turn.GetLoser();

	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		// Get data about the card
		const uint8_t card_sprite_index = Deck::GetCardIndex(cards_played[i]);
		Sprite* card_sprite = card_sprites_.at(card_sprite_index);

		// Update the card
		DelayTime* wait = DelayTime::create(DELAY_BEFORE_STARTING_TURN - CARD_MOVE_DURATION);
		EaseSineOut* move_to = EaseSineOut::create(MoveTo::create(CARD_MOVE_DURATION, PLAYER_HAND_MIDDLE_POSITIONS[loser_id]));
		RemoveSelf* remove_self = RemoveSelf::create();
		Sequence* move_then_remove = Sequence::create({ wait, move_to, remove_self });
		card_sprite->runAction(move_then_remove);
	}

	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartTurn), DELAY_BEFORE_STARTING_TURN);
}

void HelloWorld::FinishedRound()
{
	uint8_t max_score = 0;

	// Display scores
	Layer* score_layer = Layer::create();
	{
		Vec2 position(640.0f, 550.0f);

		for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
		{
			const uint8_t score = hearts_.GetPlayers()[i].GetScore();
			max_score = score > max_score ? score : max_score;

			std::string score_string;
			score_string.reserve(16);
			score_string = std::string("Player-") + std::to_string(i) + std::string(": ") + std::to_string(score);

			Label* score_label = Label::createWithTTF(score_string, TTF_FONT_PATH, 24);
			score_label->setPosition(position);
			score_layer->addChild(score_label);

			position.y -= 75.0f;
		}
	}
	score_layer->runAction(Sequence::createWithTwoActions(DelayTime::create(DELAY_BEFORE_STARTING_ROUND), RemoveSelf::create()));
	addChild(score_layer, 250);

	// Check if game should continue
	max_score_reached_ = max_score >= 100;
	scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::StartRound), DELAY_BEFORE_STARTING_ROUND);
}

void HelloWorld::StartRound(float dt)
{
	//CCLOG(__FUNCTION__);
	if (max_score_reached_ == true)
	{
		Director::getInstance()->end();
	}
	else
	{
		hearts_.StartRound();
	}
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

void HelloWorld::UpdateCardSpritesForPlayer(uint8_t i_player_id)
{
	const std::vector<Card>& player_hand = hearts_.GetPlayers()[i_player_id].GetCardsInHand();

	const float card_rotation = i_player_id * 90.0f;

	for (uint8_t j = 0; j < player_hand.size(); ++j)
	{
		const uint8_t card_sprite_index = Deck::GetCardIndex(player_hand[j]);
		Sprite* card_sprite = card_sprites_.at(card_sprite_index);
		card_sprite->setPosition(GetHandPositionForCard(i_player_id, j));
		card_sprite->setRotation(card_rotation);

		if (card_sprite->getParent() == nullptr)
		{
			addChild(card_sprite, 100 + j);
		}
	}
}

Sprite* HelloWorld::GetSpriteForCard(const Card& i_card) const
{
	return card_sprites_.at(Deck::GetCardIndex(i_card));
}

Vec2 HelloWorld::GetHandPositionForCard(uint8_t i_player_id, uint8_t i_card_id) const
{
	const float card_position_stride_x = (i_player_id == 0 ? 50.0f : (i_player_id == 2 ? -50.0f : 0.0f));
	const float card_position_stride_y = (i_player_id == 1 ? 50.0f : (i_player_id == 3 ? -50.0f : 0.0f));
	const Vec2 card_stride(card_position_stride_x * i_card_id, card_position_stride_y * i_card_id);

	return PLAYER_HAND_START_POSITIONS[i_player_id] + card_stride;
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

	auto bg = LayerColor::create(Color4B(0, 51, 102, 255));
	addChild(bg, 0);

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
}
