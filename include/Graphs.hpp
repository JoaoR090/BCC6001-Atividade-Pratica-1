#pragma once

/**
 * Namespace contendo tipos de grafos.
 * Sua implementação decorre das notas aulas de um aluno. 
 * Por isso há outras implementações que não são usadas no resto do código.
 */

#include "libgexf/edgeiter.h"
#include "libgexf/gexf.h"
#include "libgexf/nodeiter.h"
#include "libgexf/undirectedgraph.h"
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <flat_set>
#include <queue>
#include <utility>
#include <concepts>
#include <print>
#include <algorithm>
#include <libgexf/libgexf.h>
#include <filesystem>

namespace Graphs {

    template <typename T>
    concept Hashable = requires(T a, T b) {
        { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
        { a == b } -> std::convertible_to<bool>;
    };

    template <typename T, typename Weight = int>
    struct Edge {
        T dest;
        Weight weight{1};

        bool operator<(const Edge& other) const {
            if (dest != other.dest) {
                return dest < other.dest;
            }
            return weight < other.weight;
        }

        bool operator==(const Edge& other) const = default;
    };

    template <Hashable T, typename Weight = int>
    class Graph {
    protected:
        std::unordered_map<T, std::flat_set<Edge<T, Weight>>> adj;

        libgexf::GEXF read(const std::filesystem::path &path) {
            if (!std::filesystem::exists(path)) {
                throw std::runtime_error("Arquivo .gexf não existe.");
            }

            libgexf::FileReader reader = libgexf::FileReader();
            reader.init(path.string());
            reader.slurp();
            return reader.getGEXFCopy();
        }

    public:
        Graph(const size_t initial_size = 0) {
            this->adj.reserve(initial_size);
        }

        void add_vertex(const T& vertex) {
            if (!this->adj.contains(vertex)) {
                this->adj[vertex] = {};
            }
        }

        void push_edges(std::initializer_list<std::tuple<T, T, Weight>> initializer_list) {
            for (const auto &[source, end, weight]: initializer_list) {
                this->push_edge(source, end, weight);
            }
        }

        void push_edges(std::initializer_list<std::pair<T, T>> initializer_list) {
            for (const auto &[source, end]: initializer_list) {
                this->push_edge(source, end, Weight{1});
            }
        }

        virtual void push_edge(const T &source, const T &end, Weight weight = Weight{1}) = 0;
        virtual void print() const = 0;
        virtual ~Graph() = default;

        const std::unordered_map<T, std::flat_set<Edge<T, Weight>>>& get_adj() const {
            return this->adj;
        }

        bool is_accessible(const T& source, const T& end) const {
            if (source == end) return true;
            if (!this->adj.contains(source) || !this->adj.contains(end)) return false;

            std::unordered_set<T> visited;
            std::queue<T> q;

            visited.insert(source);
            q.push(source);

            while (!q.empty()) {
                T curr = q.front();
                q.pop();

                if (curr == end) return true;

                if (auto it = this->adj.find(curr); it != this->adj.end()) {
                    for (const auto& edge: it->second) {
                        if (!visited.contains(edge.dest)) {
                            visited.insert(edge.dest);
                            q.push(edge.dest);
                        }
                    }
                }
            }

            return false;
        }

        void depth_first_search(const T &start) {
            if (!this->adj.contains(start)) return;

            std::unordered_set<T> visited;
            std::stack<T> s;

            visited.insert(start);
            s.push(start);
            
            while(!s.empty()) {
                T curr = s.top();
                s.pop();

                std::println("DFS: Visitado {}", curr);

                if (auto it = this->adj.find(curr); it != this->adj.end()) {
                    for (const auto &edge: it->second) {
                        if (!visited.contains(edge.dest)) {
                            visited.insert(edge.dest);
                            s.push(edge.dest);
                        }
                    }
                }
            }
        }

        void breadth_first_search(const T &start) {
            if (!this->adj.contains(start)) return;
            
            std::unordered_set<T> visited;
            std::queue<T> q;

            visited.insert(start);
            q.push(start);
            
            while (!q.empty()) {
                T curr = q.front();
                q.pop();
                
                std::println("BFS: Visitado {}", curr);

                if (auto it = this->adj.find(curr); it != this->adj.end()) {
                    for (const auto &edge: it->second) {
                        if (!visited.contains(edge.dest)) {
                            visited.insert(edge.dest);
                            q.push(edge.dest);
                        }
                    }
                }
            }
        }
    };

    template <Hashable T, typename Weight = int>
    class UnDirectedGraph: public Graph<T, Weight> {
    public:
        UnDirectedGraph(const size_t initial_size = 0): Graph<T, Weight>(initial_size) {}

        UnDirectedGraph(std::initializer_list<std::pair<T, T>> initializer_list): Graph<T, Weight>(initializer_list.size()) {
            this->push_edges(initializer_list);
        }

        UnDirectedGraph(const std::filesystem::path &path) {
            libgexf::GEXF gexf = this->read(path);
            libgexf::UndirectedGraph graph = gexf.getUndirectedGraph();
            libgexf::EdgeIter *edgeIter = graph.getEdges();
            libgexf::NodeIter *nodeIter = graph.getNodes();

            while(nodeIter->hasNext()) {
                this->add_vertex(nodeIter->next());
            }

            while(edgeIter->hasNext()) {
                edgeIter->next();
                this->push_edge(edgeIter->currentSource(), edgeIter->currentTarget());
            }

            delete nodeIter;
            delete edgeIter;
        }

        void push_edge(const T &source, const T &end, Weight weight = Weight{1}) override {
            this->add_vertex(source);
            this->add_vertex(end);
            this->adj[source].insert(Edge<T, Weight>{end, weight});
            this->adj[end].insert(Edge<T, Weight>{source, weight});
        }

        void print() const override {
            for (const auto &[source, adjacencies]: this->adj) {
                for (const auto &edge: adjacencies) {
                    if constexpr (requires { source < edge.dest; }) {
                        if (source > edge.dest) continue;
                    }
                    if (edge.weight == Weight{1}) {
                        std::println("{} - {}", source, edge.dest);
                    } else {
                        std::println("{} -({}) - {}", source, edge.weight, edge.dest);
                    }
                }
            }
        }

        size_t get_degree(const T &source) const {
            if (const auto it = this->adj.find(source); it != this->adj.end()) {
                return it->second.size();
            }
            return 0;
        }

        size_t get_max_degree() const {
            size_t degree = 0;
            for (const auto &[source, adjacencies]: this->adj) {
                degree = std::max(degree, adjacencies.size());
            }
            return degree;
        }

        // Calcula a excentricidade de um vértice
        // Este código não usa os pesos mesmo que estejam guardados internamente
        // uma vez que a especificação da Atividade prática não considerada pesos (ao menos implicitamente).
        size_t get_eccentricity(const T &start) const {
            if (!this->adj.contains(start)) return 0;

            // Executamos uma busca em largura mas agora calculamos as distâncias durante o processo
            std::unordered_map<T, size_t> distances;
            std::queue<T> q;

            distances[start] = 0;
            q.push(start);
            
            size_t max_dist = 0;
            while (!q.empty()) {
                T curr = q.front();
                q.pop();

                size_t current_dist = distances[curr];
                // Atualizamos a maior distância encontrada em toda iteração
                max_dist = std::max(max_dist, current_dist);

                if (auto it = this->adj.find(curr); it != this->adj.end()) {
                    for (const auto &edge: it->second) {
                        if (!distances.contains(edge.dest)) {
                            distances[edge.dest] = current_dist + 1;
                            q.push(edge.dest);
                        }
                    }
                }
            }

            // Agora temos a maior distância encontrada
            return max_dist;
        }

        // Retorna todas as excentricidades dos vértices.
        std::vector<std::pair<T, size_t>> get_eccentricities() const {
            std::vector<std::pair<T, size_t>> result;
            result.reserve(this->adj.size());
            
            for (const auto &[vertex, _]: this->adj) {
                result.emplace_back(vertex, get_eccentricity(vertex));
            }
            
            return result;
        }

        // Calcula a centralidade normalizada de um vértice
        // Este código não usa os pesos mesmo que estejam guardados internamente
        // uma vez que a especificação da Atividade prática não considerada pesos (ao menos implicitamente).
        double get_normalized_closeness_centrality(const T &start) const {
            if (!this->adj.contains(start)) return 0.0;

            size_t total_nodes = this->adj.size();
            if (total_nodes <= 1) return 0.0;

            // Executamos uma busca em largura mas agora calculamos as distâncias durante o processo
            std::unordered_map<T, size_t> distances;
            std::queue<T> q;

            distances[start] = 0;
            q.push(start);
            
            size_t sum_distances = 0;
            while (!q.empty()) {
                T curr = q.front();
                q.pop();

                size_t current_dist = distances[curr];
                if (curr != start) {
                    sum_distances += current_dist;
                }

                if (auto it = this->adj.find(curr); it != this->adj.end()) {
                    for (const auto &edge: it->second) {
                        if (!distances.contains(edge.dest)) {
                            distances[edge.dest] = current_dist + 1;
                            q.push(edge.dest);
                        }
                    }
                }
            }

            size_t reachable_nodes = distances.size(); // Inclui o próprio vértice inicial
            if (reachable_nodes <= 1 || sum_distances == 0) return 0.0;

            // Fórmula de Wasserman-Faust para normalização em grafos gerais/desconexos:
            // C = ((n - 1) / (N - 1)) * ((n - 1) / sum_distances)
            // Onde n é o número de nós alcançáveis (incluindo o start) e N é o total de nós do grafo.
            double n_minus_1 = static_cast<double>(reachable_nodes - 1);
            double N_minus_1 = static_cast<double>(total_nodes - 1);
            
            return (n_minus_1 / N_minus_1) * (n_minus_1 / static_cast<double>(sum_distances));
        }

        // Retorna todas as centralidades normalizadas dos vértices.
        std::vector<std::pair<T, double>> get_normalized_closeness_centralities() const {
            std::vector<std::pair<T, double>> result;
            result.reserve(this->adj.size());
            
            for (const auto &[vertex, _]: this->adj) {
                result.emplace_back(vertex, get_normalized_closeness_centrality(vertex));
            }
            
            return result;
        }
    };

    template <Hashable T, typename Weight = int>
    class DirectedGraph: public Graph<T, Weight> {
    public:
        DirectedGraph(const size_t initial_size = 0): Graph<T, Weight>(initial_size) {}

        DirectedGraph(std::initializer_list<std::pair<T, T>> initializer_list): Graph<T, Weight>(initializer_list.size()) {
            this->push_edges(initializer_list);
        }

        DirectedGraph(const std::filesystem::path &path) {
            libgexf::GEXF gexf = this->read(path);
            libgexf::DirectedGraph graph = gexf.getDirectedGraph();
            libgexf::EdgeIter *edgeIter = graph.getEdges();
            libgexf::NodeIter *nodeIter = graph.getNodes();

            while(nodeIter->hasNext()) {
                this->add_vertex(nodeIter->next());
            }

            while(edgeIter->hasNext()) {
                edgeIter->next();
                this->push_edge(edgeIter->currentSource(), edgeIter->currentTarget());
            }

            delete nodeIter;
            delete edgeIter;
        }

        void push_edge(const T &source, const T &end, Weight weight = Weight{1}) override {
            this->add_vertex(source);
            this->add_vertex(end);
            this->adj[source].insert(Edge<T, Weight>{end, weight});
        }

        void print() const override {
            for (const auto &[source, adjacencies]: this->adj) {
                for (const auto &edge: adjacencies) {
                    if (edge.weight == Weight{1}) {
                        std::println("{} -> {}", source, edge.dest);
                    } else {
                        std::println("{} -({})-> {}", source, edge.weight, edge.dest);
                    }
                }
            }
        }

        size_t get_in_degree(const T &end) const {
            size_t degree = 0;
            for (const auto &[source, adjacencies]: this->adj) {
                for (const auto &edge: adjacencies) {
                    if (edge.dest == end) {
                        ++degree;
                    }
                }
            }
            return degree;
        }

        size_t get_max_in_degree() const {
            std::unordered_map<T, size_t> in_degrees;

            for (const auto &[source, adjacencies]: this->adj) {
                if (!in_degrees.contains(source)) {
                    in_degrees[source] = 0;
                }
                for (const auto &edge: adjacencies) {
                    in_degrees[edge.dest]++;
                }
            }

            size_t max_deg = 0;
            for (const auto &[vertex, deg]: in_degrees) {
                max_deg = std::max(max_deg, deg);
            }
            return max_deg;
        }

        size_t get_out_degree(const T &source) const {
            if (const auto it = this->adj.find(source); it != this->adj.end()) {
                return it->second.size();
            }
            return 0;
        }

        size_t get_max_out_degree() const {
            size_t degree = 0;
            for (const auto &[source, adjacencies]: this->adj) {
                degree = std::max(degree, adjacencies.size());
            }
            return degree;
        }
    };
}