#include "Player.h"

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Cards.h"
#include "Hearts.h"

//============================================================================
// Player

void Player::Init(uint8_t i_id)
{
	id_ = i_id;
	hand_.reserve(Hearts::NUM_CARDS_PER_PLAYER);
	hand_.resize(Hearts::NUM_CARDS_PER_PLAYER, nullptr);
}

void Player::Print() const
{
	CCLOG("Player-%d", id_);
	CCLOG("Score:%d", score_);
	for (const auto& card : hand_)
	{
		Deck::PrintCard(*card);
	}
}

//============================================================================
// Turn

void Turn::Init(uint8_t i_id, const uint8_t* i_order)
{
	id_ = i_id;
	memcpy_s(order_, sizeof(uint8_t) * Player::NUM_PLAYERS, i_order, sizeof(uint8_t) * Player::NUM_PLAYERS);
	cards_.reserve(Player::NUM_PLAYERS);
	cards_.resize(Player::NUM_PLAYERS, nullptr);
}

void Turn::Print() const
{
	CCLOG("Turn-%d", id_);
	CCLOG("Order:%d, %d, %d, %d", order_[0], order_[1], order_[2], order_[3]);
	CCLOG("Loser:%d", loser_id_);
	for (const auto& card : cards_)
	{
		Deck::PrintCard(*card);
	}
}
