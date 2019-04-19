#include <cstdio>
#include "string.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <queue>

using namespace std;

#define ACTIONCOUNT 7

enum Action {
	moveLeft = 0,
	moveRight = 1,
	loadChicken = 2,
	loadDoubleChicken = 3,
	loadWolf = 4,
	loadChickenWolf = 5,
	loadDoubleWolf = 6
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
	Node* parent;
	Node** successors;
	int depth;
	int sortValue;
};

struct aStarCompareDistance{
	bool operator()(Node* const& node1, Node* const& node2)
	{
		//cout << "Comparing node with value " << node1->sortValue << " and " << node2->sortValue << endl;
		return node1->sortValue < node2->sortValue;
		
	}
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

void assignDepth(Node* node)
{
	if(node->parent == NULL)
		node->depth = 0;
	else
		node->depth = node->parent->depth + 1;
}

vector<State> traceParentPath (Node* goalNode){
	vector<State> output;
	Node* currentNode = goalNode;
	while(currentNode->parent != NULL)
	{
	State temp = currentNode->parent->state;
		output.push_back(currentNode->parent->state);
		//printf("Right:%d,%d,%d\nLeft:%d,%d,%d\n\n",
		//	temp.rightChickenCount, temp.rightWolfCount, temp.rightBoatCount,
		//	temp.leftChickenCount, temp.leftWolfCount, temp.leftBoatCount);
		currentNode = currentNode->parent;
	}
	return output;
} 

char* stateToString(State state){
	char* output = new char[100];
	sprintf(output, "%d,%d,%d\n%d,%d,%d\n\n", state.leftChickenCount, state.leftWolfCount, state.leftBoatCount,
		state.rightChickenCount, state.rightWolfCount, state.rightBoatCount);
	return output;
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

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class MyQueue : public std::priority_queue<T, Container, Compare>
{
	public:
		typedef typename
        std::priority_queue<
        T,
        Container,
        Compare>::container_type::const_iterator const_iterator;

    bool find(const T&val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first!=last) {
            if (checkSameState((*first)->state, val->state)) return true;
            ++first;
        }
        return false;
    }
};

bool checkInClosedSet(State state, vector<State>* set){
	for (int i = 0; i < set->size(); i ++){
		if(checkSameState(state, (*set)[i]))
			return true;
	}

	return false;
}

bool checkInClosedSet(State state, vector<Node*>* set){
	for (int i = 0; i < set->size(); i ++){
		if(checkSameState(state, (*set)[i]->state))
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

State loadStateFromFile(char *stateFile)
{
	cout << "READING FILE " << stateFile << endl;
	vector<vector<string> > stateVals;
	State state = initStateObj();
	ifstream infile(stateFile);
	vector<string> lineVals;


	while (infile)
	{
		string s;
		if (!getline(infile, s))
			break;
		istringstream ss(s);
		lineVals.clear();

		while (ss)
		{
			string s;
			if (!getline(ss, s, ','))
				break;
			lineVals.push_back(s);
		}
		stateVals.push_back(lineVals);
	}
	//Read in from initialStateFile and Load values into state obj

	state.leftChickenCount = stoi(stateVals.at(0).at(0));
	state.leftWolfCount = stoi(stateVals.at(0).at(1));
	state.leftBoatCount = stoi(stateVals.at(0).at(2));
	state.rightChickenCount = stoi(stateVals.at(1).at(0));
	state.rightWolfCount = stoi(stateVals.at(1).at(1));
	state.rightBoatCount = stoi(stateVals.at(1).at(2));

	cout << "Left Chicken Count: " << state.leftChickenCount << endl;
	cout << "Left Wolf Count: " << state.leftWolfCount << endl;
	cout << "Left boat Count: " << state.leftBoatCount << endl;

	cout << "Right Chicken Count: " << state.rightChickenCount << endl;
	cout << "Right Wolf Count: " << state.rightWolfCount << endl;
	cout << "Right boat Count: " << state.rightBoatCount << endl;

	cout << "Boat Chicken Count: " << state.boatChickenCount << endl;
	cout << "Boat Wolf Count: " << state.boatWolfCount << endl;

	return state;
}

bool checkValidAction(State state, Action action)
{
	switch (action)
	{
	case moveLeft:
		if (state.leftBoatCount == 1)
			return false;
		if (state.boatChickenCount == 0 && state.boatWolfCount == 0)
			return false;
		return true;
	case moveRight:
		if (state.rightBoatCount == 1)
			return false;
		if (state.boatChickenCount == 0 && state.boatWolfCount == 0)
			return false;
		return true;
	case loadChicken:
		if (state.leftBoatCount == 1)
		{
			if (state.leftChickenCount == 0)
			{
				return false;
			}
		}
		if (state.rightBoatCount == 1)
		{
			if (state.rightChickenCount == 0)
			{
				return false;
			}
		}
		if (state.boatChickenCount + state.boatWolfCount > 0)
			return false;
		return true;
	case loadDoubleChicken:
		if (state.leftBoatCount == 1)
		{
			if (state.leftChickenCount < 2)
			{
				return false;
			}
		}
		if (state.rightBoatCount == 1)
		{
			if (state.rightChickenCount < 2)
			{
				return false;
			}
		}
		if (state.boatChickenCount + state.boatWolfCount > 0)
			return false;
		return true;
	case loadWolf:
		if (state.leftBoatCount == 1)
		{
			if (state.leftWolfCount == 0)
			{
				return false;
			}
		}
		if (state.rightBoatCount == 1)
		{
			if (state.rightWolfCount == 0)
			{
				return false;
			}
		}
		if (state.boatChickenCount + state.boatWolfCount > 0)
			return false;
		return true;
	case loadChickenWolf:
		if (state.leftBoatCount == 1)
		{
			if (state.leftChickenCount == 0 || state.leftWolfCount == 0)
			{
				return false;
			}
		}
		if (state.rightBoatCount == 1)
		{
			if (state.rightChickenCount == 0 || state.rightWolfCount == 0)
			{
				return false;
			}
		}
		if (state.boatChickenCount + state.boatWolfCount > 0)
			return false;
		return true;
	case loadDoubleWolf:
		if (state.leftBoatCount == 1)
		{
			if (state.leftWolfCount < 2)
			{
				return false;
			}
		}
		if (state.rightBoatCount == 1)
		{
			if (state.rightWolfCount < 2)
			{
				return false;
			}
		}
		if (state.boatChickenCount + state.boatWolfCount > 0)
			return false;
		return true;
	}
	return true;
}

State applyAction(State state, Action action)
{
 cout << "Parent state: " << endl << stateToString(state) << endl;
	State returnState;
	switch (action)
	{
	case moveLeft:

		state.leftBoatCount++;
		state.rightBoatCount--;
		while (state.boatWolfCount != 0)
		{
			state.boatWolfCount--;
			state.leftWolfCount++;
		}
		while (state.boatChickenCount != 0)
		{
			state.boatChickenCount--;
			state.leftChickenCount++;
		}
		returnState = state;
		break;
	case moveRight:
		state.leftBoatCount--;
		state.rightBoatCount++;
		while (state.boatWolfCount != 0)
		{
		if(state.boatWolfCount > 2)
				cout << "What the fuck!!!" << endl;
			state.boatWolfCount--;
			state.rightWolfCount++;
		}
		while (state.boatChickenCount != 0)
		{
			if(state.boatChickenCount > 2)
				cout << "What the fuck" << endl;
			state.boatChickenCount--;
			state.rightChickenCount++;
			returnState = state;
		}
		break;
	case loadChicken:
		if (state.leftBoatCount == 1)
			state.leftChickenCount--;
		else
			state.rightChickenCount--;
		state.boatChickenCount++;
		returnState = state;
		break;
	case loadDoubleChicken:
		if (state.leftBoatCount == 1)
			state.leftChickenCount -= 2;
		else
			state.rightChickenCount -= 2;
		state.boatChickenCount += 2;
		returnState = state;
		break;
	case loadWolf:
		if (state.leftBoatCount == 1)
			state.leftWolfCount--;
		else
			state.rightWolfCount--;
		state.boatWolfCount++;
		returnState = state;
		break;
	case loadDoubleWolf:
		if (state.leftBoatCount == 1)
			state.leftWolfCount -= 2;
		else
			state.rightWolfCount -= 2;
		state.boatWolfCount += 2;
		returnState = state;
		break;
	case loadChickenWolf:
		if (state.leftBoatCount == 1)
		{
			state.leftWolfCount--;
			state.leftChickenCount--;
		}
		else
		{
			state.rightWolfCount--;
			state.rightChickenCount--;
		}
		state.boatWolfCount++;
		state.boatChickenCount++;
		returnState = state;
		break;
	default:
		returnState = state;
		break;

		if (returnState.leftChickenCount < returnState.leftWolfCount || returnState.rightChickenCount < returnState.rightWolfCount){
			returnState = initStateObj();
			cout << "here" << endl;
		}
		//printf("New state rightChickenCount: %d\n", returnState.rightChickenCount);

		return returnState;
	}
}

Node *getSingleStateSuccessor(State state, Action action)
{
	if (checkValidAction(state, action))
	{
		cout << "Child for action: " << action << endl;
		Node *successorNode = new Node();
		successorNode->state = applyAction(state, action);
		cout << "Got new successor: " << stateToString(successorNode->state) << endl;
		return successorNode;
	}
	else
	{
		return NULL;
	}
}

Node **getStateSuccessors(State state)
{
	Node **successors = new Node *[ACTIONCOUNT];

	successors[moveLeft] = getSingleStateSuccessor(state, moveLeft);
	successors[moveRight] = getSingleStateSuccessor(state, moveRight);
	successors[loadChicken] = getSingleStateSuccessor(state, loadChicken);
	successors[loadWolf] = getSingleStateSuccessor(state, loadWolf);
	successors[loadDoubleChicken] = getSingleStateSuccessor(state, loadDoubleChicken);
	successors[loadDoubleWolf] = getSingleStateSuccessor(state, loadDoubleWolf);
	successors[loadChickenWolf] = getSingleStateSuccessor(state, loadChickenWolf);

	return successors;
}

vector<State> executeBreadthSearch(Node *node, State goalState, int *nodeCount) 
{
	vector<State> visitedStates, emptySet;
	vector<Node*> frontier;
	State initialNodeState = node->state;
	node->parent = NULL;
	frontier.push_back(node);
	Node* currentNode;


	if (checkSameState(initialNodeState, goalState))
	{
		return traceParentPath(node);
	}
	else
	{
		while (true)
		{
			if (frontier.empty())
				return emptySet;

			currentNode = frontier.front();
			frontier.erase(frontier.begin());
			visitedStates.push_back(currentNode->state);

			Node **successors = new Node *[ACTIONCOUNT];

			for (int i = 0; i < ACTIONCOUNT; i++)
			{
				
				successors[i] = getSingleStateSuccessor(currentNode->state, static_cast<Action>(i));
				if (successors[i] != NULL)
				{
					successors[i]->parent = currentNode;
					if (!checkInClosedSet(successors[i]->state, &visitedStates) && !checkInClosedSet(successors[i]->state, &frontier))
					{
						if (checkSameState(successors[i]->state, goalState))
						{
							
							return traceParentPath(successors[i]);
						}
						else
						{
							frontier.push_back(successors[i]);
						}
					}

					//delete successors[i];
				}
			}

			//delete[] successors;

		}
	}
}

vector<State> executeDepthSearch(Node *node, State goalState, vector<State> *visitedStates, vector<State> path, int *nodeCount)
{
	vector<State> emptySet;
	State nodeState = node->state;
	delete node;

	if (checkSameState(nodeState, goalState))
	{
		path.push_back(nodeState);
		//Solution found, return path
		return path;
	}
	else if (checkInClosedSet(nodeState, visitedStates))
	{
		return emptySet;
	}
	else
	{
		*nodeCount += 1;
		visitedStates->push_back(nodeState);
		path.push_back(nodeState);
		//node->successors = getStateSuccessors(node->state);
		Node **successors = new Node *[ACTIONCOUNT];

		for (int i = 0; i < ACTIONCOUNT; i++)
		{
			successors[i] = getSingleStateSuccessor(nodeState, static_cast<Action>(i));
			if (successors[i] != NULL)
			{
				vector<State> result = executeDepthSearch(successors[i], goalState, visitedStates, path, nodeCount);
				if (!result.empty())
				{
					delete [] successors;
					return result;
				}
			}
		}
		return emptySet;
		//printf("Size pieces are:\n ObjectSize: %li\n ReturnSize: %li\n", sizeof(Node**), sizeof(node->successors));
	}
}

vector<State> executeLimitedDepthSearch(Node *node, State goalState, vector<State> *visitedStates, int limit, vector<State> path, int *nodeCount)
{
	vector<State> emptySet;
	State nodeState = node->state;

	if (checkSameState(node->state, goalState))
	{
		path.push_back(node->state);
		//Solution found, return path
		return path;
	}
	else if (checkInClosedSet(node->state, visitedStates))
	{
		return emptySet;
	}
	else if (limit > 0)
	{
		*nodeCount += 1;
		visitedStates->push_back(nodeState);
		path.push_back(nodeState);
		//node->successors = getStateSuccessors(node->state);
		Node **successors = new Node *[ACTIONCOUNT];

		for (int i = 0; i < ACTIONCOUNT; i++)
		{
			successors[i] = getSingleStateSuccessor(nodeState, static_cast<Action>(i));
			if (successors[i] != NULL)
			{
				vector<State> result = executeDepthSearch(successors[i], goalState, visitedStates, path, nodeCount);
				if (!result.empty())
				{
					delete [] successors;
					return result;
				}
			}
		}
		return emptySet;
		//printf("Size pieces are:\n ObjectSize: %li\n ReturnSize: %li\n", sizeof(Node**), sizeof(node->successors));
	}
	else
	{
		return emptySet;
	}
}

vector<State> executeIterativeDepthSearch(Node *node, State goalState, vector<State> path, int *nodeCount)
{
	vector<State> solution;
	int limitCount = 0;
	while (solution.empty())
	{
		solution = executeLimitedDepthSearch(node, goalState, new vector<State>(), limitCount, path, nodeCount);
		limitCount++;
	}
	return solution;
}

int aStarHeuristic(State state, State goalState)
{
	int sum = 0;
	//cout << "Left boat count " << state.leftBoatCount << endl;
	//cout << "Left goal boat count " << goalState.leftBoatCount << endl;
	//sum += abs(state.leftBoatCount - goalState.leftBoatCount);
	/*sum += abs(state.leftChickenCount - goalState.leftChickenCount);
	sum += abs(state.leftWolfCount - goalState.leftWolfCount);
	sum += abs(state.rightBoatCount - goalState.rightBoatCount);
	sum += abs(state.rightChickenCount - goalState.rightChickenCount);
	sum += abs(state.rightWolfCount - goalState.rightWolfCount);*/

	cout << sum << endl;

	return sum;
}

bool checkIfInPriorityQueue(MyQueue<Node*,vector<Node*>,aStarCompareDistance> frontierQ, Node* node){
			
		return frontierQ.find(node);
}

vector<State> executeAStarSearch(Node *node, State goalState, int *nodeCount) 
{
	vector<State> visitedStates, emptySet;
	MyQueue<Node*, vector<Node*>, aStarCompareDistance> frontierQ;
	State initialNodeState = node->state;
	node->parent = NULL;
	node->sortValue = aStarHeuristic(node->state, goalState);
	//visitedStates.push_back(node->state);
	frontierQ.push(node);
	Node* currentNode;


	if (checkSameState(initialNodeState, goalState))
	{
		return traceParentPath(node);
	}
	else if (checkInClosedSet(initialNodeState, &visitedStates))
	{

		return emptySet;
	}
	else
	{
		while (true)
		{
			if (frontierQ.empty())
			{
				cout << "q is empty" << endl;
				return emptySet;
			}
			/*else{
				cout << "Current Frontier Size: " << frontierQ.size() << endl;
			}*/
			currentNode = frontierQ.top();
			frontierQ.pop();

			currentNode->sortValue = aStarHeuristic(currentNode->state, goalState);

			Node **successors = new Node *[ACTIONCOUNT];

			for (int i = 0; i < ACTIONCOUNT; i++)
			{
				
				successors[i] = getSingleStateSuccessor(currentNode->state, static_cast<Action>(i));
				if (successors[i] != NULL)
				{
					successors[i]->parent = currentNode;
					visitedStates.push_back(currentNode->state);
					//cout << stateToString(successors[i]->state) << endl;
					cout << "HELLOTHERE" << endl;
					if (!checkInClosedSet(successors[i]->state, &visitedStates) && !checkIfInPriorityQueue(frontierQ, successors[i]))
					{
						if (checkSameState(successors[i]->state, goalState))
						{
							cout << "Solution found" << endl;
							return traceParentPath(successors[i]);
						}
						else
						{
							frontierQ.push(successors[i]);
						}
					}
				}
			}
		}
	}
}

void writeSolutionToOutput(vector<State> solutionPath, int *nodesExpanded, char *outputFile)
{
	ofstream openfile(outputFile);

	for (int i = 0; i < solutionPath.size(); i++)
	{
		char *string = stateToString(solutionPath[i]);
		openfile << string << endl;
		;
		delete[] string;
	}
	openfile << "Nodes Expanded: " << *nodesExpanded << endl;

	return;
}

void findSolution(Node *solutionTree, State goalState, char *searchMode, char *outputFile)
{

	vector<State> emptySet;
	int *nodesExpanded = new int(0);
	vector<State> solutionPath;
	//Perform search depending on what mode is selected
	if (strcmp(searchMode, "bfs") == 0)
	{
		solutionPath = executeBreadthSearch(solutionTree, goalState, nodesExpanded);
		for (int i = 0; i < solutionPath.size(); i++)
		{
			char *string = stateToString(solutionPath[i]);
			cout << string << endl;
			delete[] string;
		}
	}
	else if (strcmp(searchMode, "dfs") == 0)
	{
		solutionPath = executeDepthSearch(solutionTree, goalState, new vector<State>, emptySet, nodesExpanded);
		for (int i = 0; i < solutionPath.size(); i++)
		{
			char *string = stateToString(solutionPath[i]);
			cout << string << endl;
			delete[] string;
		}
	}
	else if (strcmp(searchMode, "iddfs") == 0)
	{
		solutionPath = executeIterativeDepthSearch(solutionTree, goalState, emptySet, nodesExpanded);
		for (int i = 0; i < solutionPath.size(); i++)
		{
			char *string = stateToString(solutionPath[i]);
			cout << string << endl;
			delete[] string;
		}
	}
	else if (strcmp(searchMode, "astar") == 0)
	{
		solutionPath = executeAStarSearch(solutionTree, goalState, nodesExpanded);
		for (int i = 0; i < solutionPath.size(); i++)
		{
			char *string = stateToString(solutionPath[i]);
			cout << string << endl;
			delete[] string;
		}
	}
	writeSolutionToOutput(solutionPath, nodesExpanded, outputFile);

	return;
}

int main(int args, char **argv)
{
	Node *SolutionTree = new Node();

	//Check to see if the number of arguments if correct.
	if (args != 5)
	{
		printf("Incorrect Number of argument provided.  Arguments should be:\n");
		printf("<Initial State File> <Goal State File> <Search Mode> <Output File>\n");
		return -1;
	}
	//Check if arguments are valid
	if (!checkValidArgs(argv))
	{
		printf("Invalid arguments\n");
		return -1;
	}

	State initialState = loadStateFromFile(argv[1]);
	State goalState = loadStateFromFile(argv[2]);
	SolutionTree->state = initialState;

	findSolution(SolutionTree, goalState, argv[3], argv[4]);
	cout << "it finished" << endl;

	return 0;
}
