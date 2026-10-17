#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

// constants

// system core
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

struct Vec3
{

	float x, y, z;

};

struct connection
{
	int a, b;
};

class Screen {
public:
	Screen();
	~Screen();

	bool init();
	void quit();
	void pixel(float x, float y);
	void line(float x1, float y1, float x2, float y2);
	void Bresline(float x1, float y1, float x2, float y2);

	void setCorners();
	void setConnections();
	void calcCentroid();
	void wireFrame();
	void frameCountInc();
	void frameCountReset();


	void rotateAll(Vec3& point, float x = 1.0f, float y = 1.0f, float z = 1.0f);
	void rotateX(Vec3& point, float x = 1.0f, float y = 1.0f, float z = 1.0f);
	void rotateY(Vec3& point, float x = 1.0f, float y = 1.0f, float z = 1.0f);
	void rotateZ(Vec3& point, float x = 1.0f, float y = 1.0f, float z = 1.0f);

	void handleEvents();
	void update();
	void render();
	void clearPoints();
	Uint32 getFrameCount();

	

	bool running() {
		return isRunning;
	}
private:
	bool isRunning = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<SDL_FPoint> points;
	std::vector<Vec3> corners;
	std::vector<connection> connections;
	Vec3 centroid {0.0f, 0.0f, 0.0f};

	float fps = 0.0f;
	Uint32 frameCount = 0;

};

