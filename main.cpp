#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

const int windowWidth = 800;
const int windowHeight = 800;
const int cellSize = 10;
const int gridWidth = windowWidth / cellSize;
const int gridHeight = windowHeight / cellSize;

using Grid = std::vector<std::vector<bool>>;

void initializeGrid(Grid& grid) {
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            grid[i][j] = rand() % 2;
        }
    }
}

int countAliveNeighbors(const Grid& grid, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}

void updateGrid(const Grid& current, Grid& next) {
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            int aliveNeighbors = countAliveNeighbors(current, i, j);
            if (current[i][j]) {
                next[i][j] = (aliveNeighbors == 3 || aliveNeighbors == 2);
            } else {
                next[i][j] = (aliveNeighbors == 3);
            }
        }
    }
}

void renderGrid(sf::RenderWindow& window, const Grid& grid) {
    window.clear();
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
            cell.setPosition(i * cellSize, j * cellSize);
            cell.setFillColor(grid[i][j] ? sf::Color::White : sf::Color::Black);
            window.draw(cell);
        }
    }
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Conway's Game of Life");

    Grid current(gridWidth, std::vector<bool>(gridHeight, false));
    Grid next = current;

    initializeGrid(current);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateGrid(current, next);
        current = next;

        renderGrid(window, current);

        sf::sleep(sf::milliseconds(200)); // Adjust the speed of the simulation, 100ms is recommended
    }

    return 0;
}
