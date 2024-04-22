#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <bitset>

int is_straight(int* hand);
int is_quadsorfullhouse(int* hand);

int main()
{
    int card0, card1, card2, card3, card4;
    int hand[5];
    int straights = 0;
    int quads_or_fullhouses = 0;
    clock_t t1, t2;

    bool straight_table[1 << 15] = { false };
    bool quads_or_fullhouse_table[1 << 15] = { false };
    // 32768 bytes = 32 kB of memory used on the stack each

    for (card0 = 2; card0 <= 14; ++card0)
    {
        for (card1 = 2; card1 <= 14; ++card1)
        {
            for (card2 = 2; card2 <= 14; ++card2)
            {
                for (card3 = 2; card3 <= 14; ++card3)
                {
                    for (card4 = 2; card4 <= 14; ++card4)
                    {
                        hand[0] = card0;
                        hand[1] = card1;
                        hand[2] = card2;
                        hand[3] = card3;
                        hand[4] = card4;

                        if (is_straight(hand))
                        {
                            int bit_hand = 0;
                            bit_hand |= 1 << card0;
                            bit_hand |= 1 << card1;
                            bit_hand |= 1 << card2;
                            bit_hand |= 1 << card3;
                            bit_hand |= 1 << card4;

                            straight_table[bit_hand] = true;
                        }
                        else if (is_quadsorfullhouse(hand))
                        {
                            int bit_hand = 0;
                            bit_hand |= 1 << card0;
                            bit_hand |= 1 << card1;
                            bit_hand |= 1 << card2;
                            bit_hand |= 1 << card3;
                            bit_hand |= 1 << card4;

                            quads_or_fullhouse_table[bit_hand] = true;
                        }
                    }
                }
            }
        }
    }

    t1 = clock();
    for (card0 = 0; card0 < 52; ++card0)
    {
        for (card1 = 0; card1 < 52; ++card1)
        {
            if (card1 == card0) continue;

            for (card2 = 0; card2 < 52; ++card2)
            {
                if (card2 == card0 || card2 == card1) continue;

                for (card3 = 0; card3 < 52; ++card3)
                {
                    if (card3 == card0 || card3 == card1 || card3 == card2) continue;

                    for (card4 = 0; card4 < 52; ++card4)
                    {
                        if (card4 == card0 || card4 == card1 || card4 == card2 || card4 == card3) continue;

                        // hand[0] = (card0 % 13) + 2;
                        // hand[1] = (card1 % 13) + 2;
                        // hand[2] = (card2 % 13) + 2;
                        // hand[3] = (card3 % 13) + 2;
                        // hand[4] = (card4 % 13) + 2;

                        // if (is_straight(hand))
                        //     ++straights;

                        // if (is_quadsorfullhouse(hand))
                        //     ++quads_or_fullhouses;

                        int bit_hand = 0;
                        bit_hand |= 1 << ((card0 % 13) + 2);
                        bit_hand |= 1 << ((card1 % 13) + 2);
                        bit_hand |= 1 << ((card2 % 13) + 2);
                        bit_hand |= 1 << ((card3 % 13) + 2);
                        bit_hand |= 1 << ((card4 % 13) + 2);

                        if (straight_table[bit_hand])
                            ++straights;

                        if (quads_or_fullhouse_table[bit_hand])
                            ++quads_or_fullhouses;
                    }
                }
            }
        }
    }

    t2 = clock();
    std::cout << "Suoria     : " << straights << " kpl (oikea arvo 1228800)" << std::endl;
    std::cout << "Nelosia tai täyskäsiä: " << quads_or_fullhouses << " kpl (oikea arvo 524160)" << std::endl;
    // 449280 fullhouses, 74880 quads
    std::cout << "Aikaa kului: " << (t2 - t1) / (float)CLOCKS_PER_SEC << "s" << std::endl;

    return 0;
}

int is_straight(int* hand)
{
    std::sort(hand, hand + 5);

    if (hand[4] == 14 && hand[0] == 2 && hand[1] == 3 && hand[2] == 4 && hand[3] == 5)
        return 1;

    for (int i = 0; i < 4; ++i)
    {
        if (hand[i] + 1 != hand[i + 1])
            return 0;
    }

    return 1;
}

int is_quadsorfullhouse(int* hand)
{
    int counts[15] = { 0 };
    for (int i = 0; i < 5; ++i)
    {
        counts[hand[i]]++;
    }

    bool has_three = false;
    bool has_two = false;
    for (int i = 2; i <= 14; ++i)
    {
        if (counts[i] == 4)
        {
            return 1;
        }
        else if (counts[i] == 3)
        {
            has_three = true;
        }
        else if (counts[i] == 2)
        {
            has_two = true;
        }
    }
    if (has_three && has_two)
    {
        return 1;
    }

    return 0;
}
