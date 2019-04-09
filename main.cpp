#include <cstdio>
#include "string.h"

enum Action {
	moveLeft,
	moveRight,
	loadChicken,
	loadWolf
};

struct State{
public:
	int leftWolfCount;
	int leftChickenCount;
	int leftBoatCount;
	int rightWolfCount;
	int rightChickenCount;
	int rightBoatCount;
	int boatWolfCount;
	int boatChickenCount;
};

struct Node{
public:
	State state;
	Node** successors;
}; 

State initStateObj(){
	State state;
	state.leftWolfCount = 0;
	state.leftChickenCount = 0;
	state.leftBoatCount = 0;
	state.rightWolfCount = 0;
	state.rightChickenCount = 0;
	state.rightBoatCount = 0;
	state.boatWolfCount = 0;
	state.boatChickenCount = 0;

	return state;
}

bool checkSameState(State state1, State state2){
	if(state1.leftWolfCount != state2.leftWolfCount)
		return false;
	if(state1.leftChickenCount != state2.leftChickenCount)
		return false;
	if(state1.leftBoatCount != state2.leftBoatCount)
		return false;
	if(state1.rightWolfCount != state2.rightWolfCount)
		return false;
	if(state1.rightChickenCount != state2.rightChickenCount)
		return false;
	if(state1.rightBoatCount != state2.rightBoatCount)
		return false;
	return true;
}


bool checkValidArgs(char** argv){
// Things ot check
// Intila state is a file and valid
// Gaol state is a file and valid
// Search mode is one of the possible options

	return true;
}

State loadStateFromFile(char* stateFile){
	State state = initStateObj();

	//Read in from initialStateFile and Load values into state obj


	state.rightWolfCount = 3;
	state.rightChickenCount = 3;
	state.rightBoatCount = 1;

	return state;
}


//This piece needs some work
bool checkValidAction(State state, Action action){
	switch (action){
		case moveLeft:
			if(state.leftBoatCount == 1)
				return false;
			if(state.boatChickenCount == 0 && state.boatWolfCount == 0)
				return false;
			return true;
		case moveRight:
			if(state.rightBoatCount == 1)
				return false;
			if(state.boatChickenCount == 0 && state.boatWolfCount == 0)
				return false;
			return true;
		case loadChicken:
			if(state.leftBoatCount == 1){
				if(state.leftChickenCount == 0){
					return false;
				}
				if(state.leftChickenCount - 1 < state.leftWolfCount)
					return false;
			}
			if(state.rightBoatCount == 1){
				if (state.rightChickenCount == 0){
					return false;
				}
				if (state.rightChickenCount -1 < state.rightWolfCount)
					return false;
			}
			if(state.boatChickenCount + state.boatWolfCount == 2)
				return false;
			return true;
		case loadWolf:
			if(state.leftBoatCount == 1){
				if(state.leftWolfCount == 0){
					return false;
				}
				if(state.boatWolfCount + state.rightWolfCount + 1 > state.rightChickenCount + state.boatChickenCount)
					return false;
			}
			if(state.rightBoatCount == 1){
				if(state.rightWolfCount == 0){
					return false;
				}
				if(state.boatWolfCount + state.leftWolfCount + 1 > state.leftChickenCount + state.boatChickenCount)
					return false;
			}
			if(state.boatChickenCount + state.boatWolfCount == 2)
				return false;
			return true;

			
	}
	return true;
}

State applyAction(State state, Action action){
	switch(action){
		case moveLeft:
			state.leftBoatCount ++;
			state.rightBoatCount --;
			while (state.boatWolfCount != 0){
				state.boatWolfCount --;
				state.leftWolfCount ++;
			}
			while (state.boatChickenCount != 0){
				state.boatChickenCount --;
				state.leftChickenCount ++;
			}
			return state;
		case moveRight:
			state.leftBoatCount --;
			state.rightBoatCount ++;
			while (state.boatWolfCount != 0){
				state.boatWolfCount --;
				state.rightWolfCount ++;
			}
			while (state.boatChickenCount != 0){
				state.boatChickenCount --;
				state.rightChickenCount ++;
			}
			return state;
		case loadChicken:
			if(state.leftBoatCount == 1)
				state.leftChickenCount --;
			else
				state.rightChickenCount --;	
			state.boatChickenCount ++;
			return state;
		case loadWolf:
			if(state.leftBoatCount == 1)
				state.leftWolfCount --;
			else
				state.rightWolfCount --;	
			state.boatWolfCount ++;
			return state;
		default:
			return state;
	}
}


Node** getStateSuccessors(State state){
	Node** successors = new Node*[4];
	int successorCount = 0;
	if(checkValidAction(state, moveLeft)){
		Node* successorNode = new Node();
		successorNode->state = applyAction(state, moveLeft);
		successors[successorCount] = successorNode;
		successorCount++;
	}	

	if(checkValidAction(state, moveRight)){
		Node* successorNode = new Node();
		successorNode->state = applyAction(state, moveRight);
		successors[successorCount] = successorNode;
		successorCount++;
	}	
	if(checkValidAction(state, loadChicken)){
		Node* successorNode = new Node();
		successorNode->state = applyAction(state, loadChicken);
		successors[successorCount] = successorNode;
		successorCount++;
	}	
	if(checkValidAction(state, loadWolf)){
		Node* successorNode = new Node();
		successorNode->state = applyAction(state, loadWolf);
		successors[successorCount] = successorNode;
		successorCount++;
	}	
	printf("PrintCount: %d\n", successorCount);
}

void executeDepthSearch(Node* node, State goalState){
	if(checkSameState(node->state,goalState)){
		//Solution found, return path
		printf("Solution found");
	}
	node->successors = getStateSuccessors(node->state);
	printf("Size pieces are:\n ObjectSize: %li\n ReturnSize: %li\n", sizeof(Node**), sizeof(node->successors));
}


	
void findSolution(Node* solutionTree, State goalState, char* searchMode){

	//Perform search depending on what mode is selected
	executeDepthSearch(solutionTree, goalState);

}



void writeSolutionToOutput(char** outputFile){

	return;
}



int main(int args, char** argv){
	Node* SolutionTree = new Node();
	
//Check to see if the number of arguments if correct.
	if(args != 5){
		printf("Incorrect Number of argument provided.  Arguments should be:\n");
		printf("<Initial State File> <Goal State File> <Search Mode> <Output File>\n");
		return -1;
	}
//Check if arguments are valid
	if(!checkValidArgs(argv)){
		printf("Invalid arguments\n");
		return -1;
	}

	State initialState = loadStateFromFile(argv[1]);
	State goalState = loadStateFromFile(argv[2]);
	SolutionTree->state = initialState;

	findSolution(SolutionTree, goalState, argv[3]);


	return 0;
}
