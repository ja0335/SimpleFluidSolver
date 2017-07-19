#include "Particle.h"
#include <iostream>

void Particle::SetParticle(Vector2f InTopLeft, Vector2f InBottommRight, Uint32 i, Uint32 j, Real * InU, Real * InV)
{
	ParticleRadius = 10;
	TopLeft = InTopLeft;
	BottommRight = InBottommRight;
	CavitySize = Vector2f(BottommRight.x - TopLeft.x, BottommRight.y - TopLeft.y);
	Cavity_i = i;
	Cavity_j = j;
	u = InU;
	v = InV;
	Circle = CircleShape(ParticleRadius, 4);
	Circle.setFillColor(sf::Color(100, 250, 50));

	SetPosition(Cavity_i, Cavity_j);
}

void Particle::Update(RenderWindow &Window)
{
	Real dt = 0.5f;
	Real x = CavitySize.x * u[IJ(Cavity_i, Cavity_j)];
	Real y = -CavitySize.y * v[IJ(Cavity_i, Cavity_j)];
	Real size = sqrtf(x*x + y* y);
	//x /= size;
	//y /= size;
	x *= dt;
	y *= dt;

	Position.x = TopLeft.x + (Cavity_i * CavitySize.x) / GRID_SIZE;
	Position.y = TopLeft.y + (Cavity_j * CavitySize.y) / GRID_SIZE;
	Position.y = (BottommRight.y - Position.y) + TopLeft.y;

	Vector2f EndPosition = Position + Vector2f(x, y);

	sf::Vertex line[] =
	{
		sf::Vertex(Position, Color::Black),
		sf::Vertex(EndPosition, Color::Black)
	};
	Window.draw(line, 2, sf::Lines);

	size = 0.2f;
	line[0] = sf::Vertex(EndPosition, Color::Black);
	line[1] = sf::Vertex(EndPosition + Vector2f(size*(-x + y), size*(-x - y)), Color::Black);
	Window.draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(EndPosition, Color::Black);
	line[1] = sf::Vertex(EndPosition + Vector2f(size*(-x - y), size*(x - y)), Color::Black);
	Window.draw(line, 2, sf::Lines);


	//SetPosition(
	//	((EndPosition.x - TopLeft.x) * GRID_SIZE) / CavitySize.x,
	//	((EndPosition.y - TopLeft.y) * GRID_SIZE) / CavitySize.y
	//	);

	//Circle.setPosition(Position);
	//Window.draw(Circle);
}


void Particle::SetPosition(Uint32 i, Uint32 j)
{
	Cavity_i = i;
	Cavity_j = j;
}