#include "Screen.h"
#include <cmath>

Screen::Screen() {

}
Screen::~Screen() {

}

bool Screen::init()
{
	isRunning = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("FPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	isRunning = true;

	setCorners();
	setConnections();
	calcCentroid();


    return isRunning;
}

void Screen::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}

}

void Screen::setCorners()
{

	corners.emplace_back(Vec3({ 400.0f, 400.0f, 400.0f }));	//FTL
	corners.emplace_back(Vec3({ 500.0f, 400.0f, 400.0f }));	//FTR

	corners.emplace_back(Vec3({ 500.0f, 500.0f, 400.0f }));	//FBR
	corners.emplace_back(Vec3({ 400.0f, 500.0f, 400.0f }));	//FBL

	corners.emplace_back(Vec3({ 400.0f, 400.0f, 500.0f }));	//BTL
	corners.emplace_back(Vec3({ 500.0f, 400.0f, 500.0f }));	//BTR

	corners.emplace_back(Vec3({ 500.0f, 500.0f, 500.0f }));	//BBR
	corners.emplace_back(Vec3({ 400.0f, 500.0f, 500.0f }));	//BBL
	

}

void Screen::setConnections()
{
	//Front Face
	connections.emplace_back(connection{ 0, 1 });
	connections.emplace_back(connection{ 1, 2 });
	connections.emplace_back(connection{ 2, 3 });
	connections.emplace_back(connection{ 3, 0 });

	//Rear Face
	connections.emplace_back(connection{ 4, 5 });
	connections.emplace_back(connection{ 5, 6 });
	connections.emplace_back(connection{ 6, 7 });
	connections.emplace_back(connection{ 7, 4 });

	//Corner Connections
	connections.emplace_back(connection{ 0, 4 });
	connections.emplace_back(connection{ 1, 5 });
	connections.emplace_back(connection{ 2, 6 });
	connections.emplace_back(connection{ 3, 7 });
	


}

void Screen::wireFrame()
{

	//line(corners.at(0).x, corners.at(0).y, corners.at(1).x, corners.at(1).y);


	//line(corners.at(0).x, corners.at(0).y, corners.at(4).x, corners.at(4).y);
	//line(corners.at(1).x, corners.at(1).y, corners.at(5).x, corners.at(5).y);
	//line(corners.at(2).x, corners.at(2).y, corners.at(6).x, corners.at(6).y);
	//line(corners.at(3).x, corners.at(7).y, corners.at(7).x, corners.at(7).y);
}

void Screen::frameCountInc()
{
	frameCount++;
}

void Screen::frameCountReset()
{
	frameCount = 0;

}

void Screen::update()
{
		//for (int i = 0; i < 100; i++)
		//{
		//	pixel(rand() % 640, rand() % 480);

		//}

		//line(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


		pixel(centroid.x, centroid.y);

		for (auto& p : corners)
		{
			p.x -= centroid.x;
			p.y -= centroid.y;
			p.z -= centroid.z;

			rotateAll(p, 0.1, 0.1, 0.1);

			//rotateX(p, 0.1, 0.0, 0.0);
			//rotateY(p, 0.0, 0.1, 0.0);
			//rotateZ(p, 0.0, 0.0, 0.1);

			p.x += centroid.x;
			p.y += centroid.y;
			p.z += centroid.z;


		////line(corners.at(0).x, corners.at(0).y, corners.at(1).x, corners.at(1).y);

		}
	for (auto& conn : connections)
	{
		line(corners[conn.a].x, corners[conn.a].y, corners[conn.b].x, corners[conn.b].y);
		//Bresline(corners[conn.a].x, corners[conn.a].y, corners[conn.b].x, corners[conn.b].y);
	}
	
}

void Screen::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (auto& point : points)
	{

		SDL_RenderDrawPointF(renderer, point.x, point.y);

	}

	SDL_RenderPresent(renderer);
}

void Screen::clearPoints()
{

	points.clear();

}

Uint32 Screen::getFrameCount()
{
	return frameCount;
}

void Screen::quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void Screen::pixel(float x, float y)
{
	SDL_FPoint point;
	point.x = x;
	point.y = y;
	points.emplace_back(point);

}

void Screen::line(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	float length = std::sqrt(dx * dx + dy * dy);

	float theta = std::atan2(dy, dx);

	for (float i = 0; i < length; i++)
	{
		pixel(x1 + std::cos(theta) * i, y1 + std::sin(theta) * i);
	
	}

}

void Screen::Bresline(float x1, float y1, float x2, float y2) 
{
	int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = dx + dy, e2;

	while (true) 
	{
		pixel(x1, y1);  // Plot the point
		 
		e2 = 2 * err;
		if (e2 >= dy) 
		{ 
			if ((int)x1 == (int)x2) break;
			err += dy; 
			x1 += sx; 
		}
		if (e2 <= dx) 
		{ 
			if ((int)y1 == (int)y2) break;
			err += dx; 
			y1 += sy; 
		}

	}
}



void Screen::calcCentroid()
{
	
	for (auto& c : corners)
	{
		centroid.x += c.x;
		centroid.y += c.y;
		centroid.z += c.z;
	}

	centroid.x /= corners.size();
	centroid.y /= corners.size();
	centroid.z /= corners.size();

}



void Screen::rotateAll(Vec3& point, float x, float y, float z)
{
	float rad = 0.3f;

	Vec3 OriginalPoint = point;


	//Confirmed with Wikipedia version
	rad = x;
	point.y = std::cos(rad) * OriginalPoint.y - std::sin(rad) * OriginalPoint.z;
	point.z = std::sin(rad) * OriginalPoint.y + std::cos(rad) * OriginalPoint.z;
	OriginalPoint = point;

	rad = y;
	point.x = std::cos(rad) * OriginalPoint.x - std::sin(rad) * OriginalPoint.z;
	point.z = std::sin(rad) * OriginalPoint.x + std::cos(rad) * OriginalPoint.z;
	OriginalPoint = point;

	rad = z;
	point.x = std::cos(rad) * OriginalPoint.x + std::sin(rad) * OriginalPoint.y;
	point.y = -std::sin(rad) * OriginalPoint.x + std::cos(rad) * OriginalPoint.y;
	OriginalPoint = point;

}

void Screen::rotateX(Vec3& point, float x, float y, float z)
{
	float rad = 0.0f;

	rad = x;
	point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
	point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

	//rad = y;
	//point.x = std::cos(rad) * point.x - std::sin(rad) * point.z;
	//point.z = std::sin(rad) * point.x + std::cos(rad) * point.z;

	//rad = z;
	//point.x = std::cos(rad) * point.x + std::sin(rad) * point.y;
	//point.y = -std::sin(rad) * point.x + std::cos(rad) * point.y;

}

void Screen::rotateY(Vec3& point, float x, float y, float z)
{
	float rad = 0.0f;

	//rad = x;
	//point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
	//point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

	rad = y;
	point.x = std::cos(rad) * point.x - std::sin(rad) * point.z;
	point.z = std::sin(rad) * point.x + std::cos(rad) * point.z;

	//rad = z;
	//point.x = std::cos(rad) * point.x + std::sin(rad) * point.y;
	//point.y = -std::sin(rad) * point.x + std::cos(rad) * point.y;

}

void Screen::rotateZ(Vec3& point, float x, float y, float z)
{
	float rad = 0.0f;

	//rad = x;
	//point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
	//point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

	//rad = y;
	//point.x = std::cos(rad) * point.x - std::sin(rad) * point.z;
	//point.z = std::sin(rad) * point.x + std::cos(rad) * point.z;

	rad = z;
	point.x = std::cos(rad) * point.x + std::sin(rad) * point.y;
	point.y = -std::sin(rad) * point.x + std::cos(rad) * point.y;

}



void Screen::setTitle(const std::string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}
