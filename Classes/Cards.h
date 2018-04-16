#ifndef CARDS_H_
#define CARDS_H_

// Library includes
#include <stdint.h>

enum class ECardSuit : uint8_t
{
	Clubs,
	Diamonds,
	Hearts,
	Spades
};

enum class ECardRank : uint8_t
{
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace
};

struct Card
{
	ECardSuit suit;
	ECardRank rank;
};

class Deck
{
public:
	Deck() = default;
	~Deck() = default;

	void Init();
	void Shuffle();

	void Print() const;

	static const char* GetSuitName(ECardSuit i_suit);
	static const char* GetRankName(ECardRank i_rank);
	static void PrintCard(const Card& i_card);

public:
	static constexpr uint8_t					NUM_CARDS_IN_DECK = 52;

	inline const Card* GetCardAt(uint8_t i_index) const { return i_index < NUM_CARDS_IN_DECK ? &cards_[i_index] : nullptr; }

private:
	Card										cards_[NUM_CARDS_IN_DECK];

};

#endif // CARDS_H_
