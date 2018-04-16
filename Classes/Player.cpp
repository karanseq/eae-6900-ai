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
}

void Player::Print() const
{
	CCLOG("Player-%d", id_);
	CCLOG("Score:%d", score_);
	for (const auto& card : hand_)
	{
		Deck::PrintCard(card);
	}
}

Card Player::PlayCardForCurrentTurn(const Turn& i_turn)
{
	uint8_t playing_card_index = Hearts::NUM_CARDS_PER_PLAYER;

	// Am I leading this turn?
	if (i_turn.GetNumCardsPlayed() == 0)
	{
		// Is this the first turn?
		if (i_turn.GetId() == 0)
		{
			// I must have the two of clubs
			playing_card_index = 0;
		}
		else
		{
			// What's the safest card I can discard
			const uint8_t safest_card_index = FindSafestCardForTurn(i_turn);
			playing_card_index = safest_card_index < Hearts::NUM_CARDS_PER_PLAYER ? safest_card_index : playing_card_index;
		}
	}
	else
	{
		// Get the cards that have been played
		const std::vector<Card>& cards_played = i_turn.GetCardsPlayed();
		
		// What was the leading card?
		ECardSuit leading_card_suit = cards_played[0].suit;
		ECardRank leading_card_rank = cards_played[0].rank;

		// Do I have a card that matches the leading suit?
		const auto last_matching_card = std::find(hand_.begin(), hand_.end(), leading_card_suit);
		if (last_matching_card != hand_.end())
		{
			playing_card_index = last_matching_card - hand_.begin();
		}
	}

	if (playing_card_index >= Hearts::NUM_CARDS_PER_PLAYER)
	{
		CCLOGERROR("Player-%d couldn't find a suitable card for the following turn!", id_);
		i_turn.Print();
		return Card::INVALID;
	}
	else
	{
		// Copy the card before discarding it
		Card playing_card = hand_[playing_card_index];

		// Discard this card
		hand_.erase(hand_.begin() + playing_card_index);

		return playing_card;
	}
}

uint8_t Player::FindSafestCardForTurn(const Turn& i_turn) const
{
	return rand() % hand_.size();
}

//============================================================================
// Turn

Turn::Turn(uint8_t i_id, const uint8_t* i_order) :
	id_(i_id)
{
	memcpy_s(order_, sizeof(uint8_t) * Player::NUM_PLAYERS, i_order, sizeof(uint8_t) * Player::NUM_PLAYERS);
	cards_.reserve(Player::NUM_PLAYERS);
}

void Turn::Print() const
{
	CCLOG("Turn-%d", id_);
	CCLOG("Order:%d, %d, %d, %d", order_[0], order_[1], order_[2], order_[3]);
	CCLOG("Loser:%d", loser_id_);
	for (const auto& card : cards_)
	{
		Deck::PrintCard(card);
	}
}

void Turn::FindLoser()
{
	if (cards_.size() < Player::NUM_PLAYERS)
	{
		CCLOGERROR("FindLoser has been called but the turn hasn't ended yet!");
		return;
	}
}
