#include <gtest/gtest.h>

#include "src/LYCard.h"
#include "src/LYDeck.h"

class LYCard_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    }
    void TearDown()
    {
   }

};

TEST_F(LYCard_tests, init)
{
	ASSERT_EQ(cA.face, ACE);
	ASSERT_EQ(cA.suit, Clubs);
	ASSERT_EQ(cA.image, "c_A.png");
	ASSERT_EQ(cK.face, KING);
	ASSERT_EQ(cK.suit, Clubs);
	ASSERT_EQ(cK.image, "c_K.png");

	ASSERT_EQ(s9.face, NINE);
	ASSERT_EQ(s9.suit, Spades);
	ASSERT_EQ(s9.image, "s_9.png");

	ASSERT_EQ(h4.getFaceSymbol(), '4');
	ASSERT_EQ(h4.getSuitSymbol(), 'h');
	ASSERT_EQ(h4.image, "h_4.png");

	ASSERT_EQ(g_faces[14], 'A');
	ASSERT_EQ(g_faces[13], 'K');
	ASSERT_EQ(g_faces[12], 'Q');
	ASSERT_EQ(g_faces[11], 'J');
	ASSERT_EQ(g_faces[10], 'T');
	ASSERT_EQ(g_faces[9], '9');
	ASSERT_EQ(g_faces[8], '8');
	ASSERT_EQ(g_faces[7], '7');
	ASSERT_EQ(g_faces[6], '6');
	ASSERT_EQ(g_faces[5], '5');
	ASSERT_EQ(g_faces[4], '4');
	ASSERT_EQ(g_faces[3], '3');
	ASSERT_EQ(g_faces[2], '2');
	ASSERT_EQ(g_faces[1], 'Y');
	ASSERT_EQ(g_faces[0], 'X');
}

TEST_F(LYCard_tests, toString)
{
	ASSERT_EQ(cA.toString(), "Ac");
}

TEST_F(LYCard_tests, toInteger)
{
	ASSERT_EQ(cA.toInteger(), 4);
	ASSERT_EQ(flipCard.toInteger(), 0);
	ASSERT_EQ(d9.toInteger(), 23);
	ASSERT_EQ(s4.toInteger(), 41);
}

TEST_F(LYCard_tests, sort)
{
	ASSERT_EQ(cA < sA, true);
	ASSERT_EQ(dK > d3, true);
	ASSERT_EQ(cT > s8, true);
	ASSERT_EQ(cA == sA, false);
	LYCard cd1(cA.face, Nosuit);
	LYCard cd2(sA.face, Nosuit);
	ASSERT_EQ(cd1 == cd2, true);
}

TEST_F(LYCard_tests, ghost)
{
	ASSERT_EQ(smallGhost.isGhost(), true);
	ASSERT_EQ(bigGhost.isGhost(), true);
}
