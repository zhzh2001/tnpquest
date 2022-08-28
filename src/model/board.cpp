#include "board.h"
#include "../controller/game.h"
#include "../utils/msgbox.h"
#ifdef _WIN32
#include "../pdcurses/curses.h"
#else
#include "../ncurses/ncurses.h"
#endif
#include <algorithm>
#include <typeinfo>

std::vector<Terrain> Board::terrains = {
	Terrain(MultiString({"Corndog", "玉米狗"}), true),
	Terrain(MultiString({"Sand", "沙地"}), true),
	Terrain(MultiString({"Dirt", "泥地"}), true),
	Terrain(MultiString({"Grass", "草地"}), true),
	Terrain(MultiString({"Fields", "田野"}), true),
	Terrain(MultiString({"Rocks", "岩石"}), true),
	Terrain(MultiString({"Cliffs", "悬崖"}), true),
	Terrain(MultiString({"Fence", "栅栏"}), true),
	Terrain(MultiString({"Tree", "树"}), true),
	Terrain(MultiString({"Den", "兽穴"}), true),
	Terrain(MultiString({"Evergrn", "常青树"}), true),
	Terrain(MultiString({"Thorns", "荆棘"}), true),
	Terrain(MultiString({"Marsh", "沼泽"}), true),
	Terrain(MultiString({"Bushes", "灌木"}), true),
	Terrain(MultiString({"Garbage", "垃圾"}), true),
	Terrain(MultiString({"River", "河流"}), true),
	Terrain(MultiString({"Pond", "池塘"}), true),
	Terrain(MultiString({"Stream", "溪流"}), true),
	Terrain(MultiString({"SwpWater", "沼泽水"}), true),
	Terrain(MultiString({"Farms", "农场"}), true),
	Terrain(MultiString({"Yard", "院子"}), true),
	Terrain(MultiString({"BrushPil", "草堆"}), true),
	Terrain(MultiString({"FallTree", "倒下的树"}), true),
	Terrain(MultiString({"Flowers", "花"}), true),
	Terrain(MultiString({"TwolgDen", "两脚兽穴"}), true),
	Terrain(MultiString({"Plant", "植物"}), true),
	Terrain(MultiString({"TwlgPath", "两脚兽路"}), true),
	Terrain(MultiString({"RockPile", "岩石堆"}), true),
	Terrain(MultiString({"Puddle", "水洼"}), true),
	Terrain(MultiString({"Thudrpat", "雷鬼路"}), true),
	Terrain(MultiString({"Ferns", "蕨类"}), true),
	Terrain(MultiString({"TallGras", "高草"}), true),
	Terrain(MultiString({"Garden", "花园"}), true),
	Terrain(MultiString({"Cliffs", "悬崖"}), true)};

const std::vector<Prey> Board::preys = {
	Prey(MultiString({"Pigeon", "林鸽"}), {1, 1, 2}, {7, 8, 10, 31}, true, true, 5, 4),
	Prey(MultiString({"Chafinch", "苍头燕雀"}), {2, 2, 3}, {4, 7, 8, 10, 19, 24, 27, 31}, true, true, 5, 3),
	Prey(MultiString({"Starling", "椋鸟"}), {2, 2, 3, 3, 4}, {4, 7, 8, 10, 12, 14, 19, 20, 21, 22, 24, 27, 31, 32}, true, true, 5, 3),
	Prey(MultiString({"Vole", "田鼠"}), {2, 2, 3, 3}, {1, 2, 4, 5, 7, 11, 12, 13, 21, 22, 25, 27, 30, 31}, false, true, 3, 2),
	Prey(MultiString({"Vole", "田鼠"}), {2, 2, 3}, {1, 2, 4, 5, 7, 11, 12, 13, 21, 22, 25, 27, 30, 31}, false, true, 3, 2),
	Prey(MultiString({"Fish", "鱼"}), {1, 2}, {15, 16, 17}, true, true, 7, 4),
	Prey(MultiString({"Fish", "鱼"}), {2, 3}, {15, 16, 17}, true, true, 7, 5),
	Prey(MultiString({"Rabbit", "兔"}), {1, 2, 2, 3}, {4, 9, 12, 13, 21, 22, 27, 31, 32}, true, true, 8, 6),
	Prey(MultiString({"Rabbit", "兔"}), {1, 2, 3, 3, 3}, {4, 9, 12, 13, 21, 22, 27, 31, 32}, true, true, 8, 6),
	Prey(MultiString({"Squirrel", "松鼠"}), {1, 2, 3, 3}, {4, 7, 8, 10, 21, 22}, true, true, 7, 5),
	Prey(MultiString({"Squirrel", "松鼠"}), {1, 2, 3, 3}, {4, 7, 8, 10, 21, 22}, true, true, 7, 5),
	Prey(MultiString({"Mouse", "老鼠"}), {2, 3}, {4, 5, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, 25, 27, 30, 31, 32}, true, true, 2, 1),
	Prey(MultiString({"Mouse", "老鼠"}), {1, 2}, {4, 5, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, 25, 27, 30, 31, 32}, false, true, 3, 2),
	Prey(MultiString({"Shrew", "鼩鼱"}), {2, 3}, {2, 4, 5, 11, 12, 13, 21, 22, 23, 25, 27, 30, 31}, true, true, 2, 2),
	Prey(MultiString({"Shrew", "鼩鼱"}), {2, 2, 3}, {2, 4, 5, 11, 12, 13, 21, 22, 23, 25, 27, 30, 31}, false, true, 3, 2),
	Prey(MultiString({"Frog", "青蛙"}), {2, 2, 3}, {12, 16, 17, 18, 28, 30}, true, true, 5, 3)};

const std::vector<Injury> Board::injuries = {
	Injury(MultiString({"Poison", "毒药"}), {0, 1, 1, 2}, {1, 2, 4, 8, 10, 12, 13, 14, 19, 20, 24, 26}, false, false, 4),
	Injury(MultiString({"Poison Water", "毒水"}), {1, 1, 2}, {15, 16, 17, 18, 24, 28}, false, false, 3),
	Injury(MultiString({"Barbed Wire", "铁丝网"}), {1, 1, 2, 2, 3}, {4, 7, 19, 20, 24, 32}, false, false, 2),
	Injury(MultiString({"Paint", "油漆"}), {0, 1}, {14, 19, 20, 24, 32}, false, false, 4),
	Injury(MultiString({"Fall", "坠落"}), {1, 2, 2}, {5, 7, 8, 9, 10, 21, 22, 24, 27}, false, false, 4),
	Injury(MultiString({"Cave in", "塌陷"}), {1, 2}, {2, 4, 12, 19, 27, 31}, false, false, 4),
	Injury(MultiString({"Deathberries", "死亡浆果"}), {1, 2}, {11, 13, 23, 25}, false, false, 5),
	Injury(MultiString({"Rock Fall", "落石"}), {0, 1}, {27}, false, false, 4),
	Injury(MultiString({"Ticks", "壁虱"}), {1, 2, 2, 2}, {4, 8, 10, 11, 12, 13, 21, 22, 23, 25, 30, 31}, false, false, 1),
	Injury(MultiString({"Rusty Nail", "锈铁钉"}), {0, 1, 1, 2}, {14, 19, 20, 24}, false, false, 2),
	Injury(MultiString({"Broken Glass", "碎玻璃"}), {0, 1, 1, 2}, {14, 19, 20, 24, 26, 29, 32}, false, false, 2),
	Injury(MultiString({"Wasps", "黄蜂"}), {1, 1, 2}, {1, 4, 5, 8, 21, 22, 24, 25, 27}, false, false, 4),
	Injury(MultiString({"Falling Branch", "落枝"}), {0, 1}, {8, 10, 21}, false, false, 4)};

const std::vector<Benefit> Board::benefits = {
	Benefit(MultiString({"Puddle", "水洼"}), {2, 2, 3, 3, 3}, {1, 2, 4, 12}, true, true, 1, 2, 3, CatVal::thirst),
	Benefit(MultiString({"Water", "缝隙水"}), {1, 2, 2, 3, 3}, {5, 27}, true, true, 2, 3, 2, CatVal::thirst),
	Benefit(MultiString({"Dew", "叶上露珠"}), {1, 2, 3, 3, 3}, {23, 25, 30}, true, true, 3, 4, 2, CatVal::thirst),
	Benefit(MultiString({"Mouse", "被困老鼠"}), {1, 2, 3, 3, 3}, {1, 2, 4, 5, 8, 10, 11, 12, 13, 21, 22, 23, 25}, true, true, 1, 5, 2, CatVal::hunger),
	Benefit(MultiString({"BirdNest", "鸟巢"}), {1, 2, 2, 3, 3}, {8, 10, 11, 13, 21}, true, true, 2, 6, 2, CatVal::hunger),
	Benefit(MultiString({"Hot Dog", "热狗"}), {1, 1, 1, 1, 2}, {19, 20, 26, 32}, true, true, 1, 2, 3, CatVal::hunger),
	Benefit(MultiString({"Cobweb", "蜘蛛网"}), {1, 2, 3}, {7, 8, 10, 19, 20, 21, 22, 23, 24, 25, 32}, true, true, 2, 3, 2, CatVal::health),
	Benefit(MultiString({"Marigold", "金盏花"}), {1, 1, 2, 2, 3}, {4, 31, 32}, true, true, 1, 2, 2, CatVal::health),
	Benefit(MultiString({"Juniper", "杜松莓"}), {1, 2, 2, 2, 3}, {10}, true, true, 4, 5, 3, CatVal::health),
	Benefit(MultiString({"Yarrow", "蓍草"}), {1, 2, 3}, {1, 2, 4}, true, true, 3, 4, 3, CatVal::health),
	Benefit(MultiString({"Tansy", "艾菊"}), {1, 3}, {4, 7, 24, 26}, true, true, 4, 5, 2, CatVal::health),
	Benefit(MultiString({"Watrmint", "水薄荷"}), {1, 3}, {12, 30}, true, true, 1, 2, 2, CatVal::health),
	Benefit(MultiString({"Horstail", "马尾草"}), {2, 3}, {1, 2, 4, 12, 31}, true, true, 2, 3, 2, CatVal::health),
	Benefit(MultiString({"Borage", "琉璃苣叶"}), {2, 3}, {2, 4, 32}, true, true, 3, 4, 2, CatVal::health),
	Benefit(MultiString({"Coltsfot", "款冬"}), {1, 3}, {12, 30, 31}, true, true, 2, 3, 2, CatVal::health)};

const std::vector<DefenseAction> Board::defenseActions = {
	DefenseAction(MultiString({"Monster", "怪兽"}), {3, 3, 4}, {29}, true, true, {9, 16}, CatVal::skill),
	DefenseAction(MultiString({"Twoleg Trap", "两脚兽陷阱"}), {0, 1, 1}, {7, 19, 20, 24, 26, 32}, false, true, {3, 5}, CatVal::skill),
	DefenseAction(MultiString({"Twoleg Trap", "两脚兽陷阱"}), {0, 1, 1}, {7, 19, 20, 24, 26, 32}, false, false, {2, 4}, CatVal::skill),
	DefenseAction(MultiString({"Fire", "火"}), {1, 1, 2}, {4, 8, 10, 11, 13, 14, 19, 21, 24, 31}, false, false, {7, 12}, CatVal::skill),
	DefenseAction(MultiString({"Flood", "洪水"}), {1}, {15, 16, 17, 18}, false, false, {6, 10}, CatVal::skill),
	DefenseAction(MultiString({"Thorn Tangle", "缠绕的荆棘"}), {1, 1, 2}, {4, 11, 31}, false, false, {1, 2}, CatVal::skill),
	DefenseAction(MultiString({"Thorn Tangle", "缠绕的荆棘"}), {1, 2, 2}, {4, 11, 31}, false, false, {2, 3}, CatVal::skill),
	DefenseAction(MultiString({"Thorn Tangle", "缠绕的荆棘"}), {1}, {4, 11, 31}, false, false, {3, 4}, CatVal::skill),
	DefenseAction(MultiString({"Lightning", "闪电"}), {0, 0, 1, 2}, {1, 2, 4, 5, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32}, false, false, {7, 12}, CatVal::speed),
	DefenseAction(MultiString({"Horse", "马"}), {1, 1, 2, 2, 3}, {4, 19}, true, true, {3, 5}, CatVal::speed),
	DefenseAction(MultiString({"Horse", "马"}), {1, 1, 1, 2}, {4, 19}, false, true, {2, 4}, CatVal::speed),
	DefenseAction(MultiString({"Sheep", "绵羊"}), {1, 1, 2}, {4, 19}, false, true, {3, 5}, CatVal::speed),
	DefenseAction(MultiString({"Sheep", "绵羊"}), {1, 2, 2}, {4, 19}, false, true, {2, 4}, CatVal::speed),
	DefenseAction(MultiString({"Cow", "奶牛"}), {1, 2}, {4, 19}, false, true, {3, 5}, CatVal::speed),
	DefenseAction(MultiString({"Cow", "奶牛"}), {1, 2}, {4, 19}, false, true, {2, 4}, CatVal::speed),
	DefenseAction(MultiString({"Hawk", "鹰"}), {1, 1, 2}, {1, 2, 4, 5, 7, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32}, false, false, {7, 10}, CatVal::skill),
	DefenseAction(MultiString({"Hawk", "鹰"}), {0, 1, 1}, {33}, false, false, {5, 10}, CatVal::skill),
	DefenseAction(MultiString({"Owl", "猫头鹰"}), {0, 1, 1}, {4, 8, 10, 11, 12, 13, 18, 19, 23, 24, 25, 31}, false, false, {7, 10}, CatVal::skill),
	DefenseAction(MultiString({"Owl", "猫头鹰"}), {1, 1, 2}, {4, 8, 10, 11, 12, 13, 18, 19, 23, 24, 25, 31}, false, false, {5, 10}, CatVal::skill),
	DefenseAction(MultiString({"Monster", "怪兽"}), {1, 1, 2}, {29}, false, false, {9, 16}, CatVal::skill)};

std::vector<Enemy> Board::enemies = {
	Enemy(MultiString({"Twolegs", "两脚兽"}), {0, 1, 1}, {19, 20, 26, 32}, true, true, 7, 3, {6, 8}),
	Enemy(MultiString({"Twolegs", "两脚兽"}), {0, 0, 1, 1}, {19, 20, 26, 32}, true, true, 8, 7, {6, 8}),
	Enemy(MultiString({"Rat Pack", "鼠群"}), {1, 1, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}, true, true, 6, 6, {6, 10}),
	Enemy(MultiString({"Rat", "老鼠"}), {1, 2, 3, 3}, {14, 18, 21, 24}, false, true, 2, 5, {3, 5}),
	Enemy(MultiString({"Dog", "狗"}), {0, 1, 1}, {15, 19, 20, 26, 32}, true, true, 10, 8, {8, 10}),
	Enemy(MultiString({"Dog", "狗"}), {0, 1, 1}, {16, 19, 20, 26, 32}, true, true, 6, 6, {6, 8}),
	Enemy(MultiString({"Dog", "狗"}), {0, 1, 1}, {15, 19, 20, 26, 32}, true, true, 5, 4, {4, 6}),
	Enemy(MultiString({"Badger", "獾"}), {1, 1, 2, 2}, {8, 9, 13, 21, 22, 27}, true, true, 12, 12, {12, 16}),
	Enemy(MultiString({"Badger", "獾"}), {1}, {9}, false, true, 12, 10, {7, 10}),
	Enemy(MultiString({"Kittypet", "宠物猫"}), {1, 2}, {19, 20, 32}, true, true, 4, 3, {5, 7}),
	Enemy(MultiString({"Kittypet", "宠物猫"}), {1, 2}, {19, 20, 32}, false, true, 6, 3, {5, 7}),
	Enemy(MultiString({"Rogue", "泼皮猫"}), {1, 2, 2}, {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 16, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, true, true, 6, 5, {6, 8}),
	Enemy(MultiString({"Rogue", "泼皮猫"}), {1, 2, 2}, {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 18, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, true, true, 7, 6, {6, 8}),
	Enemy(MultiString({"Rogue", "泼皮猫"}), {0, 2, 2}, {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, false, true, 8, 8, {6, 8}),
	Enemy(MultiString({"Loner", "独行猫"}), {1, 2, 2}, {4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, true, true, 6, 3, {6, 8}),
	Enemy(MultiString({"Loner", "独行猫"}), {0, 1, 2, 2}, {4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 17, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, true, true, 6, 5, {6, 8}),
	Enemy(MultiString({"Loner", "独行猫"}), {0, 1, 2}, {4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 19, 20, 21, 22, 23, 24, 25, 26, 30, 31, 32}, false, true, 7, 5, {6, 8}),
	Enemy(MultiString({"Fox", "狐狸"}), {1, 1, 2}, {4, 12, 21, 31}, true, true, 9, 7, {5, 7}),
	Enemy(MultiString({"Snake", "蛇"}), {1, 2, 2, 2, 3}, {1, 2, 4, 5, 11, 12, 13, 14, 19, 21, 22, 24, 27, 31, 33}, true, true, 2, 2, {3, 4}),
	Enemy(MultiString({"Poison Snake", "毒蛇"}), {0, 1, 1, 1, 2}, {5, 21, 27, 33}, false, true, 6, 2, {3, 4}),
	Enemy(MultiString({"Stoat", "白鼬"}), {1, 2, 2}, {4, 9, 12, 13, 21, 22, 24, 27, 31, 33}, true, true, 5, 4, {5, 7}),
	Enemy(MultiString({"Racoon", "浣熊"}), {0}, {8, 9, 10, 12, 14, 21, 22, 24}, true, true, 8, 7, {8, 12}),
	Enemy(MultiString({"Cougar", "美洲狮"}), {0, 0, 0, 1, 1}, {1, 2, 4, 12, 31}, true, true, 16, 11, {10, 13})};

const std::vector<Star> Board::stars = {
	Star(MultiString({"Food Supply", "食物补给"}), {15, 16, 17}),
	Star(MultiString({"Water Supply", "水补给"}), {4, 8, 10, 11, 13, 21, 22, 23, 31}),
	Star(MultiString({"Medicine Supply", "药物补给"}), {11, 23, 25, 30, 32}),
	Star(MultiString({"Dry Dens", "干燥巢穴"}), {11, 13, 12, 27, 31})};

const std::vector<std::vector<int>> Board::boardTerrains = {
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 25, 3, 3, 3, 3, 25, 25, 25, 25, 3, 3, 3, 3, 25, 25, 25, 3, 3, 25, 5, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 25, 3, 3, 3, 3, 25, 25, 25, 25, 3, 3, 3, 3, 25, 25, 25, 3, 3, 25, 5, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 3, 3, 3, 3, 3, 3, 3, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 3, 3, 3, 3, 3, 3, 3, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 13, 13, 3, 31, 31, 31, 31, 3, 13, 13, 3, 31, 31, 31, 31, 3, 3, 3, 3, 7, 3, 3, 3, 7, 7, 7, 3, 7, 7, 7, 7, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 3, 3, 3, 7, 4, 4, 4, 3, 3, 3, 3, 7, 4, 4, 4, 3, 3, 3, 4, 7, 4, 4, 4, 3, 11, 3, 3, 7, 4, 4, 4, 3, 3, 3, 3, 7, 4, 4, 4}, {2, 11, 11, 11, 2, 2, 2, 2, 2, 11, 11, 11, 11, 11, 11, 11, 2, 2, 11, 11, 11, 11, 11, 11, 11, 2, 2, 11, 11, 11, 11, 11, 3, 3, 11, 2, 2, 2, 2, 2}, {3, 3, 3, 3, 3, 3, 3, 3, 24, 14, 14, 24, 24, 24, 3, 3, 24, 14, 14, 24, 24, 24, 3, 3, 24, 14, 14, 24, 24, 24, 3, 13, 3, 3, 3, 3, 3, 3, 3, 3}, {3, 5, 3, 3, 3, 3, 3, 3, 3, 3, 25, 25, 3, 25, 25, 3, 25, 25, 25, 25, 3, 25, 25, 5, 25, 25, 3, 3, 23, 3, 3, 28, 3, 25, 3, 23, 23, 23, 3, 3}, {13, 13, 3, 3, 3, 3, 3, 3, 13, 13, 3, 3, 11, 5, 3, 3, 13, 13, 13, 3, 3, 3, 13, 13, 13, 13, 13, 3, 3, 3, 13, 13, 13, 13, 3, 23, 23, 23, 3, 13}, {3, 3, 3, 3, 3, 25, 3, 3, 3, 23, 23, 3, 3, 25, 25, 25, 3, 23, 23, 3, 3, 3, 3, 3, 3, 3, 3, 5, 25, 3, 10, 10, 3, 3, 25, 25, 3, 3, 10, 10}, {3, 3, 28, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 25, 25, 25, 14, 14, 14, 14, 25, 25, 25, 25, 14, 14, 14, 14, 25, 25, 25, 25, 3, 3, 3, 25, 25, 25, 25, 25}, {31, 31, 31, 11, 3, 3, 3, 3, 11, 31, 31, 11, 33, 33, 33, 33, 31, 31, 11, 11, 33, 33, 33, 33, 31, 31, 31, 3, 33, 33, 33, 33, 31, 5, 3, 3, 3, 3, 33, 33}, {3, 3, 3, 3, 3, 3, 3, 3, 23, 3, 3, 3, 3, 28, 3, 25, 7, 7, 7, 7, 7, 7, 25, 25, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 25, 4, 4}, {2, 24, 24, 2, 3, 3, 3, 3, 2, 24, 24, 24, 25, 5, 25, 3, 2, 24, 24, 24, 2, 2, 4, 4, 4, 24, 24, 24, 4, 23, 23, 23, 4, 4, 4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 8, 21, 21, 21, 8, 8, 8, 8, 8, 21, 21, 21, 8, 8, 8, 8, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 9, 22, 22, 4, 4, 4, 4, 4, 4, 4, 22, 22, 5, 5, 4, 4, 4, 4, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4}, {33, 33, 33, 4, 4, 4, 4, 33, 33, 33, 33, 33, 4, 4, 33, 33, 33, 33, 33, 33, 9, 4, 4, 33, 33, 33, 33, 33, 23, 23, 4, 4, 33, 33, 4, 4, 23, 4, 5, 4}, {4, 4, 4, 4, 4, 4, 4, 4, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 4, 4, 22, 22, 22, 27, 27, 4, 25, 4, 4, 4, 4, 27, 27, 27, 4, 4, 25, 4, 4}, {31, 31, 31, 31, 2, 2, 25, 25, 31, 31, 31, 31, 25, 2, 16, 16, 31, 31, 31, 16, 16, 16, 16, 16, 31, 31, 2, 16, 16, 16, 16, 16, 9, 2, 2, 25, 2, 2, 16, 16}, {12, 12, 30, 30, 18, 18, 30, 30, 12, 12, 30, 30, 18, 18, 30, 30, 12, 12, 12, 18, 18, 18, 30, 30, 30, 30, 12, 12, 30, 30, 30, 30, 30, 30, 18, 18, 18, 12, 30, 30}, {18, 18, 12, 18, 18, 18, 18, 18, 12, 18, 30, 30, 30, 18, 18, 18, 18, 18, 30, 30, 30, 18, 30, 30, 18, 30, 30, 18, 18, 30, 30, 30, 18, 30, 30, 18, 18, 30, 30, 30}, {25, 18, 18, 18, 18, 12, 18, 18, 18, 18, 18, 18, 12, 18, 18, 25, 22, 30, 22, 22, 22, 18, 18, 18, 30, 30, 18, 18, 18, 18, 18, 18, 12, 12, 12, 18, 18, 18, 25, 18}, {18, 18, 18, 18, 18, 18, 12, 12, 12, 18, 18, 18, 18, 12, 12, 12, 14, 14, 14, 18, 18, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 21, 21, 21, 21, 21, 21, 1, 1, 31, 31, 31, 21, 21, 21, 21, 1, 31, 31, 31}, {22, 22, 1, 1, 1, 1, 1, 1, 16, 1, 1, 1, 25, 25, 25, 1, 16, 16, 25, 1, 1, 1, 1, 1, 16, 16, 16, 1, 1, 25, 1, 25, 16, 1, 1, 1, 1, 1, 1, 1}, {25, 25, 3, 3, 5, 3, 3, 3, 26, 26, 26, 3, 25, 3, 3, 25, 23, 23, 25, 26, 26, 28, 25, 25, 3, 25, 25, 25, 26, 26, 26, 26, 3, 3, 3, 3, 3, 3, 25, 3}, {23, 23, 32, 32, 32, 3, 3, 3, 23, 23, 32, 23, 32, 3, 3, 3, 32, 32, 32, 32, 32, 3, 3, 3, 23, 23, 32, 23, 32, 3, 11, 3, 23, 23, 32, 32, 32, 3, 3, 3}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 4, 26, 26, 26, 4, 4, 4, 4, 4}, {10, 4, 4, 4, 4, 4, 4, 4, 10, 10, 4, 4, 4, 10, 10, 10, 10, 10, 10, 4, 4, 10, 10, 10, 10, 10, 10, 4, 4, 10, 10, 10, 10, 10, 10, 4, 4, 4, 4, 4}, {10, 3, 3, 3, 3, 10, 10, 10, 10, 3, 17, 17, 17, 10, 10, 10, 10, 10, 10, 10, 3, 10, 10, 10, 3, 10, 10, 10, 3, 3, 3, 17, 3, 10, 10, 10, 3, 3, 13, 13}, {3, 3, 3, 3, 3, 3, 25, 3, 17, 17, 25, 5, 3, 22, 22, 17, 5, 3, 17, 17, 17, 17, 25, 22, 5, 5, 3, 25, 3, 3, 25, 25, 5, 5, 3, 3, 3, 3, 25, 3}, {15, 15, 15, 2, 2, 25, 2, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 21, 21, 15, 15, 15, 15, 15, 21, 21, 21, 25, 2, 25, 2, 2, 15, 15, 15}, {2, 2, 4, 4, 4, 4, 4, 4, 16, 16, 16, 16, 16, 2, 4, 25, 16, 16, 16, 16, 16, 16, 4, 4, 22, 22, 22, 22, 22, 22, 9, 22, 16, 16, 16, 16, 4, 4, 22, 22}, {4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8}, {12, 12, 12, 12, 12, 28, 28, 33, 12, 30, 30, 12, 12, 12, 33, 33, 25, 30, 30, 12, 12, 12, 30, 33, 30, 12, 28, 28, 12, 12, 30, 23, 12, 12, 12, 12, 12, 12, 12, 23}, {31, 31, 12, 12, 12, 12, 12, 12, 31, 31, 30, 12, 28, 30, 30, 12, 31, 30, 30, 12, 12, 30, 30, 12, 31, 28, 12, 30, 12, 12, 12, 30, 30, 30, 12, 12, 12, 12, 12, 30}, {8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 4, 4, 4, 25, 4, 8}, {17, 17, 17, 17, 17, 17, 17, 17, 4, 4, 10, 10, 10, 4, 4, 17, 10, 10, 10, 10, 10, 4, 10, 10, 10, 10, 10, 10, 10, 4, 10, 10, 10, 4, 4, 4, 4, 4, 10, 10}, {27, 27, 9, 2, 2, 2, 2, 33, 27, 27, 27, 10, 10, 2, 33, 33, 10, 10, 10, 10, 10, 2, 2, 10, 10, 10, 10, 2, 2, 2, 11, 10, 10, 10, 10, 2, 11, 2, 2, 10}};

Board::Board() : nowScreen(0), nowRow(0), nowCol(std::uniform_int_distribution<>(0, cols - 1)(Game::getRng())), backupScreen(0), backupRow(0), backupCol(0)
{
	MsgBox::initBoard(this);
}

void Board::placeTerrain()
{
	// random screens in game
	std::vector<int> perm;
	for (int i = 0; i < totalScreens; i++)
		perm.push_back(i);
	std::shuffle(perm.begin(), perm.end(), Game::getRng());
	for (int i = 0; i < screens - 1; i++) // the last screen is Moonpool
	{
		int screen = perm[i];
		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				int terrain = boardTerrains[screen][row * cols + col];
				cells[i][row][col].setTerrain(&terrains[terrain]);
				if (row > 0 && row < rows - 1) // items not in the first and last row
					terrains[terrain].addCell(&cells[i][row][col]);
			}
		}
	}
}

template <class T>
void Board::placeItems(const std::vector<T> &items)
{
	for (int i = 0; i < items.size(); i++)
	{
		const auto &counts = items[i].getCounts();
		const auto &places = items[i].getPlaces();
		int rarity = counts[std::uniform_int_distribution<>(0, counts.size() - 1)(Game::getRng())];
		while (rarity--)
		{
			int place = places[std::uniform_int_distribution<>(0, places.size() - 1)(Game::getRng())];
			Cell *pos;
			if (terrains[place].randomCell(pos))
			{
				pos->setItem(&items[i]);
				if (typeid(T) == typeid(Enemy)) // if it is an enemy, add it to enemyCells
					enemyCells.push_back(pos);
			}
		}
	}
}

void Board::placeItems()
{
	placeItems(preys);
	placeItems(injuries);
	placeItems(benefits);
	placeItems(defenseActions);
	placeItems(enemies);
}

void Board::placeStars()
{
	for (int i = 0; i < stars.size(); i++)
	{
		int idx = std::uniform_int_distribution<>(0, enemyCells.size() - 1)(Game::getRng());
		Cell *pos = enemyCells[idx];
		pos->setStar(&stars[i]);
		enemyCells[idx] = enemyCells.back();
		enemyCells.pop_back();
	}
	// bug compatible with flash version
	cells[0][1][2].setStar(&stars[2]);
	cells[0][1][2].setItem(&enemies[6]);
}

void Board::renderScreen(bool fallback)
{
	mvprintw(0, 0, fallback ? "+" : "┌");
	for (int i = 1; i < 72; i++)
		if (fallback)
			printw(i % 9 ? "-" : "+");
		else
			printw(i % 9 ? "─" : "┬");
	printw(fallback ? "+" : "┐");
	for (int row = rows - 1; row >= 0; row--)
	{
		mvprintw((rows - row) * 3 - 2, 0, fallback ? "|" : "│");
		for (int col = 0; col < cols; col++)
		{
			Cell &cell = cells[nowScreen][row][col];
			if (fallback && row == nowRow && col < nowCol)
				printw("        ");
			else
			{
				if (row == nowRow && col == nowCol)
				{
					attron(A_REVERSE);
					cell.renderTerrain(fallback);
					attroff(A_REVERSE);
				}
				else
					cell.renderTerrain(false);
			}
			printw(fallback ? "|" : "│");
		}
		if (fallback && row == nowRow)
		{
			mvprintw((rows - row) * 3 - 2, 0, fallback ? "|" : "│");
			for (int col = 0; col < nowCol; col++)
			{
				Cell &cell = cells[nowScreen][row][col];
				cell.renderTerrain(false);
				printw(fallback ? "|" : "│");
			}
		}
		mvprintw((rows - row) * 3 - 1, 0, fallback ? "|" : "│");
		for (int col = 0; col < cols; col++)
		{
			Cell &cell = cells[nowScreen][row][col];
			if (fallback && row == nowRow && col < nowCol)
				printw("        ");
			else
			{
				if (row == nowRow && col == nowCol)
				{
					attron(A_REVERSE);
					cell.renderItem(fallback);
					attroff(A_REVERSE);
				}
				else
					cell.renderItem(false);
			}
			printw(fallback ? "|" : "│");
		}
		if (fallback && row == nowRow)
		{
			mvprintw((rows - row) * 3 - 1, 0, fallback ? "|" : "│");
			for (int col = 0; col < nowCol; col++)
			{
				Cell &cell = cells[nowScreen][row][col];
				cell.renderItem(false);
				printw(fallback ? "|" : "│");
			}
		}
		if (row > 0)
		{
			mvprintw((rows - row) * 3, 0, fallback ? "+" : "├");
			for (int i = 1; i < 72; i++)
				if (fallback)
					printw(i % 9 ? "-" : "+");
				else
					printw(i % 9 ? "─" : "┼");
			printw(fallback ? "+" : "┤");
		}
	}
	mvprintw(rows * 3, 0, fallback ? "+" : "└");
	for (int i = 1; i < 72; i++)
		if (fallback)
			printw(i % 9 ? "-" : "+");
		else
			printw(i % 9 ? "─" : "┴");
	printw(fallback ? "+" : "┘");
}

void Board::clearScreen(Cell::Visibility visibility)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[nowScreen][i][j].setVisibility(visibility);
}

bool Board::move(int ch, Cat &cat)
{
	switch (ch)
	{
	case KEY_UP:
		if (++nowRow >= rows)
		{
			nowRow = 0;
			if (++nowScreen >= screens - 1)
			{
				std::string msg(MultiString({"You found Moonpool!", "你找到了月池！"}));
				if (cat.getStars() < 4)
				{
					MsgBox::create(msg.c_str(), MultiString({"But you don't have enough stars.", "但你没有足够的星星。"}));
					nowScreen--;
					nowRow = rows - 1;
				}
				else
				{
					MsgBox::create(msg.c_str(), (MultiString({"You earned your warrior name: ", "你获得了你的战士名字："}) + cat.getWarriorName()).c_str());
					endwin();
					exit(0);
				}
			}
			clearScreen();
		}
		break;
	case KEY_DOWN:
		if (--nowRow < 0)
		{
			nowRow = rows - 1;
			if (--nowScreen < 0)
			{
				nowScreen = 0;
				nowRow = 0;
				return false;
			}
			clearScreen();
		}
		break;
	case KEY_LEFT:
		if (--nowCol < 0)
		{
			nowCol = 0;
			return false;
		}
		break;
	case KEY_RIGHT:
		if (++nowCol >= cols)
		{
			nowCol = cols - 1;
			return false;
		}
		break;
	default:
		return false;
	}
	const Terrain *terrain = cells[nowScreen][nowRow][nowCol].getTerrain();
	if (terrain >= &terrains[15] && terrain <= &terrains[18] && !cat.getWaterproof())
		cat.loseHealth(1);
	return true;
}

void Board::backupCoordinates()
{
	backupRow = nowRow;
	backupCol = nowCol;
	backupScreen = nowScreen;
}

void Board::restoreCoordinates()
{
	nowRow = backupRow;
	nowCol = backupCol;
	nowScreen = backupScreen;
}

bool Board::trigger(Cat &cat)
{
	Cell &cell = cells[nowScreen][nowRow][nowCol];
	bool res = cell.trigger(cat);
	if (cat.hasFled())
	{
		nowRow = 0;
		nowCol = std::uniform_int_distribution<>(0, cols - 1)(Game::getRng());
		cat.resetFled();
	}
	return res;
}

int Board::getScreen() const
{
	return nowScreen;
}

void Board::moveTarget(int ch)
{
	if (ch == KEY_UP)
	{
		if (targetRow < rows - 1)
			targetRow++;
	}
	else if (ch == KEY_DOWN)
	{
		if (targetRow > 0)
			targetRow--;
	}
	else if (ch == KEY_LEFT)
	{
		if (targetCol > 0)
			targetCol--;
	}
	else if (ch == KEY_RIGHT)
	{
		if (targetCol < cols - 1)
			targetCol++;
	}
}

void Board::chooseTarget(bool diagonal, bool fallback)
{
	mvprintw(rows * 3 + 8, 0, MultiString({"Choose a target with arrow keys and press enter.", "使用方向键选择目标并回车。"}));
	backupCoordinates();
	int ch = getch();
	for (;;)
	{
		targetRow = nowRow;
		targetCol = nowCol;
		moveTarget(ch);
		if (diagonal)
		{
			halfdelay(1);
			int ch = getch();
			moveTarget(ch);
		}
		nowRow = targetRow;
		nowCol = targetCol;
		renderScreen(fallback);
		restoreCoordinates();
		ch = getch();
		if (ch == '\n' || ch == '\r')
			break;
	}
	mvprintw(rows * 3 + 8, 0, std::string(boxWidth, ' ').c_str());
	refresh();
}

void Board::battle(Cat &cat, bool fallback)
{
	chooseTarget(cat.getDiagonal(), fallback);
	Cell &cell = cells[nowScreen][targetRow][targetCol];
	if (cell.isEnemy())
	{
		cell.trigger(cat);
		if (cat.hasFled())
		{
			nowRow = 0;
			nowCol = std::uniform_int_distribution<>(0, cols - 1)(Game::getRng());
			cat.resetFled();
		}
		else
		{
			nowRow = targetRow;
			nowCol = targetCol;
		}
	}
	else
		MsgBox::create(MultiString({"Nothing to battle here.", "这里没有可以战斗的东西。"}));
}

void Board::hunt(Cat &cat, bool fallback)
{
	chooseTarget(cat.getDiagonal(), fallback);
	if (cells[nowScreen][targetRow][targetCol].hunt(cat))
	{
		cells[nowScreen][targetRow][targetCol].setItem(nullptr);
		nowRow = targetRow;
		nowCol = targetCol;
	}
}

void Board::explore(Cat &cat, bool fallback)
{
	chooseTarget(cat.getDiagonal(), fallback);
	cells[nowScreen][targetRow][targetCol].setVisibility(Cell::Visibility::visible);
}

bool Board::getWater() const
{
	const Terrain *terrain = cells[nowScreen][nowRow][nowCol].getTerrain();
	if (terrain == &terrains[28])
		return true;
	if (terrain >= &terrains[15] && terrain <= &terrains[18])
		return true;
	return false;
}