#include <iostream>
#include <Windows.h>
#include <vector>

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
			cout <<"Health: " << totalships << endl;
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
	int get_columns() const { return columns; }
	int get_total()   const { return totalships; }
};

// ----------------------------- PLAYER CLASS -----------------------------
class player : public board
{
public:
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
};

// ----------------------------- ENEMY CLASS -----------------------------
class enemy : public board
{
private:
	vector<pair<int, int>> coordinates;
public:
	bool hit;
	enemy() {
		hit = false;
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				coordinates.push_back({ i, j });
	}
	~enemy() { coordinates.clear(); }

	// places enemy ships randomly, may place not all ships in certain cases
	void set_ships() {
		const int MAX_ATTEMPTS = 1+rand()%2; // regulates the randomness of ships

		for (int len = max_ship_l; len >=1; len--) {
			int count = ship_count[len];
			for (int j = 0; j < count;j++) {
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
		// clears the radar
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++) {
				canvas[i][j] = GRID;
			}
	}
	bool Input_correct(int x, int y) {
		return !(x < 0 || x > rows - 1 || y < 0 || y > columns - 1);
	}
	void shoot(player& p_board) {

		int pos = rand() % coordinates.size();
		p_board.mark(coordinates[pos].first, coordinates[pos].second);
		coordinates.erase(coordinates.begin() + pos);
	}
};

// ----------------------------- DISPLAY CLASS -----------------------------
class display {
public:
	void show_play(const player& player_board, const enemy& enemy_board) {
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
public:
	void setup() {
		srand(time(0));
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
				cout << "Enemy anihilated, good job!" << endl; break;
			}
			else if (player_board.get_total() == 0) {
				cout << "Our fleet was overpowered..." << endl; break;
			}

			char a; int b;
			if (!enemy_board.hit) {
				if (!(cin >> a >> b)) {
					cin.clear(); cin.ignore(1000, '\n');
					UI.wrongInput_msg(); continue;
				}
				a = toupper(a);
				int x = (int)a - 65;
				int y = b - 1;
				if (enemy_board.Input_correct(x, y)) {
					if (enemy_board.mark(x, y)) { UI.clear(); continue; }
				}
				else {
					UI.wrongInput_msg(); continue;
				}
			}
			UI.clear();
			UI.show_play(player_board, enemy_board);
			Sleep(400);
			enemy_board.shoot(player_board);
			UI.clear();
		}
	}
};

int main()
{
	game GAME;
	GAME.setup();
	GAME.play();
	system("pause");
	return 0;
}