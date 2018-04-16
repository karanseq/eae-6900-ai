#ifndef PLAYER_H_
#define PLAYER_H_

// Library includes
#include <stdint.h>
#include <vector>

// Forward declarations
struct Card;

//============================================================================
// Player

class Player
{
public:
	Player() = default;
	~Player() = default;

	void Init(uint8_t i_id);
	void Print() const;

public:
	static constexpr uint8_t	NUM_PLAYERS = 4;

	inline uint8_t GetId() const { return id_; }
	inline uint8_t GetScore() const { return score_; }
	inline const std::vector<const Card*>& GetCardsInHand() const { return hand_; }
	inline std::vector<const Card*>& GetCardsInHand() { return hand_; }

private:
	uint8_t										id_ = 0;
	uint16_t									score_ = 0;
	std::vector<const Card*>					hand_;

};

//============================================================================
// Turn

class Turn
{
public:
	Turn() = default;
	~Turn() = default;

	void Init(uint8_t i_id, const uint8_t* i_order);
	void Print() const;

public:
	inline const uint8_t* GetOrder() const { return order_; }
	inline uint8_t GetLoser() const { return loser_id_; }
	inline const std::vector<const Card*>& GetCardsPlayed() const { return cards_; }
	inline std::vector<const Card*>& GetCardsPlayed() { return cards_; }

private:
	uint8_t										order_[Player::NUM_PLAYERS];
	uint8_t										id_ = 0;
	uint8_t										loser_id_ = 0;
	std::vector<const Card*>					cards_;

};

#endif // PLAYER_H_