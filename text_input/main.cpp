#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

int main(){
	sf::RenderWindow window(sf::VideoMode(500,500), "Text reader!");

	std::string s;

	while(window.isOpen()){
		//Event processing.
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::TextEntered){
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !s.empty()) {
					s.pop_back();
					std::cout << s << std::endl;
				}
				else if (event.text.unicode < 128) {
					s.push_back((char)event.text.unicode);
					std::cout << s << std::endl;
				}
			}
		}
	}
}
