#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Macros.h"
#include "Particle.h"

using namespace sf;
using namespace std;


int main(int argc, char **argv)
{

	RenderWindow window(VideoMode(PLOT_RESOLUTION, PLOT_RESOLUTION), "Simple fluid solver");
	RenderTexture Canvas;
	Texture DynamicTexture;
	Sprite SpriteDynamicTexture;
	Image DyeImage;

	if (!Canvas.create(PLOT_RESOLUTION, PLOT_RESOLUTION))
		return EXIT_FAILURE;
	if (!DynamicTexture.create(GRID_SIZE, GRID_SIZE))
		return EXIT_FAILURE;


	SpriteDynamicTexture.setTexture(DynamicTexture);
	DynamicTexture.setSmooth(true);
	float SpriteScale = static_cast<float>(Canvas.getSize().x) / static_cast<float>(PLOT_RESOLUTION);
	SpriteDynamicTexture.scale(SpriteScale, SpriteScale);
	Uint64 PixelsBufferSize = PLOT_RESOLUTION * PLOT_RESOLUTION * 4;

	Uint8* Density = new Uint8[PixelsBufferSize];	memset(Density, 0, PixelsBufferSize * sizeof(Uint8));
	Real* u = new Real[PLOT_RESOLUTION * PLOT_RESOLUTION];	memset(u, 0, PLOT_RESOLUTION * PLOT_RESOLUTION * sizeof(Real));
	Real* v = new Real[PLOT_RESOLUTION * PLOT_RESOLUTION];	memset(v, 0, PLOT_RESOLUTION * PLOT_RESOLUTION * sizeof(Real));

	Sprite FinalSprite;
	Real Scale = window.getSize().y / (PLOT_RESOLUTION*1.0f);
	FinalSprite.scale(Scale, Scale);
	
	cout << "Loading density..." << endl;
	if (!DyeImage.loadFromFile("density.png"))
	{
		cout << "Can not load density.png" << endl;
		return EXIT_FAILURE;
	}
	else
	{
		memcpy(Density, DyeImage.getPixelsPtr(), PixelsBufferSize * sizeof(Uint8));
	}

	cout << "Start reading Velocity Field..." << endl;
	string line;
	ifstream u_file("u.csv");
	if (u_file.is_open())
	{
		Uint32 Index = 0;
		while (getline(u_file, line))
		{
			stringstream ss(line);
			string token;

			while (getline(ss, token, ';'))
			{
				istringstream value(token);
				value >> u[Index++];
			}
		}
		u_file.close();
	}
	ifstream v_file("v.csv");
	if (v_file.is_open())
	{
		Uint32 Index = 0;
		while (getline(v_file, line))
		{
			stringstream ss(line);
			string token;

			while (getline(ss, token, ';'))
			{
				istringstream value(token);
				value >> v[Index++];
			}
		}
		v_file.close();
	}
	
	cout << "End reading Velocity Field..." << endl;


	cout << "Start setting particles..." << endl;
	Particle* Particles = new Particle[NUM_PARTICLES];

	srand(static_cast <unsigned> (time(0)));

	for (Uint32 i = 0; i < NUM_PARTICLES; i++)
	{
		float r_i = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
		float r_j = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);

		Particles[i].SetParticle(
			Vector2f(0, 0),
			Vector2f(window.getSize().x, window.getSize().y),
			GRID_SIZE * r_i, GRID_SIZE * r_j,
			u, v);
	}

	cout << "End setting particles..." << endl;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
			{
				window.close();
			}
			
			Canvas.clear(Color::Black);

			DynamicTexture.update(Density);
			Canvas.draw(SpriteDynamicTexture);
			Canvas.display();
			
			// --------------------------------------------------------
			// Draw the final image result
			window.clear(Color::Black);
			FinalSprite.setTexture(Canvas.getTexture());
			window.draw(FinalSprite);
			
			for (Uint32 i = 0; i < NUM_PARTICLES; i++)
				Particles[i].Update(window);

			// end the current frame
			window.display();
		}
	}

	delete[] u;
	delete[] v;
	delete[] Particles;

	return 0;
}