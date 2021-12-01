#include "huffman.hpp"
#include "util.hpp"

namespace
{
const uint16_t SCALEFACTOR_TREE[][2] =
{
    {560,   1}, { 92,   2}, { 90,   3}, { 87,   4}, { 83,   5},
    { 79,   6}, { 74,   7}, { 72,   8}, { 67,   9}, { 62,  10},
    { 56,  11}, { 52,  12}, { 44,  13}, { 14,  22}, { 18,  15},
    { 16,  98}, { 17, 106}, {500, 105}, { 42,  19}, { 21,  20},
    {501, 502}, {519, 503}, { 29,  23}, { 24,  35}, { 27,  25},
    { 26,  33}, {619, 504}, { 28, 109}, {510, 505}, {110,  30},
    {113,  31}, { 32,  34}, {618, 506}, {507, 515}, {508, 509},
    { 39,  36}, { 37,  38}, {511, 512}, {514, 513}, { 40,  41},
    {516, 518}, {520, 517}, {523,  43}, {590, 521}, { 49,  45},
    { 48,  46}, {588,  47}, {525, 522}, {528, 524}, { 51,  50},
    {526, 527}, {586, 529}, { 55,  53}, { 97,  54}, {530, 531},
    {584, 532}, { 60,  57}, { 58,  59}, {585, 533}, {536, 534},
    {583,  61}, {537, 535}, { 66,  63}, { 65,  64}, {582, 538},
    {581, 539}, {580, 540}, { 70,  68}, { 71,  69}, {542, 541},
    {545, 543}, {544, 579}, { 73,  94}, {574, 546}, { 78,  75},
    { 77,  76}, {573, 547}, {572, 548}, {549, 571}, { 82,  80},
    {551,  81}, {570, 550}, {552, 569}, { 86,  84}, {567,  85},
    {553, 568}, {566, 554}, { 89,  88}, {555, 565}, {556, 564},
    {562,  91}, {557, 563}, {559,  93}, {561, 558}, { 95,  96},
    {576, 575}, {577, 578}, {587, 589}, { 99, 102}, {100, 101},
    {597, 591}, {592, 593}, {103, 104}, {594, 595}, {596, 604},
    {598, 599}, {107, 108}, {600, 601}, {602, 617}, {603, 620},
    {116, 111}, {119, 112}, {610, 605}, {114, 115}, {606, 607},
    {608, 609}, {117, 118}, {611, 612}, {613, 614}, {615, 616},
};

const uint16_t SPECTRUM1_TREE[][2] =
{
    {540,   1}, { 38,   2}, { 19,   3}, { 33,   4}, { 28,   5},
    { 11,   6}, { 16,   7}, { 25,   8}, {  9,  14}, { 10,  58},
    {500, 574}, { 12,  55}, { 13,  59}, {521, 501}, { 15,  76},
    {518, 502}, { 23,  17}, { 18,  46}, {511, 503}, { 52,  20},
    { 21,  44}, { 22,  73}, {504, 528}, { 24,  78}, {505, 509},
    { 50,  26}, { 32,  27}, {572, 506}, { 47,  29}, { 42,  30},
    { 31,  77}, {507, 529}, {580, 508}, { 34,  36}, { 74,  35},
    {510, 568}, { 37,  41}, {512, 566}, { 39,  60}, { 40,  71},
    {567, 513}, {514, 530}, { 72,  43}, {525, 515}, { 45,  62},
    {516, 544}, {517, 571}, { 48,  63}, { 49,  75}, {573, 519},
    { 51,  57}, {560, 520}, { 53,  65}, { 54,  70}, {558, 522},
    { 79,  56}, {523, 579}, {524, 556}, {562, 526}, {527, 553},
    { 69,  61}, {543, 531}, {570, 532}, { 67,  64}, {533, 555},
    { 66,  68}, {534, 542}, {547, 535}, {576, 536}, {541, 537},
    {538, 546}, {539, 549}, {565, 545}, {564, 548}, {552, 550},
    {561, 551}, {554, 578}, {559, 557}, {575, 563}, {569, 577},
};

const uint16_t SPECTRUM2_TREE[][2] =
{
    { 38,   1}, { 20,   2}, {  9,   3}, { 16,   4}, { 29,   5},
    {  6,  13}, {  7,  24}, {520,   8}, {556, 500}, { 46,  10},
    { 57,  11}, { 61,  12}, {515, 501}, { 27,  14}, { 15,  79},
    {554, 502}, { 35,  17}, { 18,  59}, { 26,  19}, {505, 503},
    { 21,  32}, { 22,  64}, { 37,  23}, {564, 504}, { 56,  25},
    {580, 506}, {507, 517}, { 45,  28}, {508, 572}, { 30,  66},
    { 55,  31}, {509, 551}, { 42,  33}, { 34,  76}, {530, 510},
    { 36,  49}, {511, 555}, {512, 558}, { 39,  51}, {540,  40},
    {567,  41}, {513, 541}, { 44,  43}, {528, 514}, {516, 550},
    {562, 518}, { 73,  47}, { 75,  48}, {519, 533}, {573,  50},
    {559, 521}, { 62,  52}, { 53,  70}, {549,  54}, {534, 522},
    {553, 523}, {524, 526}, { 78,  58}, {571, 525}, { 60,  74},
    {527, 569}, {529, 579}, { 69,  63}, {531, 543}, { 68,  65},
    {568, 532}, { 67,  77}, {557, 535}, {536, 570}, {537, 539},
    { 72,  71}, {548, 538}, {546, 542}, {544, 566}, {563, 545},
    {547, 565}, {576, 552}, {577, 560}, {561, 575}, {574, 578},
};

const uint16_t SPECTRUM3_TREE[][2] =
{
    {500,   1}, {  2,   4}, {  3,  10}, {527, 501}, { 11,   5},
    {  6,  13}, { 52,   7}, { 55,   8}, {537,   9}, {554, 502},
    {509, 503}, { 12,  27}, {536, 504}, { 14,  22}, { 15,  17},
    { 16,  20}, {505, 563}, { 18,  29}, { 32,  19}, {566, 506},
    {548,  21}, {507, 516}, { 34,  23}, { 24,  38}, { 25,  53},
    { 56,  26}, {508, 517}, { 28,  31}, {512, 510}, { 33,  30},
    {518, 511}, {530, 513}, {545, 514}, {521, 515}, { 44,  35},
    { 57,  36}, { 37,  63}, {534, 519}, { 48,  39}, { 46,  40},
    { 50,  41}, { 42,  65}, {553,  43}, {520, 560}, { 67,  45},
    {522, 542}, { 47,  59}, {524, 523}, { 49,  61}, {525,  70},
    { 51,  68}, {526, 579}, {528, 539}, { 69,  54}, {529, 555},
    {540, 531}, {564, 532}, {543,  58}, {546, 533}, { 60,  78},
    {535, 573}, { 62,  64}, {538, 558}, {567, 541}, {544, 576},
    { 66,  71}, {547,  75}, {557, 549}, {570, 550}, {575, 551},
    {572, 552}, { 77,  72}, { 74,  73}, {556,  76}, {571, 559},
    {561, 568}, {574, 562}, {565,  79}, {569, 578}, {580, 577},
};

const uint16_t SPECTRUM4_TREE[][2] =
{
    {  1,   4}, { 32,   2}, { 54,   3}, {536, 500}, {  5,   8},
    { 16,   6}, { 31,   7}, {501, 510}, { 14,   9}, { 17,  10},
    { 11,  24}, { 22,  12}, { 55,  13}, {502,  21}, { 15,  62},
    {503, 509}, {504, 530}, { 34,  18}, { 56,  19}, { 30,  20},
    {521, 505}, {518, 506}, { 37,  23}, {529, 507}, { 48,  25},
    { 38,  26}, { 45,  27}, { 28,  52}, { 29,  41}, {508, 560},
    {555, 511}, {528, 512}, { 33,  60}, {540, 513}, { 35,  42},
    {516,  36}, {514, 542}, {515, 519}, { 39,  58}, { 40,  44},
    {517, 573}, {520, 556}, { 43,  61}, {522, 532}, {523, 561},
    { 75,  46}, { 79,  47}, {524, 572}, { 66,  49}, { 72,  50},
    { 73,  51}, {569, 525}, { 53,  76}, {580, 526}, {531, 527},
    {533, 554}, { 57,  68}, {534, 563}, { 59,  69}, {535, 579},
    {537, 539}, {546, 538}, { 63,  70}, { 65,  64}, {549, 541},
    {567, 543}, { 74,  67}, {570, 544}, {557, 545}, {547, 559},
    { 78,  71}, {548, 558}, {550, 568}, {551, 575}, {552, 576},
    {565, 553}, {578,  77}, {574, 562}, {566, 564}, {571, 577},
};

const uint16_t SPECTRUM5_TREE[][2] =
{
    {540,   1}, { 68,   2}, { 66,   3}, { 53,   4}, { 38,   5},
    { 34,   6}, { 19,   7}, { 16,   8}, { 13,   9}, { 28,  10},
    { 31,  11}, { 30,  12}, {580, 500}, { 27,  14}, { 50,  15},
    {563, 501}, { 32,  17}, { 18,  77}, {502, 578}, { 23,  20},
    { 21,  25}, { 22,  63}, {503, 544}, { 24,  79}, {536, 504},
    { 73,  26}, {505, 545}, {570, 506}, { 29,  49}, {507, 571},
    {572, 508}, {573, 509}, { 33,  48}, {564, 510}, { 51,  35},
    { 36,  46}, { 37,  62}, {511, 565}, { 42,  39}, { 40,  44},
    { 41,  64}, {512, 552}, { 43,  74}, {513, 567}, { 45,  72},
    {514, 566}, { 78,  47}, {515, 519}, {516, 526}, {517, 579},
    {518, 574}, { 61,  52}, {520, 556}, { 57,  54}, { 55,  59},
    { 56,  65}, {521, 559}, { 58,  75}, {522, 542}, { 60,  71},
    {523, 557}, {524, 560}, {525, 555}, {575, 527}, {568, 528},
    {529, 551}, { 70,  67}, {530, 550}, { 69,  76}, {531, 549},
    {548, 532}, {533, 547}, {546, 534}, {553, 535}, {537, 543},
    {558, 538}, {541, 539}, {554, 562}, {569, 561}, {577, 576},
};

const uint16_t SPECTRUM6_TREE[][2] =
{
    { 60,   1}, { 47,   2}, { 32,   3}, { 28,   4}, { 15,   5},
    { 12,   6}, { 22,   7}, { 77,   8}, { 11,   9}, { 21,  10},
    {500, 572}, {507, 501}, { 13,  19}, { 14,  42}, {506, 502},
    { 25,  16}, { 17,  65}, { 56,  18}, {505, 503}, { 20,  55},
    {504, 578}, {580, 508}, { 74,  23}, {544,  24}, {509, 517},
    { 40,  26}, { 27,  41}, {564, 510}, { 36,  29}, { 30,  43},
    {569,  31}, {511, 525}, { 45,  33}, { 38,  34}, { 35,  57},
    {512, 552}, { 37,  67}, {567, 513}, { 39,  64}, {514, 568},
    {515, 570}, {516, 545}, {562, 518}, { 76,  44}, {555, 519},
    { 54,  46}, {520, 560}, { 48,  51}, {530,  49}, { 75,  50},
    {523, 521}, { 52,  58}, { 53,  72}, {522, 533}, {556, 524},
    {574, 526}, {527, 577}, {546, 528}, { 68,  59}, {529, 542},
    { 69,  61}, { 62,  63}, {531, 550}, {532, 548}, {566, 534},
    { 73,  66}, {535, 536}, {537, 543}, {551, 538}, { 71,  70},
    {539, 541}, {540, 549}, {558, 547}, {553, 575}, {576, 554},
    {557, 559}, {561, 565}, { 78,  79}, {563, 573}, {571, 579},
};

const uint16_t SPECTRUM7_TREE[][2] =
{
    {500,   1}, {  2,   3}, {508, 501}, {  4,   7}, {509,   5},
    { 28,   6}, {516, 502}, {  8,  11}, { 29,   9}, { 10,  31},
    {503,  38}, { 12,  18}, { 13,  15}, { 39,  14}, {504, 532},
    { 16,  34}, { 40,  17}, {542, 505}, { 19,  23}, { 41,  20},
    { 36,  21}, { 22,  47}, {530, 506}, { 43,  24}, { 25,  51},
    { 50,  26}, {531,  27}, {556, 507}, {517, 510}, { 30,  37},
    {525, 511}, { 32,  33}, {512, 533}, {513, 541}, { 54,  35},
    {514, 535}, {550, 515}, {518, 524}, {519, 526}, {527, 520},
    {534, 521}, { 46,  42}, {543, 522}, { 48,  44}, { 45,  59},
    {551, 523}, {529, 528}, {548, 536}, { 49,  55}, {557, 537},
    {545, 538}, { 52,  57}, { 56,  53}, {560, 539}, {549, 540},
    {558, 544}, {553, 546}, { 58,  60}, {547, 561}, {559, 552},
    { 61,  62}, {562, 554}, {555, 563},
};

const uint16_t SPECTRUM8_TREE[][2] =
{
    {  1,   6}, { 29,   2}, {  5,   3}, {518,   4}, {500, 516},
    {510, 501}, {  7,  10}, {  8,  32}, {  9,  31}, {502, 525},
    { 11,  17}, { 12,  14}, { 13,  50}, {503, 535}, { 35,  15},
    { 51,  16}, {544, 504}, { 37,  18}, { 19,  22}, { 20,  47},
    { 21,  56}, {552, 505}, { 40,  23}, { 24,  26}, { 57,  25},
    {506, 561}, { 62,  27}, { 59,  28}, {507, 563}, {509,  30},
    {517, 508}, {511, 526}, { 42,  33}, { 34,  43}, {512, 534},
    { 44,  36}, {513, 543}, { 45,  38}, { 39,  52}, {549, 514},
    { 41,  54}, {515, 546}, {519,  49}, {520, 524}, {541, 521},
    { 55,  46}, {522, 550}, { 48,  60}, {558, 523}, {527, 533},
    {528, 542}, {529, 536}, { 53,  58}, {530, 551}, {531,  61},
    {537, 532}, {538, 557}, {548, 539}, {545, 540}, {547, 556},
    {553, 559}, {554, 560}, {562, 555},
};

const uint16_t SPECTRUM9_TREE[][2] =
{
    {500,   1}, {  2,   3}, {513, 501}, {  4,   7}, {514,   5},
    { 53,   6}, {526, 502}, {  8,  16}, {  9,  12}, { 74,  10},
    {516,  11}, {539, 503}, { 54,  13}, { 56,  14}, { 90,  15},
    {504, 552}, { 17,  26}, { 18,  22}, { 57,  19}, {112,  20},
    {555,  21}, {505, 532}, { 59,  23}, { 24,  77}, { 25,  79},
    {592, 506}, { 27,  34}, { 28,  65}, { 63,  29}, { 30,  32},
    { 31,  91}, {507, 619}, { 33, 124}, {546, 508}, { 35,  44},
    { 36,  40}, { 83,  37}, { 38,  69}, { 39, 125}, {509, 645},
    { 71,  41}, { 94,  42}, { 43, 138}, {647, 510}, { 86,  45},
    { 46, 129}, { 47,  50}, {110,  48}, { 49, 128}, {511, 662},
    {121,  51}, { 52, 145}, {512, 650}, {527, 515}, { 75,  55},
    {517, 553}, {530, 518}, { 76,  58}, {519, 543}, { 60, 102},
    { 61,  62}, {565, 520}, {544, 521}, { 64, 114}, {522, 593},
    { 80,  66}, { 67,  92}, { 68, 116}, {523, 595}, {106,  70},
    {610, 524}, {117,  72}, { 73, 163}, {537, 525}, {540, 528},
    {529, 541}, {566, 531}, { 78, 104}, {545, 533}, {606, 534},
    { 81, 136}, { 82, 135}, {535, 570}, {119,  84}, {609,  85},
    {633, 536}, { 87,  96}, {107,  88}, { 89, 109}, {538, 623},
    {554, 542}, {547, 558}, {105,  93}, {548, 644}, { 95, 126},
    {549, 574}, { 97,  99}, {127,  98}, {550, 575}, {100, 139},
    {141, 101}, {661, 551}, {103, 113}, {605, 556}, {557, 618},
    {560, 559}, {657, 561}, {155, 108}, {562, 586}, {624, 563},
    {111, 120}, {636, 564}, {567, 579}, {568, 580}, {115, 123},
    {578, 569}, {583, 571}, {152, 118}, {572, 596}, {573, 617},
    {600, 576}, {122, 142}, {637, 577}, {581, 607}, {631, 582},
    {621, 584}, {585, 611}, {643, 587}, {588, 656}, {130, 146},
    {131, 133}, {143, 132}, {663, 589}, {150, 134}, {603, 590},
    {604, 591}, {137, 144}, {594, 632}, {597, 659}, {140, 164},
    {648, 598}, {612, 599}, {601, 625}, {638, 602}, {620, 608},
    {613, 626}, {147, 156}, {148, 153}, {149, 151}, {614, 639},
    {615, 651}, {616, 627}, {622, 634}, {154, 159}, {628, 629},
    {630, 635}, {157, 160}, {158, 165}, {640, 652}, {641, 665},
    {167, 161}, {162, 166}, {642, 654}, {658, 646}, {660, 649},
    {664, 653}, {655, 668}, {666, 667},
};

const uint16_t SPECTRUM10_TREE[][2] =
{
    {  6,   1}, {  2,  19}, {  3,  15}, {  4,  13}, { 59,   5},
    {500, 555}, { 55,   7}, {  8,  10}, {  9, 103}, {501, 516},
    { 86,  11}, { 12,  87}, {526, 502}, {102,  14}, {503, 556},
    { 60,  16}, { 89,  17}, {111,  18}, {579, 504}, { 20,  25},
    { 63,  21}, { 22,  68}, { 23,  66}, {121,  24}, {592, 505},
    { 26,  31}, { 27,  71}, {124,  28}, {135,  29}, {105,  30},
    {535, 506}, { 32,  39}, { 75,  33}, { 34,  79}, { 35,  37},
    { 36, 107}, {507, 612}, {129,  38}, {648, 508}, { 40,  44},
    {108,  41}, {143,  42}, { 43, 120}, {657, 509}, { 82,  45},
    { 46,  51}, { 47,  49}, { 48, 162}, {510, 603}, {166,  50},
    {511, 654}, {154,  52}, {159,  53}, { 54, 167}, {667, 512},
    { 58,  56}, {527,  57}, {528, 513}, {514, 515}, {517, 553},
    { 88,  61}, { 62, 104}, {518, 566}, { 64,  91}, { 65, 122},
    {519, 545}, { 67, 147}, {605, 520}, { 94,  69}, { 70, 114},
    {521, 547}, { 72,  96}, {115,  73}, { 74, 152}, {522, 598},
    {117,  76}, {149,  77}, { 78, 156}, {636, 523}, { 99,  80},
    {119,  81}, {524, 658}, { 83, 132}, {101,  84}, {614,  85},
    {627, 525}, {529, 542}, {530, 554}, {531, 567}, {112,  90},
    {580, 532}, {113,  92}, { 93, 123}, {546, 533}, { 95, 138},
    {559, 534}, {127,  97}, { 98, 106}, {536, 631}, {100, 142},
    {647, 537}, {662, 538}, {543, 539}, {541, 540}, {568, 544},
    {548, 618}, {549, 623}, {635, 550}, {130, 109}, {164, 110},
    {661, 551}, {581, 552}, {569, 557}, {558, 137}, {608, 560},
    {151, 116}, {561, 632}, {141, 118}, {562, 591}, {637, 563},
    {564, 626}, {594, 565}, {570, 582}, {571, 606}, {125, 126},
    {572, 609}, {573, 139}, {148, 128}, {574, 634}, {575, 613},
    {157, 131}, {649, 576}, {133, 145}, {165, 134}, {602, 577},
    {136, 140}, {619, 578}, {583, 593}, {584, 596}, {597, 585},
    {586, 620}, {587, 604}, {601, 588}, {144, 161}, {589, 617},
    {146, 153}, {590, 639}, {607, 595}, {599, 633}, {645, 150},
    {600, 646}, {610, 621}, {611, 622}, {615, 664}, {158, 155},
    {656, 616}, {644, 624}, {625, 659}, {628, 641}, {163, 160},
    {629, 655}, {638, 630}, {643, 640}, {665, 642}, {660, 650},
    {651, 663}, {652, 653}, {666, 668},
};

const uint16_t SPECTRUM11_TREE[][2] =
{
    {  1,  13}, {  2,   6}, {  3,   4}, {500, 518}, { 71,   5},
    {501, 535}, { 72,   7}, {  8,  10}, {  9,  76}, {537, 502},
    {116,  11}, { 12, 187}, {551, 503}, { 14,  20}, { 15,  84},
    { 78,  16}, { 81,  17}, {169,  18}, { 19, 188}, {504, 550},
    { 21,  31}, { 22,  26}, { 23,  89}, {175,  24}, {202,  25},
    {505, 228}, { 27,  94}, { 28, 132}, {191,  29}, { 93,  30},
    {673, 506}, { 32,  38}, { 33, 102}, { 67,  34}, { 35,  98},
    { 36, 141}, { 37, 264}, {507, 564}, { 39,  48}, { 40,  44},
    { 41, 107}, { 42, 218}, {101,  43}, {508, 728}, {164,  45},
    {236,  46}, {221,  47}, {653, 509}, {110,  49}, { 50,  54},
    { 51,  62}, {253,  52}, {252,  53}, {510, 716}, { 55,  58},
    { 56, 279}, {269,  57}, {721, 511}, {271,  59}, { 60,  65},
    {753,  61}, {514, 512}, {260,  63}, { 64, 285}, {513, 750},
    {284,  66}, {515, 770}, {135,  68}, {245,  69}, {217,  70},
    {516, 581}, {788, 517}, { 73,  74}, {519, 536}, { 75, 115},
    {520, 552}, {554,  77}, {569, 521}, { 79, 119}, {118,  80},
    {522, 588}, { 82, 167}, {150,  83}, {523, 584}, { 85, 125},
    {122,  86}, {212,  87}, {223,  88}, {533, 524}, { 90, 129},
    { 91, 155}, { 92, 226}, {720, 525}, {627, 526}, { 95, 158},
    {230,  96}, { 97, 214}, {690, 527}, {204,  99}, {274, 100},
    {528, 580}, {529, 729}, {103, 137}, {104, 183}, {105, 162},
    {161, 106}, {530, 706}, {146, 108}, {109, 186}, {759, 531},
    {209, 111}, {112, 238}, {222, 113}, {267, 114}, {532, 682},
    {553, 534}, {117, 148}, {570, 538}, {539, 572}, {149, 120},
    {171, 121}, {540, 603}, {123, 151}, {286, 124}, {780, 541},
    {172, 126}, {127, 153}, {128, 262}, {542, 592}, {130, 177},
    {131, 229}, {626, 543}, {179, 133}, {134, 157}, {544, 659},
    {136, 194}, {724, 545}, {138, 143}, {139, 206}, {140, 247},
    {695, 546}, {273, 142}, {547, 726}, {196, 144}, {266, 145},
    {548, 614}, {208, 147}, {549, 760}, {571, 555}, {556, 589},
    {605, 557}, {152, 189}, {621, 558}, {227, 154}, {559, 585},
    {190, 156}, {560, 641}, {561, 611}, {159, 181}, {160, 193},
    {628, 562}, {688, 563}, {163, 195}, {630, 565}, {233, 165},
    {166, 256}, {600, 566}, {168, 287}, {567, 777}, {199, 170},
    {568, 574}, {573, 604}, {173, 200}, {174, 216}, {575, 783},
    {270, 176}, {624, 576}, {243, 178}, {577, 610}, {242, 180},
    {658, 578}, {182, 203}, {579, 689}, {184, 224}, {185, 265},
    {742, 582}, {664, 583}, {586, 587}, {590, 601}, {607, 591},
    {593, 737}, {198, 192}, {594, 643}, {595, 671}, {772, 596},
    {597, 598}, {248, 197}, {679, 599}, {602, 657}, {606, 778},
    {241, 201}, {608, 669}, {609, 625}, {723, 612}, {205, 255},
    {646, 613}, {207, 232}, {615, 680}, {705, 616}, {249, 210},
    {257, 211}, {714, 617}, {213, 215}, {618, 782}, {619, 676},
    {622, 620}, {623, 784}, {629, 645}, {219, 220}, {631, 711},
    {632, 758}, {756, 633}, {634, 251}, {781, 635}, {275, 225},
    {710, 636}, {637, 754}, {639, 638}, {640, 787}, {642, 655},
    {231, 244}, {674, 644}, {757, 647}, {235, 234}, {730, 648},
    {745, 649}, {276, 237}, {762, 650}, {258, 239}, {240, 281},
    {699, 651}, {652, 773}, {672, 654}, {656, 771}, {675, 660},
    {692, 246}, {691, 661}, {662, 743}, {663, 744}, {250, 277},
    {666, 665}, {667, 763}, {715, 668}, {278, 254}, {748, 670},
    {678, 677}, {681, 697}, {683, 747}, {259, 268}, {684, 732},
    {263, 261}, {685, 734}, {686, 785}, {687, 718}, {693, 722},
    {694, 741}, {696, 739}, {698, 701}, {731, 700}, {702, 751},
    {786, 703}, {283, 272}, {736, 704}, {725, 707}, {740, 708},
    {709, 727}, {712, 761}, {713, 746}, {717, 764}, {282, 280},
    {768, 719}, {749, 733}, {735, 767}, {738, 752}, {769, 755},
    {765, 766}, {779, 774}, {775, 776},
};

const uint16_t (*const SPECTRUM_TREES[])[2] =
{
    SPECTRUM1_TREE,
    SPECTRUM2_TREE,
    SPECTRUM3_TREE,
    SPECTRUM4_TREE,
    SPECTRUM5_TREE,
    SPECTRUM6_TREE,
    SPECTRUM7_TREE,
    SPECTRUM8_TREE,
    SPECTRUM9_TREE,
    SPECTRUM10_TREE,
    SPECTRUM11_TREE,
};

inline uint16_t DecodeBits(const uint16_t (*const tree)[2], const uint8_t *data, size_t &pos)
{
    // <500: node, else: leaf
    uint16_t index = 0;
    do {
        index = tree[index][extract_bit(data, pos++)];
    }
    while (index < 500);
    return index - 500;
}
}

namespace Huffman
{
int DecodeScalefactorBits(const uint8_t *data, size_t &pos)
{
    // ISO/IEC 13818-7 Scalefactor Huffman codebook parameters
    return DecodeBits(SCALEFACTOR_TREE, data, pos) - 60;
}

void DecodeSpectrumQuadBits(int codebook, const uint8_t *data, size_t &pos, int &unsigned_, int &w, int &x, int &y, int &z)
{
    // ISO/IEC 13818-7 Spectrum Huffman codebooks parameters
    uint16_t index = DecodeBits(SPECTRUM_TREES[codebook], data, pos);
    int un = codebook >= 2;
    unsigned_ = un;
    w = index / 27 + un - 1;
    x = index / 9 % 3 + un - 1;
    y = index / 3 % 3 + un - 1;
    z = index % 3 + un - 1;
}

void DecodeSpectrumPairBits(int codebook, const uint8_t *data, size_t &pos, int &unsigned_, int &y, int &z)
{
    // ISO/IEC 13818-7 Spectrum Huffman codebooks parameters
    uint16_t index = DecodeBits(SPECTRUM_TREES[codebook], data, pos);
    int mod = codebook >= 10 ? 17 : codebook >= 8 ? 13 : codebook >= 6 ? 8 : 9;
    int un = codebook >= 6;
    unsigned_ = un;
    y = index / mod + un * 4 - 4;
    z = index % mod + un * 4 - 4;
}
}