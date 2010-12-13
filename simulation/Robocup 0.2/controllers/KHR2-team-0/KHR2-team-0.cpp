#include GoalKeeper.hpp
#include Forward.hpp
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

int main(int argc, const *argv[]) {
	
	if (argc < 3) {
	cout << "Error: could not find teamID and playerID in controllerArgs" << endl;
	return 0;
	}
	
	int teamNum   = atoi (argv[1]);
	int playerNum = atoi (argv[2]);
	
	Player *player = NULL;
	if (playerID == 0)
	player = new GoalKeeper(teamID, playerID);
	else
	player = new FieldPlayer(teamID, playerID);
	
	player->run();
	
	delete player;
}