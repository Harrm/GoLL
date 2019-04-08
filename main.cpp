#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "logic.h"

template <typename T>
using Matrix = std::array<std::array<T, Logic::kFieldSize>, Logic::kFieldSize>;

class Image: public sf::Drawable {
public:
  static std::optional<Image> loadFrom(const std::string& path) {
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path)) {
      return std::nullopt;
    }

    auto sprite = std::make_unique<sf::Sprite>();
    return Image {std::move(texture), std::move(sprite)};
  }

  void draw (sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(*sprite, states);
  }

private:

  explicit Image(std::unique_ptr<sf::Texture> t, std::unique_ptr<sf::Sprite> s):
    texture {std::move(t)},
    sprite {std::move(s)} {
    sprite->setTexture(*texture);
  }

  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;

};



void genetic(Logic& l, const Matrix<bool>& substrate) {
  l.updateField(substrate);

  std::uniform_int_distribution<uint32_t> dist(0, 100);
  std::default_random_engine rand;

  for(int i = 0; i < Logic::kFieldSize; i++) {
      for(int c = 0; c < Logic::kFieldSize; c++) {
        if(dist(rand) < 1) {
          l.killCell(c, i);
        } else if (dist(rand) > 98) {
          l.reviveCell(c, i);
        }
      }
    }
}



int main(int argc, char** argv) {

  if(argc != 2) {
    std::cerr << "Please specify input image";
    return -1;
  }

  sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");

  auto i = Image::loadFrom(argv[1]).value();

  sf::Shader shader;
  if(!shader.loadFromFile("../data/shaders/whiteblack.glsl", sf::Shader::Fragment)) {
    return -1;
  }

  sf::RenderTexture blackWhite;
  if (!blackWhite.create(512, 512))
    return -1;
  blackWhite.clear();
  shader.setUniform("texture", sf::Shader::CurrentTexture);
  blackWhite.draw(i, &shader);
  blackWhite.display();

  sf::Image substrateImage = blackWhite.getTexture().copyToImage();
  std::array<std::array<bool, Logic::kFieldSize>, Logic::kFieldSize> substrate;
  for(int i = 0; i < Logic::kFieldSize; i++) {
    for(int c = 0; c < Logic::kFieldSize; c++) {
      substrate[c][i] = substrateImage.getPixel(i, c) == sf::Color::Black;
    }
  }

  Logic l;
  l.randomizeField();

  for(int i = 0; i < Logic::kFieldSize * Logic::kFieldSize; i++) {
    if(i % Logic::kFieldSize == 0) {
      std::cout << "\n";
    }
  }

  sf::Texture cellTex;
  if(!cellTex.loadFromFile("../data/life_cell2x2.png")) {
    return -1;
  }
  std::vector<sf::Sprite> cells;
  cells.reserve(Logic::kFieldSize*Logic::kFieldSize);
  for(int i = 0; i < Logic::kFieldSize; i++) {
    for(int c = 0; c < Logic::kFieldSize; c++) {
      cells.emplace_back(cellTex);
      cells.back().scale(0.5, 0.5);
      cells.back().setPosition(cells.back().getTextureRect().width * c / 2, cells.back().getTextureRect().height * i / 2);
    }
  }

  sf::Clock clock; // starts the clock
  sf::Time elapsed;

  int iterations = 100;
  while (window.isOpen() && iterations > 0) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    elapsed += clock.getElapsedTime();
    clock.restart();

    if(elapsed > sf::seconds(.03f)) {
      elapsed = sf::Time::Zero;
      genetic(l, substrate);
      iterations--;
    }

    window.clear();

    sf::Sprite sprite(blackWhite.getTexture());
    //window.draw(sprite);

    for(int i = 0; i < Logic::kFieldSize * Logic::kFieldSize; i++) {
      if(l.getField()[i / Logic::kFieldSize][i % Logic::kFieldSize] == Logic::State::Alive)
        window.draw(cells[i]);
    }

    window.display();
  }
  return 0;
}
