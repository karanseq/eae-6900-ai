#ifndef PLAYER_H_
#define PLAYER_H_

// Library includes
#include <stdint.h>
#include <vector>

// Game includes
#include "Cards.h"

// Forward declarations
class Turn;

//============================================================================
// Player

class Player
{
public:
	Player() = default;
	~Player() = default;

	void Init(uint8_t i_id);
	void Print() const;

	Card PlayCardForCurrentTurn(const Turn& i_turn);

	inline bool HasCard(const Card& i_card) const { return std::find(hand_.begin(), hand_.end(), i_card) != hand_.end(); }

private:
	uint8_t FindCardWhenNoCardsPlayed(const Turn& i_turn) const;
	uint8_t FindCardWhenHaveLeadingSuit(const Turn& i_turn) const;
	uint8_t FindCardWhenDontHaveLeadingSuit(const Turn& i_turn) const;

public:
	static constexpr uint8_t	NUM_PLAYERS = 4;

	inline uint8_t GetId() const { return id_; }
	inline uint8_t GetScore() const { return score_; }
	inline const std::vector<Card>& GetCardsInHand() const { return hand_; }
	inline std::vector<Card>& GetCardsInHand() { return hand_; }

private:
	uint8_t										id_ = 0;
	uint16_t									score_ = 0;
	std::vector<Card>							hand_;

};

//============================================================================
// Turn

class Turn
{
public:
	Turn(uint8_t i_id, const uint8_t* i_order);
	~Turn() = default;

	void Print() const;

	inline void AddCard(const Card& i_card) { cards_.push_back(i_card); }
	void FindLoser();

public:
	inline const uint8_t* GetOrder() const { return order_; }
	inline uint8_t GetId() const { return id_; }
	inline uint8_t GetLoser() const { return loser_id_; }
	inline const std::vector<Card>& GetCardsPlayed() const { return cards_; }
	inline std::vector<Card>& GetCardsPlayed() { return cards_; }

	inline uint8_t GetNumCardsPlayed() const { return uint8_t(cards_.size()); }
	inline ECardSuit GetLeadingCardSuit() const { return (cards_.empty() ? ECardSuit::Invalid : cards_[0].suit); }

private:
	uint8_t										order_[Player::NUM_PLAYERS];
	uint8_t										id_ = 0;
	uint8_t										loser_id_ = 0;
	std::vector<Card>							cards_;

};

#endif // PLAYER_H_