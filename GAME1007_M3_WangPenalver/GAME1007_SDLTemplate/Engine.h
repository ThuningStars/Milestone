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

class Bullet 
{
private:
	
	SDL_Rect m_pFireDst;	
	SDL_Rect m_pWaterDst;
	
public:

	Bullet(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Bullet \n";
		this->m_pFireDst.x = spawnLoc.x;
		this->m_pFireDst.y = spawnLoc.y;
		this->m_pFireDst.w = 14;
		this->m_pFireDst.h = 7;

	}
	~Bullet() // Destructor
	{
		cout << "De-Allocating Bullet at " << &(*this) << endl;
		
	}
	void SetWaterDst(SDL_Point newloc)
	{
		m_pWaterDst.w = 7;
		m_pWaterDst.h = 7;
	}
	void SetLoc(SDL_Point newloc)
	{
		m_pFireDst.x = newloc.x;
		m_pFireDst.y = newloc.y;
	}
	void Update()
	{
		this->m_pFireDst.x += 6;
	}
	void UpdateEnemyB()
	{
		this->m_pWaterDst.x -= 6;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_pFireDst);
	}
	SDL_Rect* GetRekt() { return &m_pFireDst; }
};
//class Enemy : public Sprite
//{	
//private:
//	//SDL_Rect m_dest = { 0,0,30,30};
//
//public: 
//	
//	void SetLoc(SDL_Point newloc)
//	{
//		m_dst.x = newloc.x;
//		m_dst.y = newloc.y;
//	}
//	void Update()
//	{
//		m_dst.x -= 10;
//		
//	}
//	
//	SDL_Rect* GetRekt() { return &m_dst; }
//
//};
class Engine
{
private: // private properties.
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pEnemyTexture;
	SDL_Texture* m_pTexture;
	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pFireballTexture;
	SDL_Texture* m_pWaterballTexture;
	Sprite m_player, m_bg1, m_bg2, m_enemy, m_fireball, m_waterball;
	int m_speed = 5; // In-class initialization. Not normal.
	int m_dstWidth = 432, m_dstHeight = 512;
	int m_srcWidth = m_dstWidth / 3, m_srcHeight = m_dstHeight / 4, m_srcy = 0;
	int m_time = 0;

	vector<Bullet*> m_bullet;
	vector<Bullet*> m_enemyBullet;

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
