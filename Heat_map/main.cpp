#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace sf;

sf::Color get_color(sf::Color from, sf::Color to, double coef, int transparency)
{
	unsigned int r = static_cast<unsigned int>(round((double)from.r + (coef / 100) * (to.r - from.r)));
	unsigned int g = static_cast<unsigned int>(round((double)from.g + (coef / 100) * (to.g - from.g)));
	unsigned int b = static_cast<unsigned int>(round((double)from.b + (coef / 100) * (to.b - from.b)));
	return sf::Color(r, g, b, transparency);
}
sf::Color get_color(sf::Color from, sf::Color to, sf::Color between, double coef, int transparency)
{
	unsigned r,g,b;
	if (coef <= 50.0)
	{
		r = static_cast<unsigned int>(round((double)from.r + (coef / 50) * (between.r - from.r)));
		g = static_cast<unsigned int>(round((double)from.g + (coef / 50) * (between.g - from.g)));
		b = static_cast<unsigned int>(round((double)from.b + (coef / 50) * (between.b - from.b)));
	}
	else
	{
		r = static_cast<unsigned int>(round((double)between.r + ((coef - 50) / 50) * (to.r - between.r)));
		g = static_cast<unsigned int>(round((double)between.g + ((coef - 50) / 50) * (to.g - between.g)));
		b = static_cast<unsigned int>(round((double)between.b + ((coef - 50) / 50) * (to.b - between.b)));
	}
	return sf::Color(r, g, b, transparency);
}

int main()
{
	//load coefs from file; 

	std::fstream in("coefs.txt");
	std::vector<int> coefs;

	int num;
	while (in >> num)
		coefs.push_back(num);

	int images_quantity = coefs.size();


	//global parametres
	sf::Color from = sf::Color::Green;
	sf::Color between = sf::Color::Yellow;
	sf::Color to = sf::Color::Red;
	int transparency = 255; // 0 - 255
	double scale = 85;


	//load images of map
	
	std::vector<sf::Sprite> sprites;
	std::vector<sf::Texture> textures;

	for (int i = 1; i <= images_quantity; ++i)
	{
		sf::Texture texture;
		std::string path = "images/image" + std::to_string(i) + ".png";
		if (!texture.loadFromFile(path)) std::cout << "fail to load image from images/image" + std::to_string(i) + ".png" << std::endl;
		textures.push_back(texture);
	}
	for (int i = 0; i < textures.size(); ++i)
	{
		sf::Sprite sprite;
		sprite.setTexture(textures[i]);
		sprite.setColor(get_color(from, to, between, coefs[i], transparency));
		sprite.setScale(sf::Vector2f((float)(scale / 100), (float)(scale / 100)));
		sprites.push_back(sprite);
	}

	sf::Image map_background_image;
	if (!map_background_image.loadFromFile("images/map_background.png"))
		std::cout << "fail to load map_background from images/map_background.png" << std::endl;
	sf::Texture texture_of_background;
	texture_of_background.loadFromImage(map_background_image);

	sf::Sprite sprite_background;
	sprite_background.setTexture(texture_of_background);
	sprite_background.setScale(sf::Vector2f((float)(scale / 100), (float)(scale / 100)));
	int window_width = static_cast<int>(round(map_background_image.getSize().x * (scale / 100)));
	int window_height = static_cast<int>(round(map_background_image.getSize().y * (scale / 100)));
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Heat map");

	//**********
	// main loop
	//**********

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		window.draw(sprite_background);
		
		for (auto sprite : sprites)
			window.draw(sprite);
		window.display();
	}
	return 0;
}