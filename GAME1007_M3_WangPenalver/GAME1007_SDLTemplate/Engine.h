#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Sprite
{
public:
	SDL_Rect m_src; // Source rectangle.
	SDL_Rect m_dst; // Destination rectangle.
};

class Bullet
{
private:
	SDL_Rect m_dst = { 0,0,16,16 };
public:
	void SetLoc(SDL_Point newloc)
	{
		m_dst.x = newloc.x;
		m_dst.y = newloc.y;
	}
	SDL_Rect* GetRect() { return &m_dst; }
	void Update()
	{
		m_dst.y -= 2; // Move the bullet "up" 2 pixels every frame.
	}
};
class Engine
{
private: // private properties.
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	SDL_Texture* m_pTexture;
	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pFireballTexture;
	Sprite m_player, m_bg1, m_bg2;
	int m_speed = 5; // In-class initialization. Not normal.
	int m_dstWidth = 432, m_dstHeight = 512;
	int m_srcWidth = m_dstWidth / 3, m_srcHeight = m_dstHeight / 4, m_srcy = 0;
	int m_time = 0;
	Bullet m_bullet;

private: // private method prototypes.
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	bool KeyDown(SDL_Scancode c);
	void Update();
	void Render();
	void Sleep();

public: // public method prototypes.
	int Run();
};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
