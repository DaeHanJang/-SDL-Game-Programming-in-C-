#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <limits>

struct GraphNode {
	std::vector<GraphNode*> mAdjacent; //�� ���� ���� ����� �����͸� ������ ����
};

struct Graph {
	std::vector<GraphNode*> mNodes; //�׷����� ������� ������
};

struct WeightedEdge {
	//�� ������ � ��尡 ����� �ִ���
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	float mWeight; //�� ������ ����ġ
};

struct WeightedGraphNode {
	std::vector<WeightedEdge*> mEdges; //�ܺη� ���ϴ� ������ ����
};

//���� �׷����� WeightedGraphNode�� ����
struct WeightedGraph {
	std::vector<WeightedGraphNode*> mNodes;
};

using NodeToParentMap = std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap) {
	bool pathFound = false; //��θ� ã�Ҵ��� �� �� �ִ� �÷���
	std::queue<const GraphNode*> q; //����ؾ� �� ���
	q.emplace(start); //���� ��带 ť�� ����
	while (!q.empty()) {
		//ť���� ��带 ����
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal) {
			pathFound = true;
			break;
		}
		//ť���� ���� ���� ��带 ����
		for (const GraphNode* node : current->mAdjacent) {
			//���� ��带 �����ϰ� �θ� null�̸� ť�� ���� ���� �����
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start) {
				//�� ����� �θ� ��带 �����ϰ� ť�� ����
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

	//���� ��带 ���� ���� �����ϰ� ���� ���տ� �ִٰ� ��ŷ
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//���� �������� ���� ��� �߰�
		for (const WeightedEdge* edge : current->mEdges) {
			GBFSScratch& data = outMap[edge->mTo]; //�� ����� �߰� �����͸� ����
			//���� ���տ� �ִ� ��尡 �ƴ϶�� �׽�Ʈ �ʿ�
			if (!data.mInClosedSet) {
				data.mParentEdge = edge; //���� ����� �θ� ������ ����
				if (!data.mInOpenSet) {
					//�� ����� �޸���ƽ�� ����ϰ� ���� ���տ� �߰�
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		//���� ������ ����ִٸ� ��ΰ� �������� ����
		if (openSet.empty()) break; //�������� ���

		//���� ���տ��� ���� ����� ���� ��带 ã��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				return outMap[a].mHeuristic < outMap[b].mHeuristic;
			}
		);
		//���� ���� �����ϰ� ���� ���տ��� ���� �������� �̵���Ŵ
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false; //��θ� ã������
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

	//���� ��带 ���� ���� �����ϰ� ���� ���տ� �ִٰ� ��ŷ
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		//���� �������� ���� ��� �߰�
		for (const WeightedEdge* edge : current->mEdges) {
			const WeightedGraphNode* neighbor = edge->mTo;
			AStarScratch& data = outMap[neighbor]; //�� ����� �߰� �����͸� ����
			//���� ���տ� ������ Ȯ��
			if (!data.mInClosedSet) {
				if (!data.mInOpenSet) {
					//���� ���տ� ���ٸ� �θ�� �ݵ�� ���� ���տ� �־�� ��
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					//���� ����� �θ��� ���� ��� + �θ𿡼� �ڽ����� �̵��ϴ� ������ ����ġ
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else {
					//���� ��尡 �θ� ��尡 ������ �Ǵ��ϰ��� ���ο� ���� ����� ���
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						//���� ��尡 �� ����� �θ� ���� ä�õ�
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}

			//���� ������ ����ִٸ� ��ΰ� �������� ����
			if (openSet.empty()) break;

			//���� ���տ��� ���� ����� ���� ��带 ã��
			auto iter = std::min_element(openSet.begin(), openSet.end(),
				[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
					float f0fA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
					float f0fB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
					return f0fA < f0fB;
				}
			);
			//���� ���� �����ϰ� ���� ���տ��� ���� �������� �̵���Ŵ
			current = *iter;
			openSet.erase(iter);
			outMap[current].mInOpenSet = false;
			outMap[current].mInClosedSet = true;
		}
	} while (current != goal);

	return (current == goal) ? true : false; //��θ� ã������
}

struct GameState {
	enum SquareState { Empty, X, O };
	SquareState mBoard[3][3];
};

struct GTNode {
	std::vector<GTNode*> mChildren; //�ڽ� ���
	GameState mState; //�� ����� ���� ����
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
	//���� ���� ���� ���
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
	//���� ���� ���� ���
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
	return 0.f; //���
}

float MinPlayer(const GTNode* node);

float MaxPlayer(const GTNode* node) {
	//���� ����� ������ ��ȯ
	if (node->mChildren.empty()) return GetScore(node->mState);

	//�ִ��� ������ ���� Ʈ���� ã��
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, MinPlayer(child));
	}
	
	return maxValue;
}

float MinPlayer(const GTNode* node) {
	//���� ����� ������ ��ȯ
	if (node->mChildren.empty()) return GetScore(node->mState);

	//�ּڰ��� ������ ���� Ʈ�� ã��
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren)
		minValue = std::min(minValue, MaxPlayer(child));

	return minValue;
}

const GTNode* MinimaxDecide(const GTNode* root) {
	//�ִ밪�� ���� ���� Ʈ���� ã�Ƽ� ������ ����
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
	//���� ����� ������ ��ȯ
	if (node->mChildren.empty()) return GetScore(node->mState);

	float maxValue = -std::numeric_limits<float>::infinity();
	//�ִ밪�� ���� ���� Ʈ���� ã��
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta)
			return maxValue; //��Ÿ ����ġ��
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta) {
	//���� ����� ������ ��ȯ
	if (node->mChildren.empty()) return GetScore(node->mState);

	float minValue = std::numeric_limits<float>::infinity();
	//�ּҰ��� ���� ���� Ʈ���� ã��
	for (const GTNode* child : node->mChildren) {
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha)
			return minValue; //���� ����ġ��
		beta = std::max(minValue, beta);
	}
	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root) {
	const GTNode* choice = nullptr;

	//���Ĵ� ���� ���Ѵ�, ��Ÿ�� ���� ���Ѵ� ������ ����
	//�ִ밪�� ���� ���� Ʈ���� ã�Ƽ� ������ ����
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

