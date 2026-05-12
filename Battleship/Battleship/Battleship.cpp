#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <conio.h>

using namespace std;

class board
{
protected:
	const int rows = 10;
	const int columns = 10;

	const char SHIP = 'M';
	const char HIT = 'X';
	const char MISS = 'O';
	const char BUFFER = '*';
	const char GRID = '.'; //board background
	const char LETTER = 'A'; // the letter from which the coordinates start

	static const int max_ship_l = 4; // the length of the longest ship
	// specifies how many ships of certain type there are
	const int ship1x = 4;
	const int ship2x = 3;
	const int ship3x = 2;
	const int ship4x = 1;

	// the array of ship type counts 
	int ship_count[max_ship_l + 1] = { 0, ship1x, ship2x, ship3x, ship4x };
	int totalships;

	char** canvas;
	char** ships_layout;

public:
	board() {
		totalships = 0;

		//initializing canvas
		canvas = new char* [rows];
		for (int i = 0; i < rows; i++)
			canvas[i] = new char[columns];

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				canvas[i][j] = GRID;

		//initializing ships_layout
		ships_layout = new char* [rows];
		for (int i = 0; i < rows; i++)
			ships_layout[i] = new char[columns];

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				ships_layout[i][j] = ' ';
	}
	~board() {
		for (int i = 0; i < rows; i++) delete[] canvas[i];
		delete[] canvas;

		for (int i = 0; i < rows; i++) delete[] ships_layout[i];
		delete[] ships_layout;
	}

	void show(bool setup) const { // shows the canvas of the board
		//in setup mode shows the count of ships that can be placed
		if (setup) {
			cout << "Ships left: \n";
			for (int i = 1; i < max_ship_l + 1; i++)
				cout << i << " size = " << ship_count[i] << endl;
			cout << "Health: " << totalships << endl;
		}
		char letter = LETTER;
		cout << "  ";
		for (int i = 1; i <= columns; i++)
			cout << i << " ";

		cout << endl;
		for (int i = 0; i < rows; i++) {
			cout << letter++ << " ";
			for (int j = 0; j < columns; j++)
				cout << canvas[i][j] << ' ';
			cout << endl;
		}
	}
	void show_layout() const {
		char letter = 'A';
		cout << "  ";
		for (int i = 1; i <= columns; i++)
			cout << i << " ";
		cout << endl;
		for (int i = 0; i < rows; i++) {
			cout << letter++ << " ";
			for (int j = 0; j < columns; j++)
				cout << ships_layout[i][j] << ' ';
			cout << endl;
		}
	}
	bool mark(int x, int y) {
		if (ships_layout[x][y] == SHIP) {
			canvas[x][y] = HIT;
			ships_layout[x][y] = HIT;
			totalships--;
			return true;
		}
		else if (ships_layout[x][y] != HIT) {
			canvas[x][y] = MISS;
			return false;
		}
		return false;
	}
	bool Input_Correct(int x, int y, int length, char direction) {


		if (x < 0 || x > rows - 1 || y < 0 || y > columns - 1 ||
			(direction != 'V' && direction != 'H') ||
			length < 1 || length > max_ship_l || ship_count[length] < 1)
			return false;

		if (ships_layout[x][y] == SHIP || ships_layout[x][y] == BUFFER)
			return false;

		switch (direction)
		{
		case 'V':
			if (x + length - 1 > rows - 1) return false;
			for (int i = x; i < x + length; i++)
				if (ships_layout[i][y] == SHIP || ships_layout[i][y] == BUFFER)
					return false;
			break;
		case 'H':
			if (y + length - 1 > columns - 1) return false;
			for (int i = y; i < y + length; i++)
				if (ships_layout[x][i] == SHIP || ships_layout[x][i] == BUFFER)
					return false;
			break;
		}
		ship_count[length]--;
		return true;
	}
	void set_ship(int x, int y, int length, char direction) {

		switch (direction)
		{
		case 'V':
			// sets buffer for vertically placed ship
			if (x - 1 >= 0) {
				if (y - 1 >= 0 && ships_layout[x - 1][y - 1] != SHIP)
					ships_layout[x - 1][y - 1] = BUFFER;
				if (y + 1 < columns && ships_layout[x - 1][y + 1] != SHIP)
					ships_layout[x - 1][y + 1] = BUFFER;
				if (ships_layout[x - 1][y] != SHIP)
					ships_layout[x - 1][y] = BUFFER;
			}
			if (x + length < rows) {
				if (y - 1 >= 0 && ships_layout[x + length][y - 1] != SHIP)
					ships_layout[x + length][y - 1] = BUFFER;
				if (y + 1 < columns && ships_layout[x + length][y + 1] != SHIP)
					ships_layout[x + length][y + 1] = BUFFER;
				if (ships_layout[x + length][y] != SHIP)
					ships_layout[x + length][y] = BUFFER;
			}

			// sets the ship vertically
			for (int i = x; i < x + length; i++) {
				canvas[i][y] = SHIP;
				ships_layout[i][y] = SHIP;
				if (y + 1 < columns && ships_layout[i][y + 1] != SHIP)
					ships_layout[i][y + 1] = BUFFER;
				if (y - 1 >= 0 && ships_layout[i][y - 1] != SHIP)
					ships_layout[i][y - 1] = BUFFER;
				totalships++;
			}
			break;

		case 'H':
			// sets buffer for horizontally placed ship
			if (y - 1 >= 0) {
				if (x - 1 >= 0 && ships_layout[x - 1][y - 1] != SHIP)
					ships_layout[x - 1][y - 1] = BUFFER;
				if (x + 1 < rows && ships_layout[x + 1][y - 1] != SHIP)
					ships_layout[x + 1][y - 1] = BUFFER;
				if (ships_layout[x][y - 1] != SHIP)
					ships_layout[x][y - 1] = BUFFER;
			}
			if (y + length < columns) {
				if (x - 1 >= 0 && ships_layout[x - 1][y + length] != SHIP)
					ships_layout[x - 1][y + length] = BUFFER;
				if (x + 1 < rows && ships_layout[x + 1][y + length] != SHIP)
					ships_layout[x + 1][y + length] = BUFFER;
				if (ships_layout[x][y + length] != SHIP)
					ships_layout[x][y + length] = BUFFER;
			}

			// sets the ship horizontally
			for (int i = y; i < y + length; i++) {
				canvas[x][i] = SHIP;
				ships_layout[x][i] = SHIP;
				if (x + 1 < rows && ships_layout[x + 1][i] != SHIP)
					ships_layout[x + 1][i] = BUFFER;
				if (x - 1 >= 0 && ships_layout[x - 1][i] != SHIP)
					ships_layout[x - 1][i] = BUFFER;
				totalships++;
			}
			break;
		}
	}
	void reset_ships() {
		// reset ship counts
		ship_count[0] = 0;
		ship_count[1] = ship1x;
		ship_count[2] = ship2x;
		ship_count[3] = ship3x;
		ship_count[4] = ship4x;

		// clear both grids
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++) {
				canvas[i][j] = GRID;
				ships_layout[i][j] = ' ';
			}

		// resets totalships
		totalships = 0;
	}
	int get_columns() const { return columns; }
	int get_total()   const { return totalships; }
};

// ----------------------------- PLAYER CLASS -----------------------------
class enemy;
class player : public board
{
public:
	bool shoot(enemy& e_board, bool& player_hits);

	bool is_unshot(int x, int y) {
		return !( HIT==canvas[x][y] || MISS == canvas[x][y]);
	}
};

// ----------------------------- ENEMY CLASS -----------------------------
class enemy : public board
{
private:
	vector<pair<int, int>> coordinates;
	vector<pair<int, int>> all_coordinates;

	enum class AIMode { HUNT, TARGET };
	AIMode mode = AIMode::HUNT;

	vector<pair<int, int>> hit_chain; // consecutive hits on the same ship
	char locked_direction; // V - vertical, H - horizontal or \0 (unknown yet)
	bool dir_lock; // is direction locked
	bool front_blocked;
	bool back_blocked;

	bool hit;

public:

	enemy() {
		srand(time(0));
		hit = false;
		mode = AIMode::HUNT;
		locked_direction = '\0';
		dir_lock = false;
		front_blocked = false;
		back_blocked = false;

		bool initial_mode = rand() % 2;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++) {
				if (initial_mode == 0) {

					if ((i + j) % 2 == 0)
						coordinates.push_back({ i, j });
				}
				else {
					if ((i + j) % 2 != 0)
						coordinates.push_back({ i, j });
				}
				all_coordinates.push_back({ i,j });
			}

	}
	~enemy() {
		coordinates.clear();
		all_coordinates.clear();
	}

	// places enemy ships randomly, may place not all ships in certain cases
	void set_ships() {

		// 1x ships placed
		int count = ship_count[1];
		for (int j = 0; j < count;j++) {
			const int MAX_ATTEMPTS = rand()%2; // regulates the randomness of ships
			for (int k = 0; k < MAX_ATTEMPTS; k++) {

				int x = rand() % rows;
				int y = rand() % columns;
				char direction = (rand() % 2) ? 'V' : 'H';

				if (board::Input_Correct(x, y, 1, direction)) {
					set_ship(x, y, 1, direction);
					break;
				}

			}
		}
		// other ships placed
		for (int len = 2; len < max_ship_l+1; len++) {
			int count = ship_count[len];
			for (int j = 0; j < count;j++) {
				const int MAX_ATTEMPTS = 1 + rand() % max_ship_l; // regulates the randomness of ships
				for (int k = 0; k < MAX_ATTEMPTS; k++) {

					int x = rand() % rows;
					int y = rand() % columns;
					char direction = (rand() % 2) ? 'V' : 'H';

					if (board::Input_Correct(x, y, len, direction)) {
						set_ship(x, y, len, direction);
						break;
					}

				}
			}
		}

		if (totalships == 0) {
			reset_ships();
			set_ships();
		}

		// clears the radar
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++) {
				canvas[i][j] = GRID;
			}
	}
	bool Input_correct(int x, int y) {
		return !(x < 0 || x > rows - 1 || y < 0 || y > columns - 1);
	}

	// HELPERS
	bool is_valid_and_unshot(int x, int y, player& p_board) {
		if (Input_correct(x, y) && p_board.is_unshot(x,y))
			return true;
		else return false;
	}

	void remove_from_coordinates(pair<int, int> target) {
		for (int i = 0; i < coordinates.size();i++) {
			if (coordinates[i] == target) {
				if (!coordinates.empty())
					coordinates.erase(coordinates.begin() + i);
				break;
			}
		}
		for (int i = 0; i < all_coordinates.size();i++) {
			if (all_coordinates[i] == target) {
				if (!all_coordinates.empty())
					all_coordinates.erase(all_coordinates.begin() + i);
				break;
			}
		}
	}

	void infer_direction() {
		if (hit_chain.size() < 2) return; // need at least 2 to know direction

		// Compare first and second hit
		if (hit_chain[0].first == hit_chain[1].first)
			locked_direction = 'H'; // same row -> horizontal
		else
			locked_direction = 'V'; // same col -> vertical

		dir_lock = true;
	}

	void reset_state() {
		hit_chain.clear();
		dir_lock = false;
		locked_direction = '\0';
		front_blocked = false;
		back_blocked = false;
		mode = AIMode::HUNT;
	}

	void check_boundary_blocks() {
		pair<int, int> front = hit_chain.front();
		pair<int, int> back = hit_chain.back();

		if (locked_direction == 'H') {
			if (front.second - 1 < 0) front_blocked = true;
			if (back.second + 1 >= columns) back_blocked = true;
		}
		else if (locked_direction == 'V') {
			if (front.first - 1 < 0) front_blocked = true;
			if (back.first + 1 >= rows) back_blocked = true;
		}
	}

	vector<pair<int, int>> get_target_candidates(player& p_board) {
		vector<pair<int, int>> candidates;

		pair<int, int> front = hit_chain.front();
		pair<int, int> back = hit_chain.back();

		if (!dir_lock) {
			// only 1 hit — try all 4 neighbours
			int dx[] = { -1,  1,  0, 0 };
			int dy[] = { 0,  0, -1, 1 };
			for (int d = 0; d < 4; d++) {
				int nx = front.first + dx[d];
				int ny = front.second + dy[d];
				if (is_valid_and_unshot(nx, ny, p_board))
					candidates.push_back({ nx, ny });
			}
		}
		else {
			// direction known — only shoot along that axis
			if (locked_direction == 'H') {
				if (!front_blocked && is_valid_and_unshot(front.first, front.second - 1, p_board))
					candidates.push_back({ front.first, front.second - 1 });
				if (!back_blocked && is_valid_and_unshot(back.first, back.second + 1, p_board))
					candidates.push_back({ back.first, back.second + 1 });
			}
			else { // 'V'
				if (!front_blocked && is_valid_and_unshot(front.first - 1, front.second, p_board))
					candidates.push_back({ front.first - 1, front.second });
				if (!back_blocked && is_valid_and_unshot(back.first + 1, back.second, p_board))
					candidates.push_back({ back.first + 1, back.second });
			}
		}
		return candidates;
	}
	pair<int, int> pick_hunt_target() {
		if (!coordinates.empty()) {
			int pos = rand() % coordinates.size();
			pair<int, int> target = coordinates[pos];
			coordinates.erase(coordinates.begin() + pos);

			for (int i = 0; i < all_coordinates.size(); i++) {
				if (all_coordinates[i] == target) {
					all_coordinates.erase(all_coordinates.begin() + i);
					break;
				}
			}
			return target;
		}
		else {
			// checkerboard exhausted — fall back to remaining cells
			int pos = rand() % all_coordinates.size();
			pair<int, int> target = all_coordinates[pos];
			all_coordinates.erase(all_coordinates.begin() + pos);
			return target;
		}
	}
	pair<int, int> pick_target_target(player& p_board) {
		auto candidates = get_target_candidates(p_board);

		if (candidates.empty()) {
			if (!dir_lock) {
				// truly exhausted with no direction — must be sunk (1x1 case)
				reset_state();
				return pick_hunt_target();
			}

			// candidates empty but ship NOT confirmed sunk
			// we went one direction and hit a miss/wall — flip both block flags
			// so get_target_candidates tries the opposite end
			front_blocked = false;
			back_blocked = false;

			// re-check from opposite perspective
			candidates = get_target_candidates(p_board);

			if (candidates.empty()) {
				// now truly nothing left — ship is sunk
				reset_state();
				return pick_hunt_target();
			}
		}

		pair<int, int> target = candidates[0];
		remove_from_coordinates(target);
		return target;
	}
	void handle_miss(pair<int, int> target, player& p_board) {
		if (mode != AIMode::TARGET) return;

		pair<int, int> front = hit_chain.front();
		pair<int, int> back = hit_chain.back();

		if (!dir_lock) {
			// check if all 4 neighbours are now exhausted
			auto remaining = get_target_candidates(p_board);
			if (remaining.empty())
				reset_state(); // 1x1 ship was sunk
			return; // direction unknown, just keep trying neighbours
		}

		// figure out which end we missed on and block it
		if (locked_direction == 'H') {
			if (target.second == front.second - 1) front_blocked = true;
			if (target.second == back.second + 1) back_blocked = true;
		}
		else {
			if (target.first == front.first - 1) front_blocked = true;
			if (target.first == back.first + 1) back_blocked = true;
		}

		if (front_blocked && back_blocked)
			reset_state(); // ship is sunk
	}
	void handle_hit(pair<int, int> target) {
		mode = AIMode::TARGET;
		hit_chain.push_back(target);

		sort(hit_chain.begin(), hit_chain.end());

		if (hit_chain.size() >= 2)
			infer_direction();

		if (dir_lock)
			check_boundary_blocks();

	}

	// SHOOTING AI
	void shoot(player& p_board) {

		pair<int, int> target;

		// pick where to shoot
		if (mode == AIMode::HUNT)
			target = pick_hunt_target();
		else
			target = pick_target_target(p_board);

		// fire
		hit = p_board.mark(target.first, target.second);

		// update AI state
		if (hit)
			handle_hit(target);
		else
			handle_miss(target, p_board);

	}
	bool get_hit() { return hit; };
};

// -------------------------------- PLAYER SHOOT --------------------------------
bool player::shoot(enemy& e_board, bool& player_hits) {

	char a; int b;
	if (!(cin >> a >> b)) {
		cin.clear(); cin.ignore(1000, '\n');
		return false;
	}
	a = toupper(a);
	int x = (int)a - 65;
	int y = b - 1;
	if (e_board.Input_correct(x, y)) {
		player_hits = e_board.mark(x, y);
		return true;
	}
	else return false;

}

// ----------------------------- DISPLAY CLASS -----------------------------
class display {
public:
	void show_play(const player& player_board, const enemy& enemy_board) {
		enemy_board.show_layout();
		cout << "RADAR" << endl;
		cout << "Enemy fleet health: " << enemy_board.get_total() << endl;
		enemy_board.show(false);
		for (int i = 0; i < enemy_board.get_columns() * 2 + 2; i++)
			cout << "-";
		cout << endl;
		cout << "FLEET STATUS" << endl;
		cout << "Your fleet health: " << player_board.get_total() << endl;
		player_board.show(false);
		cout << "Waiting for orders: ";
	}
	void show_setup(const player& player_board) {
		cout << "Place your ships Captain!" << endl << endl;
		cout << "Accepted order: (coordinate) (length) (direction)\ne.g. C3 4 V/H\nV for vertical, H for horizontal placement" << endl << endl;
		cout << "When done enter P to continue" << endl;
		cout << "Enter R to reset ship placement" << endl;
		cout << endl;
		cout << "YOUR FLEET" << endl;
		player_board.show(true);
		cout << "Waiting for orders: ";
	}
	void main_menu() {
		cout << "Press:" << endl;
		cout << "1. Play" << endl;
		cout << "2. Exit" << endl;

	}
	void clear() {
		system("cls");
		cout << "\033[H";
	}
	void wrongInput_msg() {
		clear();
		cout << "Incorrect order, Sir! Try again..." << endl << endl;
	}
	void cant_place_msg() {
		clear();
		cout << "The ship cannot be located there, Captain!" << endl << endl;
	}
};

// ----------------------------- GAME CLASS -----------------------------
class game
{
private:
	display UI;
	player  player_board;
	enemy   enemy_board;
	bool player_hits;
public:
	game() :player_hits(false) {};

	void setup() {
		srand(time(0));
		player_board.reset_ships();
		enemy_board.reset_ships();
		enemy_board.set_ships();
		while (true) {
			UI.show_setup(player_board);
			char a; int b, length; char direction;

			if (!(cin >> a)) {
				cin.clear(); cin.ignore(1000, '\n');
				UI.wrongInput_msg(); continue;
			}
			if (a == 'P' || a == 'p') break;
			else if (a == 'R' || a == 'r') {
				player_board.reset_ships();
				UI.clear();
				continue;
			}
			if (!(cin >> b >> length >> direction)) {
				cin.clear(); cin.ignore(1000, '\n');
				UI.wrongInput_msg(); continue;
			}

			direction = toupper(direction);
			a = toupper(a);
			int x = (int)a - 65;
			int y = b - 1;

			if (player_board.Input_Correct(x, y, length, direction))
				player_board.set_ship(x, y, length, direction);
			else {
				UI.cant_place_msg(); continue;
			}
			UI.clear();
		}
	}

	void play() {
		UI.clear();
		while (true) {
			UI.show_play(player_board, enemy_board);

			if (enemy_board.get_total() == 0) {
				cout << "Enemy anihilated, good job!" << endl;
				UI.main_menu();
				break;
			}
			else if (player_board.get_total() == 0) {
				cout << "Our fleet was overpowered..." << endl;
				UI.main_menu();
				break;

			}

			if (!enemy_board.get_hit()) {
				if (player_board.shoot(enemy_board, player_hits)) {
					if (player_hits) {
						player_hits = false;
						continue;
					}
				}
				else {
					UI.clear();
					UI.wrongInput_msg();
				}
			}
			UI.clear();
			UI.show_play(player_board, enemy_board);
			Sleep(400);
			enemy_board.shoot(player_board);
			UI.clear();
		}
	}
	void main() {
		cout << "    Welcome \n      to \n *BATTLESHIPS*" << endl;
		cout << endl;

		UI.main_menu();
		bool running = true;
		while (running) {
			if (_kbhit()) {
				char button = _getch();
				button = toupper(button);
				switch (button) {
				case '1':
					system("cls");
					setup();
					play();
					break;
				case '2':
					system("cls");
					running = false;
					break;
				}
			}
		}
	}
};

int main()
{
	game GAME;
	GAME.main();
	return 0;
}