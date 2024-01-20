#pragma once

class Scoreboard
{
	unsigned int currentMap;
	unsigned int p1Score;
	unsigned int p2Score;

	public:
		Scoreboard() {
			currentMap = 0;
			p1Score = 0;
			p2Score = 0;
		};
		~Scoreboard() {};
		void win(int playerID) {
			if(playerID == 1)
				p1Score++;
			else
				p2Score++;

			currentMap++;
			//pause the game for 1000 ms
			SDL_Delay(1000);

			if (currentMap == 3) {
				std::cout << "GAME OVER\n";
				std::cout << "Player 1: " << p1Score << " points\n";
				std::cout << "Player 2: " << p2Score << " points\n";
				currentMap = 0;
			}

		}
		int checkCurrentMap() {
			return currentMap;
		}
};

