#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Sprite
{
public:
	SDL_Rect m_src; // Source rectangle.
	SDL_Rect m_dst; // Destination rectangle.
	SDL_Rect m_playercoDst;

	void SetRekts(const SDL_Rect s, const SDL_Rect d) //non default construcotr
	{
		m_src = s;
		m_dst = d;
	}
	SDL_Rect* GetSrc() { return &m_src; }
	SDL_Rect* GetDst() { return &m_dst; }
	SDL_Rect* GetPlayercoDst() { return &m_playercoDst;}

};

class Bullet 
{
private:
	
		SDL_Rect m_pFireDst;
	
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
	//void SetLoc(SDL_Point newloc)
	//{
	//	m_pFireDst.x = newloc.x;
	//	m_pFireDst.y = newloc.y;
	//}
	void Update()
	{
		this->m_pFireDst.x += 6;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_pFireDst);
	}
	SDL_Rect* GetRekt() { return &m_pFireDst; }
};

class EnemyBullet
{
private:

	SDL_Rect m_pWaterDst;
	Mix_Chunk* waterball = nullptr;
public:


	EnemyBullet(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Enemy Bullet\n";
		this->m_pWaterDst.x = spawnLoc.x;
		this->m_pWaterDst.y = spawnLoc.y;
		this->m_pWaterDst.w = 8;
		this->m_pWaterDst.h = 8;

	}
	~EnemyBullet() // Destructor
	{
		cout << "De-Allocating Enemy Bullet at " << &(*this) << endl;

	}
	void UpdateEnemyB()
	{
		this->m_pWaterDst.x -= 5;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_pWaterDst);
	}

	SDL_Rect* GetRekt() { return &m_pWaterDst; }

};

class Enemy
{
private:

	SDL_Rect m_pEnemyDst;
	SDL_Rect m_pEnemycoDst;
public:

	Enemy(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Enemy \n";
		this->m_pEnemyDst.x = spawnLoc.x;
		this->m_pEnemyDst.y = spawnLoc.y;
		this->m_pEnemyDst.w = 86;
		this->m_pEnemyDst.h = 102;

		this->m_pEnemycoDst.x = spawnLoc.x + 15;
		this->m_pEnemycoDst.y = spawnLoc.y + 10;
		this->m_pEnemycoDst.w = 86 - 15;
		this->m_pEnemycoDst.h = 102 - 10;

	}
	~Enemy() // Destructor
	{
		cout << "De-Allocating Enemy at " << &(*this) << endl;

	}
	void UpdateEnemy()
	{
		this->m_pEnemyDst.x -= 3;
		this->m_pEnemycoDst.x -= 3;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_pEnemyDst);
	}
	SDL_Rect* GetRekt() { return &m_pEnemyDst; }
	SDL_Rect* GetEnemycoRekt()
	{
		return &m_pEnemycoDst;
	}
};

class ObstaclesTree
{
private:
	SDL_Rect m_treeDst;
	SDL_Rect m_treeRect;
public:

	ObstaclesTree(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Tree \n";
		this->m_treeDst.x = spawnLoc.x;
		this->m_treeDst.y = 768 - 122 * 3 + 50;
		this->m_treeDst.w = 104*3;
		this->m_treeDst.h = 122*3;

		this->m_treeRect.x = spawnLoc.x;
		this->m_treeRect.y = 768 - 122 * 2 + 50;
		this->m_treeRect.w = 104 * 3;
		this->m_treeRect.h = 122 * 3;

	}
	~ObstaclesTree() // Destructor
	{
		cout << "De-Allocating Tree at " << &(*this) << endl;

	}
	void UpdateTree()
	{
		this->m_treeDst.x -= 5/2;
		this->m_treeRect.x -= 5 / 2;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_treeDst);
	}
	SDL_Rect* GetRekt() 
	{
		return &m_treeDst; 
	}
	SDL_Rect* GetTreecoRekt()
	{
		return &m_treeRect;
	}
};
class ObstaclesBird1
{
private:
	SDL_Rect m_bird1Dst;
	SDL_Rect m_bird1coDst;
public:

	ObstaclesBird1(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Bird1 \n";
		this->m_bird1Dst.x = spawnLoc.x;
		this->m_bird1Dst.y = 0;
		this->m_bird1Dst.w = 95/3*2;
		this->m_bird1Dst.h = 96*2;

		this->m_bird1coDst.x = spawnLoc.x;
		this->m_bird1coDst.y = 0;
		this->m_bird1coDst.w = 95 / 3 * 2;
		this->m_bird1coDst.h = 96 * 2 - 10;
	}
	~ObstaclesBird1() // Destructor
	{
		cout << "De-Allocating bird1 at " << &(*this) << endl;

	}
	void UpdateBird1()
	{
		this->m_bird1Dst.x -= 4;
		this->m_bird1coDst.x -= 4;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_bird1Dst);
	}
	SDL_Rect* GetRekt() { return &m_bird1Dst; }
	SDL_Rect* GetBird1coRekt() { return &m_bird1coDst; }
};
class ObstaclesBird2
{
private:
	SDL_Rect m_bird2Dst;
	SDL_Rect m_bird2coDst;
public:

	ObstaclesBird2(SDL_Point spawnLoc = { 1024, 384 })
	{
		cout << "CONSTRUCTING Bird2 \n";
		this->m_bird2Dst.x = spawnLoc.x;
		this->m_bird2Dst.y = spawnLoc.y;
		this->m_bird2Dst.w = 96/3*2;
		this->m_bird2Dst.h = 32*2;

		this->m_bird2coDst.x = spawnLoc.x + 16;
		this->m_bird2coDst.y = spawnLoc.y + 16;
		this->m_bird2coDst.w = 96 / 3 * 2 - 16;
		this->m_bird2coDst.h = 32 * 2 - 16;

	}
	~ObstaclesBird2() // Destructor
	{
		cout << "De-Allocating bird2 at " << &(*this) << endl;

	}
	void UpdateBird2()
	{
		this->m_bird2Dst.x -= 3;
		this->m_bird2coDst.x -= 3;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* text, const SDL_Rect* src)
	{
		SDL_RenderCopy(rend, text, src, &m_bird2Dst);
	}
	SDL_Rect* GetRekt() { return &m_bird2Dst; }
	SDL_Rect* GetBird2coRekt() { return &m_bird2coDst; }
};
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
	SDL_Texture* m_explodeTexture;
	SDL_Texture* m_pTreeTexture;
	SDL_Texture* m_bird1Texture;
	SDL_Texture* m_bird2Texture;
	Mix_Music* m_bgm = nullptr;
	Mix_Chunk* shoot = nullptr;
	Mix_Chunk* waterball = nullptr;
	Mix_Chunk* explosion = nullptr;
	Sprite m_player, m_bg1, m_bg2, m_enemy, m_fireball, m_waterball, m_pExplode, m_eExplode, m_tree, m_bird1, m_bird2;
	int m_speed = 5; // In-class initialization. Not normal.
	int m_dstWidth = 432, m_dstHeight = 512;
	int m_srcWidth = m_dstWidth / 3, m_srcHeight = m_dstHeight / 4, m_srcy = 0;
	int m_time = 0, m_timerE = 0, m_timerB = 0, m_timerPE = 0, m_timerEE = 0, m_timerTree = 0, m_timerBird1 = 0, m_timerBird2 = 0;
	bool m_playerOn = true, m_pExploOn = false, m_eExploOn = false;
	int m_explodeSrcW = 1226 / 11, m_explodeSrcH = 440 / 4;

	vector<Bullet*> m_bullet;
	vector<EnemyBullet*> m_enemyBullet;
	vector<Enemy*> m_enemyNumber;
	vector<ObstaclesTree*> m_treeNumber;
	vector<ObstaclesBird1*> m_bird1Number;
	vector<ObstaclesBird2*> m_bird2Number;

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
