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

struct Deck
{
public:
	static void Init(Deck* io_deck);
	static void Shuffle(Deck* io_deck);

	static const char* GetSuitName(ECardSuit i_suit);
	static const char* GetRankName(ECardRank i_rank);

	static void Print(const Deck* i_deck);

public:
	static constexpr uint8_t NUM_CARDS_IN_DECK = 52;

	Card cards[NUM_CARDS_IN_DECK];
};

#endif // CARDS_H_
