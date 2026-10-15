#include "include/Graphs.hpp"
#include <print>
#include <iostream>
#include <fstream>

using Graphs::UnDirectedGraph;

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <filesystem>
#include <string>
#include <format>
#include <print>

template <typename T, typename U>
void print_export_to_txt(const std::filesystem::path &filepath, const std::vector<std::pair<T, U>> &data) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo .txt para exportação.");
    }
    int vertex_width = 5;

    for (const auto &[vertex, value] : data) {
        // Formata a linha usando largura fixa para o vértice ({:<15} alinha à esquerda em 15 espaços)
        std::string linha_console = std::format("{:<{}} | {}", vertex, vertex_width, value);
        std::string linha_arquivo = std::format("{:<{}}{}{}", vertex, vertex_width, "\t", value); // Ou mantenha largura no arquivo se preferir

        // Imprime formatado
        std::println("{}", linha_console);
        file << linha_arquivo << "\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::println(std::cerr, "Uso: {} <arquivo_entrada.gexf>", argv[0]);
        return 1;
    }

    UnDirectedGraph graph = UnDirectedGraph<std::string>(std::filesystem::path(argv[1]));

    const auto eccentricities = graph.get_eccentricities();
    const auto normalized_closeness_centrality = graph.get_normalized_closeness_centralities();

    std::println("\nImprimindo e exportando as excentricidades dos vértices.");
    print_export_to_txt("eccentricity.txt", eccentricities);
    std::println("\nImprimindo e exportando as centralizades normalizadas dos vértices.");
    print_export_to_txt("closeness_centrality.txt", normalized_closeness_centrality);
    return 0;
}