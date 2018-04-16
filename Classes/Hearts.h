#ifndef HEARTS_H_
#define HEARTS_H_

// Library includes
#include <vector>
#include <stdint.h>

// Game includes
#include "Cards.h"
#include "Player.h"

// Forward declarations
class HelloWorld;

enum class EGameState : uint8_t
{
	None,
	DealCards,
	RoundStart,
	RoundEnd,
	TurnStart,
	TurnEnd,
};

class Hearts
{
public:
	void Init(HelloWorld* i_scene);
	void Shutdown();

	void StartRound();
	void StartTurn();
	void PlayCard();

private:
	void DealCards();

public:
	static constexpr uint8_t					NUM_CARDS_PER_PLAYER = Deck::NUM_CARDS_IN_DECK / Player::NUM_PLAYERS;
	
	inline const Deck& GetDeck() const { return deck_; }
	inline const Player* GetPlayers() const { return players_; }
	inline const std::vector<Turn>& GetTurns() const { return turns_; }

private:
	Deck										deck_;
	Player										players_[Player::NUM_PLAYERS];

	std::vector<Turn>							turns_;
	uint8_t										current_turn_id_ = 0;
	
	HelloWorld*									scene_ = nullptr;

public:
	Hearts() = default;
	~Hearts() = default;

}; // class Hearts

#endif // HEARTS_H_