#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <vector>
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

class Bullet : public Sprite
{
private:
	SDL_Rect m_rect;
public:
	Bullet(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Bullet \n";
		this->m_rect.x = spawnLoc.x;
		this->m_rect.y = spawnLoc.y;
		this->m_rect.w = 5;
		this->m_rect.h = 5;

	}
	~Bullet() // Destructor
	{
		cout << "De-Allocating Bullet at " << &(*this) << endl;
	}
	void SetLoc(SDL_Point newloc)
	{
		m_rect.x = newloc.x;
		m_rect.y = newloc.y;
	}
	void Update()
	{
		this->m_rect.x += 6;
	}
	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &m_rect);

	}
	SDL_Rect* GetRekt() { return &m_rect; }
};
class Enemy 
{	
private:
	SDL_Rect m_enerect;
public: 
	Enemy(SDL_Point spawnLoc = { 1024, 384})
	{
		cout << "CONSTRUCTING ENEMY \n";
		this->m_enerect.x = spawnLoc.x;
		this->m_enerect.y = spawnLoc.y;
		this->m_enerect.w = 100;
		this->m_enerect.h = 100;


	}
	void SetLoc(SDL_Point loc)
	{

	}
	void Update()
	{
		this->m_enerect.x -= 10;
	}
	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &m_enerect);
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
	Sprite m_player, m_bg1, m_bg2, m_enemy, m_fireball;
	int m_speed = 5; // In-class initialization. Not normal.
	int m_dstWidth = 432, m_dstHeight = 512;
	int m_srcWidth = m_dstWidth / 3, m_srcHeight = m_dstHeight / 4, m_srcy = 0;
	int m_time = 0;
	vector<Bullet*> m_bullet;
	Enemy* EnemyOne;

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
