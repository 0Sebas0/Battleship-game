#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

class board
{
protected:
	const int rows = 10;
	const int columns = 10;

	const char ship = 'M';
	const int max_ship_l = 4;
	const int ship4x = 1;
	const int ship3x = 0;
	const int ship2x = 0;
	const int ship1x = 0;
	int totalships = ship4x * 4 + ship3x * 3 + ship2x * 2 + ship1x;

	char** canvas;
	char** ships_layout;

public:
	board() {
		canvas = new char* [rows];
		for (int i = 0; i < rows; i++) {
			canvas[i] = new char[columns];
		}
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				canvas[i][j] = ' ';
			}
		}
		ships_layout = new char* [rows];
		for (int i = 0; i < rows; i++) {
			ships_layout[i] = new char[columns];
		}
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				ships_layout[i][j] = ' ';
			}
		}
	}
	~board() {
		for (int i = 0; i < rows; i++)
			delete[] canvas[i];
		delete[] canvas;

		for (int i = 0; i < rows; i++)
			delete[] ships_layout[i];
		delete[] ships_layout;
	}

	void show() const {
		char letter = 'A';
		cout << "  ";
		for (int i = 1; i <= columns; i++)
			cout << i << " ";
		cout << endl;
		for (int i = 0; i < rows; i++) {
			cout << letter << " ";
			letter++;
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
			cout << letter << " ";
			letter++;
			for (int j = 0; j < columns; j++)
				cout << ships_layout[i][j] << ' ';
			cout << endl;
		}
	}
	bool mark(int x, int y) {

		if (ships_layout[x][y] == ship) {
			canvas[x][y] = 'X';
			ships_layout[x][y] = 'X';
			totalships--;
			return true;
		}
		else if (ships_layout[x][y] != 'X') {
			canvas[x][y] = 'O';
			return false;
		}
	}
	int get_columns() const { return columns; }
	int get_total() const { return totalships; }
};

// ----------------------------- PLAYER CLASS -----------------------------
class player : public board
{
public:
	void set_ship(int x, int y, int length, char direction) {

		switch (direction)
		{
		case 'V':
			// mark top and bottom of the ship for placement restrictions
			// top    x-1 y-1 /  x-1 y / x-1 y+1
			if (x - 1 >= 0) {
				if (y - 1 >= 0 && ships_layout[x - 1][y - 1] != ship)
					ships_layout[x - 1][y - 1] = '*';

				if (y + 1 < columns && ships_layout[x - 1][y + 1] != ship)
					ships_layout[x - 1][y + 1] = '*';

				if (ships_layout[x - 1][y] != ship)
					ships_layout[x - 1][y] = '*';
			}
			// bottom x+length y-1 / x+length y / x+length y+1
			if (x + length < rows) {
				if (y - 1 >= 0 && ships_layout[x + length][y - 1] != ship)
					ships_layout[x + length][y - 1] = '*';

				if (y + 1 < columns && ships_layout[x + length][y + 1] != ship)
					ships_layout[x + length][y + 1] = '*';

				if (ships_layout[x + length][y] != ship)
					ships_layout[x + length][y] = '*';
			}

			for (int i = x; i < x + length; i++) {
				canvas[i][y] = ship;
				ships_layout[i][y] = ship;

				//mark arround ship for placement restrictions
				   //right x y+1 
				if (y + 1 < columns) {
					if (ships_layout[i][y + 1] != ship)
						ships_layout[i][y + 1] = '*';
				}

				//left  x y-1 
				if (y - 1 >= 0) {
					if (ships_layout[i][y - 1] != ship)
						ships_layout[i][y - 1] = '*';
				}
			}
			break;

		case 'H':
			// mark left and right of the ship for placement restrictions
			// left    x-1 y-1 / x y-1 / x+1 y-1 
			if (y - 1 >= 0) {
				if (x - 1 >= 0 && ships_layout[x - 1][y - 1] != ship)
					ships_layout[x - 1][y - 1] = '*';

				if (x + 1 < rows && ships_layout[x + 1][y - 1] != ship)
					ships_layout[x + 1][y - 1] = '*';

				if (ships_layout[x][y - 1] != ship)
					ships_layout[x][y - 1] = '*';
			}
			// right   x-1 y+length / x y+length / x+1 y+length
			if (y + length < columns) {
				if (x - 1 >= 0 && ships_layout[x - 1][y + length] != ship)
					ships_layout[x - 1][y + length] = '*';

				if (x + 1 < rows && ships_layout[x + 1][y + length] != ship)
					ships_layout[x + 1][y + length] = '*';

				if (ships_layout[x][y + length] != ship)
					ships_layout[x][y + length] = '*';
			}

			for (int i = y; i < y + length; i++) {
				canvas[x][i] = ship;
				ships_layout[x][i] = ship;

				//mark arround ship for placement restrictions
				   //bottom     x+1 y / is x+1 out of bounds?
				if (x + 1 < rows) {
					if (ships_layout[x + 1][i] != ship)
						ships_layout[x + 1][i] = '*';
				}

				//top  x-1 y / is x-1 out of bounds?
				if (x - 1 >= 0) {
					if (ships_layout[x - 1][i] != ship)
						ships_layout[x - 1][i] = '*';
				}


			}
			break;
		}

	}
	bool Input_Correct(int x, int y, int length, char direction) {
		if (x<0 || x>rows - 1 || y<0 || y>columns - 1 || (direction != 'V' && direction != 'H') || length < 1 || length > max_ship_l) {

			return false;
		}
		switch (direction)
		{
		case 'V':
			if (x + length - 1 > rows - 1) return false;
			for (int i = x; i < x + length - 1; i++) {
				if (ships_layout[i][y] == ship || ships_layout[i][y] == '*')
					return false;
			}
			break;
		case 'H':
			if (y + length - 1 > columns - 1) return false;
			for (int i = y; i < y + length - 1; i++) {
				if (ships_layout[x][i] == ship || ships_layout[x][i] == '*')
					return false;
			}
			break;
		}

		return true;
	}

};
// ----------------------------- ENEMY CLASS -----------------------------
class enemy : public board
{
private:
	vector <pair<int, int>> coordinates;
	pair<int, int> hit_coordinate;
public:
	bool hit;
	enemy() {
		hit = false;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++)
			{
				coordinates.push_back({ i, j });
			}
		}
	}
	~enemy() { coordinates.clear(); };
	void set_ships() {
		char a = 'C';
		int b = 3;
		int x = (int)a - 65;
		int y = b - 1;
		int length = 4;
		for (int i = y; i < y + length; i++)
			ships_layout[x][i] = ship;
	}
	bool Input_correct(int x, int y)
	{
		if (x<0 || x>rows - 1 || y<0 || y>columns - 1) {
			return false;
		}
		return true;
	}
	void shoot(player& p_board) {
		srand(time(0));
		int pos = rand() % coordinates.size();
		if (hit) {
			hit = p_board.mark(coordinates[pos].first, coordinates[pos].second);
			hit_coordinate = { coordinates[pos].first, coordinates[pos].second };
		}
		else {
			hit = p_board.mark(coordinates[pos].first, coordinates[pos].second);
		}
		coordinates.erase(coordinates.begin() + pos);
	}
};

// ----------------------------- DISPLAY CLASS -----------------------------
class display {
public:

	void show_play(const player& player_board, const enemy& enemy_board) {
		//cout << "ENEMY SHIPS" << endl;
		//cout << "Health: " << enemy_board.get_total() << endl;
		//enemy_board.show_layout();
		cout << "RADAR" << endl;
		enemy_board.show();
		for (int i = 0; i < enemy_board.get_columns() * 2 + 2; i++)
			cout << "-";
		cout << endl;
		cout << "FLEET STATUS" << endl;
		//cout << "Health: " << player_board.get_total() << endl;
		player_board.show();

		cout << "Waiting for orders: ";
	}
	void show_setup(const player& player_board)
	{
		cout << "Place your ships Captain!" << endl;
		cout << endl;
		cout << "Accepted order: (coordinate) (length) (direction) \ne.g. C3 4 V/H\nV for vertical, H for horizontal placement" << endl;
		cout << endl;
		cout << "When done enter P to continue" << endl;
		cout << endl;
		cout << "YOUR FLEET" << endl;
		player_board.show();

		cout << "Waiting for orders: ";
	}
	void clear()
	{
		system("cls");
		cout << "\033[H";
	}
	void wrongInput_msg()
	{
		clear();
		cout << "Incorrect order, Sir! Try again..." << endl;
		cout << endl;
	}
	void cant_place_msg()
	{
		clear();
		cout << "The ship cannot be located there, Captain!" << endl;
		cout << endl;
	}
};

// ----------------------------- GAME CLASS -----------------------------
class game
{
private:
	display UI;
	player player_board;
	enemy enemy_board;
public:
	void setup() {

		enemy_board.set_ships(); // places the ships of the enemy
		while (true) {
			UI.show_setup(player_board);

			char a;
			int b;
			int length;
			char direction;

			if (!(cin >> a)) { // handle bad input types
				cin.clear();
				cin.ignore(1000, '\n');
				UI.wrongInput_msg();
				continue;
			}
			if (a == 'P' || a == 'p') break;

			if (!(cin >> b >> length >> direction)) {   // handle bad input types
				cin.clear();
				cin.ignore(1000, '\n');
				UI.wrongInput_msg();
				continue;
			}

			direction = toupper(direction);
			a = toupper(a);
			int x = (int)a - 65;
			int y = b - 1;

			if (player_board.Input_Correct(x, y, length, direction)) {
				player_board.set_ship(x, y, length, direction);
			}
			else {
				UI.cant_place_msg();
				continue;
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
				break;
			}
			else if (player_board.get_total() == 0) {
				cout << "Our fleet was overpowered..." << endl;
				break;
			}

			char a;
			int b;
			if (enemy_board.hit == false) {
				if (!(cin >> a >> b)) {   // handle bad input types
					cin.clear();
					cin.ignore(1000, '\n');
					UI.wrongInput_msg();
					continue;
				}

				a = toupper(a);
				int x = (int)a - 65;
				int y = b - 1;
				if (enemy_board.Input_correct(x, y)) { // check is input correct

					if (!enemy_board.hit) {
						if (enemy_board.mark(x, y)) {
							UI.clear();
							continue;
						}
					}
				}
				else {
					UI.wrongInput_msg();
					continue;
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