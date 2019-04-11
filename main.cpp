#include <cstdio>
#include "string.h"
#include <iostream>
#include <vector>

using namespace std;

enum Action {
	moveLeft = 0,
	moveRight = 1,
	loadChicken = 2,
	loadWolf = 3
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

void printStateToScreen(State state){
	printf("%d,%d,%d\n%d,%d,%d\n\n", state.leftChickenCount, state.leftWolfCount, state.leftBoatCount,
		state.rightChickenCount, state.rightWolfCount, state.rightBoatCount);
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

bool checkInClosedSet(State state, vector<State> set){
	for (int i = 0; i < set.size(); i ++){
		if(checkSameState(state, set[i]))
			return true;
	}

	return false;
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
			}
			if(state.rightBoatCount == 1){
				if (state.rightChickenCount == 0){
					return false;
				}

			}
			if(state.boatChickenCount + state.boatWolfCount == 2)
				return false;
			return true;
		case loadWolf:
			if(state.leftBoatCount == 1){
				if(state.leftWolfCount == 0){
					return false;
				}
			}
			if(state.rightBoatCount == 1){
				if(state.rightWolfCount == 0){
					return false;
				}
			}
			if(state.boatChickenCount + state.boatWolfCount == 2)
				return false;
			return true;

			
	}
	return true;
}

State applyAction(State state, Action action){
	State returnState;
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
			returnState = state;
			break;
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
			returnState = state;
			}
			break;
		case loadChicken:
			if(state.leftBoatCount == 1)
				state.leftChickenCount --;
			else
				state.rightChickenCount --;	
			state.boatChickenCount ++;
			returnState = state;
			break;
		case loadWolf:
			if(state.leftBoatCount == 1)
				state.leftWolfCount --;
			else
				state.rightWolfCount --;	
			state.boatWolfCount ++;
			returnState = state;
			break;
		default:
			returnState = state;
			break;

		if(returnState.leftChickenCount < returnState.leftWolfCount || returnState.rightChickenCount < returnState.rightWolfCount)
			returnState = initStateObj();

		//printf("New state rightChickenCount: %d\n", returnState.rightChickenCount);

		return returnState;

	}
}

Node* getSingleStateSuccessor(State state, Action action){
	if(checkValidAction(state, action)){
		//cout << "moveLeft valid" << endl;
		Node* successorNode = new Node();
		successorNode->state = applyAction(state, action);
		return successorNode;
	}
	else{
		return NULL;
	}
}

Node** getStateSuccessors(State state){
	Node** successors = new Node*[4];
	
	successors[moveLeft] = getSingleStateSuccessor(state, moveLeft);
	successors[moveRight] = getSingleStateSuccessor(state, moveRight);
	successors[loadChicken] = getSingleStateSuccessor(state, loadChicken);
	successors[loadWolf] = getSingleStateSuccessor(state, loadWolf);

	return successors;

}

vector<State> executeDepthSearch(Node* node, State goalState, vector<State>* visitedStates, vector<State> path){
	vector<State> emptySet;
	if(checkSameState(node->state,goalState)){
		path.push_back(node->state);
		//Solution found, return path
		return path;
	}
	else if (checkInClosedSet(node->state, *visitedStates)){
		return emptySet;
	}
	else{
		visitedStates->push_back(node->state);
		path.push_back(node->state);
		node->successors = getStateSuccessors(node->state);
		for (int i = 0; i < 4; i++){
			if(node->successors[i] != NULL)
			{
				vector<State> result = executeDepthSearch(node->successors[i], goalState, visitedStates, path);
				if(!result.empty())
					return result;
			}
		}
		return emptySet;
		//printf("Size pieces are:\n ObjectSize: %li\n ReturnSize: %li\n", sizeof(Node**), sizeof(node->successors));
	}
}


	
void findSolution(Node* solutionTree, State goalState, char* searchMode){

	vector<State> emptySet;
	//Perform search depending on what mode is selected
	vector<State> solutionPath = executeDepthSearch(solutionTree, goalState, new vector<State>, emptySet);
	for(int i = 0; i < solutionPath.size(); i ++){
		printStateToScreen(solutionPath[i]);
	}

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
	State goalState = initStateObj();
	goalState.leftChickenCount = 3;
	goalState.leftWolfCount = 3;
	goalState.leftBoatCount = 1;
	SolutionTree->state = initialState;

	findSolution(SolutionTree, goalState, argv[3]);


	return 0;
}
