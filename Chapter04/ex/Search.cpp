#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <limits>

struct GraphNode {
	std::vector<GraphNode*> mAdjacent; //각 노드는 인접 노드의 포인터를 가지고 있음
};

struct Graph {
	std::vector<GraphNode*> mNodes; //그래프는 노드을을 포함함
};

struct WeightedEdge {
	//이 에지에 어떤 노드가 연결돼 있는지
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	float mWeight; //이 에지의 가중치
};

struct WeightedGraphNode {
	std::vector<WeightedEdge*> mEdges; //외부로 향하는 에지를 저장
};

//가중 그래프는 WeightedGraphNode를 가짐
struct WeightedGraph {
	std::vector<WeightedGraphNode*> mNodes;
};

using NodeToParentMap = std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap) {
	bool pathFound = false; //경로를 찾았는지 알 수 있는 플래그
	std::queue<const GraphNode*> q; //고려해야 될 노드
	q.emplace(start); //시작 노드를 큐에 넣음
	while (!q.empty()) {
		//큐에서 노드를 꺼냄
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal) {
			pathFound = true;
			break;
		}
		//큐에는 없는 인접 노드를 꺼냄
		for (const GraphNode* node : current->mAdjacent) {
			//시작 노드를 제외하고 부모가 null이면 큐에 넣지 않은 노드임
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start) {
				//이 노드의 부모 노드를 설정하고 큐에 넣음
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}
	return pathFound;
}

struct GBFSScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using GBFSMap = std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b) { return 0.f; }

bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start, const WeightedGraphNode* goal, GBFSMap& outMap) {
	std::vector<const WeightedGraphNode*> openSet;

	//시작 노드를 현재 노드로 설정하고 닫힌 집합에 있다고 마킹
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//열린 집합으로 인접 노드 추가
		for (const WeightedEdge* edge : current->mEdges) {
			GBFSScratch& data = outMap[edge->mTo]; //이 노드의 추가 데이터를 얻음
			//딛힌 집합에 있는 노드가 아니라면 테스트 필요
			if (!data.mInClosedSet) {
				data.mParentEdge = edge; //인접 노드의 부모 에지를 설정
				if (!data.mInOpenSet) {
					//이 노드의 휴리스틱을 계산하고 열린 집합에 추가
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		//열린 집합이 비어있다면 경로가 존재하지 않음
		if (openSet.empty()) break; //루프에서 벗어남

		//열린 집합에서 가장 비용이 낮은 노드를 찾음
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				return outMap[a].mHeuristic < outMap[b].mHeuristic;
			}
		);
		//현재 노드로 설정하고 열린 집합에서 닫힌 집합으로 이동시킴
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false; //경로를 찾았으면
}

struct AStarScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	float mActualFromStart = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using AStarMap = std::unordered_map<const WeightedGraphNode*, AStarScratch>;

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start, const WeightedGraphNode* goal, AStarMap& outMap) {
	std::vector< const WeightedGraphNode*> openSet;

	//시작 노드를 현재 노드로 설정하고 닫힌 집합에 있다고 마킹
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//열린 집합으로 인접 노드 추가
		for (const WeightedEdge* edge : current->mEdges) {
			const WeightedGraphNode* neighbor = edge->mTo;
			AStarScratch& data = outMap[neighbor]; //이 노드의 추가 데이터를 얻음
			//닫힌 집합에 없는지 확인
			if (!data.mInClosedSet) {
				if (!data.mInOpenSet) {
					//열린 집합에 없다면 부모는 반드시 열린 집합에 있어야 함
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					//실제 비용은 부모의 실제 비용 + 부모에서 자신으로 이동하는 에지의 가중치
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else {
					//현재 노드가 부모 노드가 될지를 판단하고자 새로운 실제 비용을 계산
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						//현재 노드가 이 노드의 부모 노드로 채택됨
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}

			//열린 집합이 비어있다면 경로가 존재하지 않음
			if (openSet.empty()) break;

			//열린 집합에서 가장 비용이 낮은 노드를 찾음
			auto iter = std::min_element(openSet.begin(), openSet.end(),
				[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
					float f0fA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
					float f0fB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
					return f0fA < f0fB;
				}
			);
			//현재 노드로 설정하고 열린 집합에서 닫힌 집합으로 이동시킴
			current = *iter;
			openSet.erase(iter);
			outMap[current].mInOpenSet = false;
			outMap[current].mInClosedSet = true;
		}
	} while (current != goal);

	return (current == goal) ? true : false; //경로를 찾았으면
}

struct GameState {
	enum SquareState { Empty, X, O };
	SquareState mBoard[3][3];
};

struct GTNode {
	std::vector<GTNode*> mChildren; //자식 노드
	GameState mState; //이 노드의 게임 상태
};

void GetStates(GTNode* root, bool xPlayer) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (root->mState.mBoard[i][j] == GameState::Empty) {
				GTNode* node = new GTNode;
				root->mChildren.emplace_back(node);
				node->mState = root->mState;
				node->mState.mBoard[i][j] = xPlayer ? GameState::X : GameState::O;
				GetStates(node, !xPlayer);
			}
		}
	}
}

float GetScore(const GameState& state) {
	//같은 행이 있을 경우
	for (int i = 0; i < 3; i++) {
		bool same = true;
		GameState::SquareState v = state.mBoard[i][0];
		for (int j = 1; j < 3; j++) {
			if (state.mBoard[i][j] != v) same = false;
		}
		if (same) {
			if (v == GameState::X) return 1.f;
			else return -1.f;
		}
	}
	//같은 열이 있을 경우
	for (int j = 0; j < 3; j++) {
		bool same = true;
		GameState::SquareState v = state.mBoard[0][j];
		for (int i = 1; i < 3; i++) {
			if (state.mBoard[i][j] != v) same = false;
		}
		if (same) {
			if (v == GameState::X) return 1.f;
			else return -1.f;
		}
	}
	if (
		((state.mBoard[0][0] == state.mBoard[1][1]) &&
			(state.mBoard[1][1] == state.mBoard[2][2])) ||
		((state.mBoard[2][0] == state.mBoard[1][1]) &&
			(state.mBoard[1][1] == state.mBoard[0][2]))
		) {
		if (state.mBoard[1][1] == GameState::X) return 1.f;
		else return -1.f;
	}
	return 0.f; //비김
}

float MinPlayer(const GTNode* node);

float MaxPlayer(const GTNode* node) {
	//리프 노드라면 점수를 반환
	if (node->mChildren.empty()) return GetScore(node->mState);

	//최댓값을 가지는 하위 트리를 찾음
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, MinPlayer(child));
	}
	
	return maxValue;
}

float MinPlayer(const GTNode* node) {
	//리프 노드라면 점수를 반환
	if (node->mChildren.empty()) return GetScore(node->mState);

	//최솟값을 가지는 하위 트리 찾음
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
		minValue = std::min(minValue, MaxPlayer(child));

	return minValue;
}

const GTNode* MinimaxDecide(const GTNode* root) {
	//최대값을 가진 서브 트리를 찾아서 선택을 저장
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren) {
		float v = MinPlayer(child);
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta);

float AlphaBetaMax(const GTNode* node, float alpha, float beta) {
	//리프 노드라면 점수를 반환
	if (node->mChildren.empty()) return GetScore(node->mState);

	float maxValue = -std::numeric_limits<float>::infinity();
	//최대값을 가진 서브 트리를 찾음
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta)
			return maxValue; //베타 가지치기
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta) {
	//리프 노드라면 점수를 반환
	if (node->mChildren.empty()) return GetScore(node->mState);

	float minValue = std::numeric_limits<float>::infinity();
	//최소값을 가진 서브 트리를 찾음
	for (const GTNode* child : node->mChildren) {
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha)
			return minValue; //알파 가지치기
		beta = std::max(minValue, beta);
	}
	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root) {
	const GTNode* choice = nullptr;

	//알파는 음의 무한대, 베타는 양의 무한대 값으로 시작
	//최대값을 가진 서브 트리를 찾아서 선택을 저장
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren) {
		float v = AlphaBetaMin(child, maxValue, beta);
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

void testBFS() {
	Graph g;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GraphNode* node = new GraphNode;
			g.mNodes.emplace_back(node);
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0) node->mAdjacent.emplace_back(g.mNodes[(i - 1) * 5 + j]);
			if (i < 4) node->mAdjacent.emplace_back(g.mNodes[(i + 1) * 5 + j]);
			if (j > 0) node->mAdjacent.emplace_back(g.mNodes[i * 5 + j - 1]);
			if (j < 4) node->mAdjacent.emplace_back(g.mNodes[i * 5 + j + 1]);
		}
	}
	NodeToParentMap map;
	bool found = BFS(g, g.mNodes[0], g.mNodes[9], map);
	std::cout << found << '\n';
}

void testHeuristic(bool useAStar) {
	WeightedGraph g;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			WeightedGraphNode* node = new WeightedGraphNode;
			g.mNodes.emplace_back(node);
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			WeightedGraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i - 1) * 5 + j];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (i < 4) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i + 1) * 5 + j];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j > 0) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j - 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j < 4) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j + 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
		}
	}
	bool found = false;
	if (useAStar) {
		AStarMap map;
		found = AStar(g, g.mNodes[0], g.mNodes[9], map);
	}
	else {
		GBFSMap map;
		found = GBFS(g, g.mNodes[0], g.mNodes[9], map);
	}
	std::cout << found << '\n';
}

void testTicTac() {
	GTNode* root = new GTNode;
	root->mState.mBoard[0][0] = GameState::O;
	root->mState.mBoard[0][1] = GameState::Empty;
	root->mState.mBoard[0][2] = GameState::X;
	root->mState.mBoard[1][0] = GameState::X;
	root->mState.mBoard[1][1] = GameState::O;
	root->mState.mBoard[1][2] = GameState::O;
	root->mState.mBoard[2][0] = GameState::X;
	root->mState.mBoard[2][1] = GameState::Empty;
	root->mState.mBoard[2][2] = GameState::Empty;

	GetStates(root, true);
	const GTNode* choice = AlphaBetaDecide(root);
	std::cout << choice->mChildren.size();
}

