#include "Engine.h"
#include <ctime>

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				// Initialize subsystems...
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					m_pTexture = IMG_LoadTexture(m_pRenderer, "img/dragon1.png");
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "img/background4.png");
					m_pFireballTexture = IMG_LoadTexture(m_pRenderer, "img/fireball.png");
					m_pWaterballTexture = IMG_LoadTexture(m_pRenderer, "img/waterball.png");
					m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "img/enemy.png");
					m_explodeTexture = IMG_LoadTexture(m_pRenderer, "img/explosion.png");
					m_pTreeTexture = IMG_LoadTexture(m_pRenderer, "img/tree.png");
					m_bird1Texture = IMG_LoadTexture(m_pRenderer, "img/bird1.png");
					m_bird2Texture = IMG_LoadTexture(m_pRenderer, "img/bird2.png");
					if (Mix_Init(MIX_INIT_MP3) != 0)
					{
						// Configure mixer.
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
						Mix_AllocateChannels(16);
						// Load sounds.
						shoot = Mix_LoadWAV("sound/Fireball.wav");
						if (shoot == nullptr)
							cout << Mix_GetError() << endl;
						waterball = Mix_LoadWAV("sound/waterball.mp3");
						if (waterball == nullptr)
							cout << Mix_GetError() << endl;
						explosion = Mix_LoadWAV("sound/exp.wav");
						if (explosion == nullptr)
							cout << Mix_GetError() << endl;

						m_bgm = Mix_LoadMUS("sound/bgm.mp3");
						if (m_bgm == nullptr)
							cout << Mix_GetError() << endl;
					}
				}
				else return false; // Image init failed.
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	m_player = { {0,m_srcy + m_srcHeight,m_srcWidth,m_srcHeight}, {0,384,m_dstWidth / 5,m_dstHeight / 5},{15,394,m_dstWidth / 5 - 15,m_dstHeight / 5 - 10} }; // First {} is src rectangle, and second {} destination rect
	m_fireball = { {0,0,146,72} };
	m_waterball = { {0,0,219,231}};

	srand(time(NULL));
	m_enemy = { {0,m_srcHeight * 3,m_srcWidth,m_srcHeight} };
	m_bg1 = { {0,0,1365,768},{0,0,1365,768} };
	m_bg2 = { {0,0,1365,768},{1365,0,1365,768} };
	m_eExplode = { {0,0,m_explodeSrcW,m_explodeSrcH} };
	m_pExplode = { {0,0,m_explodeSrcW,m_explodeSrcH} };
	m_pExplode.m_dst.w = m_explodeSrcW / 1.5;
	m_eExplode.m_dst.w = m_explodeSrcW / 1.5;
	m_pExplode.m_dst.h = m_explodeSrcH / 1.5;
	m_eExplode.m_dst.h = m_explodeSrcH / 1.5;
	m_tree = { {0,0,104,122} };
	m_bird1 = { {0,0,95 / 3,96} };
	m_bird2 = { {0,0,96 / 3,32} };

	Mix_PlayMusic(m_bgm, -1); // 0-n for # of loops, or -1 for infinite looping.
	Mix_VolumeMusic(70); // 0-128.

	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		
		case SDL_KEYUP:
			if (event.key.keysym.sym == ' ')// spacebar pressed/held
			{
			//spawn bullet

			 m_bullet.push_back( new Bullet({ m_player.m_dst.x+40, m_player.m_dst.y+40 }) );
			 m_bullet.shrink_to_fit();
			 cout << " New bullet vector capacity " << m_bullet.capacity() << endl;
			 Mix_PlayChannel(-1, shoot, 0);

			}
		
		}
	}
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1)
			return true;
	}
	return false;
}

void Engine::Update()
{
	// Scroll the background
	m_bg1.m_dst.x -= m_speed/2;
	m_bg2.m_dst.x -= m_speed/2;

	// Wrap the backgrounds/
	if (m_bg1.m_dst.x <= -m_bg1.m_dst.w)
	{
		m_bg1.m_dst.x = 0;
		m_bg2.m_dst.x = 1365;
	}
	
	// Animation
	m_time++;

	if (FPS / m_time == 4)
	{
		m_time = 0;
		m_player.m_src.x += m_srcWidth;
		if (m_player.m_src.x >= m_dstWidth)
			m_player.m_src.x = 0;

		m_enemy.m_src.x += m_srcWidth;
		if (m_enemy.m_src.x >= m_dstWidth)
			m_enemy.m_src.x = 0;

		m_bird1.m_src.x += 95/3;
		if (m_bird1.m_src.x >= 95/3*2)
			m_bird1.m_src.x = 0;

		m_bird2.m_src.x += 96 / 3;
		if (m_bird2.m_src.x >= 96 / 3 * 2)
			m_bird2.m_src.x = 0;

	}

	// Player Explosion Animation
	m_timerPE++;

	if (m_pExploOn == true && m_timerPE >= 2)
	{
		// Mix_PlayChannel(-1, explosion, 1);
		m_pExplode.m_src.x += m_explodeSrcW;

		if (m_pExplode.m_src.x >= m_explodeSrcW*10)
		{
			m_pExplode.m_src.x = 0;
			m_pExplode.m_src.y += m_explodeSrcH;
		}

		if (m_pExplode.m_src.y >= m_explodeSrcH * 3 && m_pExplode.m_src.x >= m_explodeSrcW * 5)
		{
			m_pExplode.m_src.x = m_pExplode.m_src.y = 0;
			m_pExploOn = false;
		}
		
		m_timerPE = 0;
		SDL_DestroyTexture(m_pTexture);

	}

	// Player Explosion Movement
	if (m_pExploOn == true)
	m_pExplode.m_dst.x -= m_speed / 2;

	// Enemy Explosion Animation
	m_timerEE++;
	if (m_eExploOn == true && m_timerEE >= 2)
	{

		m_eExplode.m_src.x += m_explodeSrcW;
		

		if (m_eExplode.m_src.x >= m_explodeSrcW * 10)
		{
			m_eExplode.m_src.x = 0;
			m_eExplode.m_src.y += m_explodeSrcH;
		}
		if (m_eExplode.m_src.y >= m_explodeSrcH * 3 && m_eExplode.m_src.x >= m_explodeSrcW * 5)
		{
			m_eExplode.m_src.x = m_eExplode.m_src.y = 0;
			m_eExploOn = false;
		}

		
		m_timerEE = 0;
		
	}

	// Enemy Explosion Movement
	if (m_eExploOn == true)
	m_eExplode.m_dst.x -= m_speed / 2;

	// enemy movement
	m_timerE++;
	m_timerB++;
	if (m_timerE == 400)
	{
		m_enemyNumber.push_back(new Enemy({ 1024,(rand() % 640) }));
		m_enemyNumber.shrink_to_fit();
		cout << " New Enemy vector capacity " << m_enemyNumber.capacity() << endl;
		m_timerE = 0;
	}
	for (unsigned i = 0; i < m_enemyNumber.size(); i++) // size() is actual filled numbers of elements
	{
		m_enemyNumber[i]->UpdateEnemy(); 
		
		// bullet
		if (m_timerB >= 55)
		{
			
			m_enemyBullet.push_back(new EnemyBullet({ m_enemyNumber[i]->GetRekt()->x,m_enemyNumber[i]->GetRekt()->y + 51 }));
			m_enemyBullet.shrink_to_fit();
			Mix_PlayChannel(-1, waterball, 0);
			cout << " New Enemy Bullet vector capacity " << m_enemyBullet.capacity() << endl;
			m_timerB = 0;
			
		}
	}

	// Enemy delete
	for (unsigned i = 0; i < m_enemyNumber.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_enemyNumber[i]->GetRekt()->x < -100)
		{
			 //m_enemy.m_dst.x = 1024;
			delete m_enemyNumber[i]; // flag for reallocation
			m_enemyNumber[i] = nullptr; // get rid of the dangling pointer
			m_enemyNumber.erase(m_enemyNumber.begin() + i);
			m_enemyNumber.shrink_to_fit();
			cout << " Enemy Deleted \n";
			
		}
	}

	// Tree Obstacles 
	m_timerTree = rand() % 4 + 1;
	if (m_timerE == 200)
	{
		if (m_timerTree > 2)
		{
			m_treeNumber.push_back(new ObstaclesTree({ (rand() % 100 + 1024), 0 }));
			m_treeNumber.shrink_to_fit();
			cout << " New Tree vector capacity " << m_treeNumber.capacity() << endl;
		}
	}
	for (unsigned i = 0; i < m_treeNumber.size(); i++) // size() is actual filled numbers of elements
	{
		m_treeNumber[i]->UpdateTree();
	}
	for (unsigned i = 0; i < m_treeNumber.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_treeNumber[i]->GetRekt()->x < -500)
		{
			//m_enemy.m_dst.x = 1024;
			delete m_treeNumber[i]; // flag for reallocation
			m_treeNumber[i] = nullptr; // get rid of the dangling pointer
			m_treeNumber.erase(m_treeNumber.begin() + i);
			m_treeNumber.shrink_to_fit();
			cout << " Tree Deleted \n";

		}
	}

	// bird1 obstacles
	m_timerBird1 = rand() % 6 + 1;
	if (m_timerE == 200)
	{
		if (m_timerBird1 > 3)
		{
			m_bird1Number.push_back(new ObstaclesBird1({ (rand() % 50 + 1024), 0 }));
			m_bird1Number.shrink_to_fit();
			cout << " New bird1 vector capacity " << m_bird1Number.capacity() << endl;
		}
	}
	for (unsigned i = 0; i < m_bird1Number.size(); i++) // size() is actual filled numbers of elements
	{
		m_bird1Number[i]->UpdateBird1();
	}
	for (unsigned i = 0; i < m_bird1Number.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_bird1Number[i]->GetRekt()->x < -100)
		{
			//m_enemy.m_dst.x = 1024;
			delete m_bird1Number[i]; // flag for reallocation
			m_bird1Number[i] = nullptr; // get rid of the dangling pointer
			m_bird1Number.erase(m_bird1Number.begin() + i);
			m_bird1Number.shrink_to_fit();
			cout << " Bird1 Deleted \n";

		}
	}

	// Bird2 Obstacles
	m_timerBird2 = rand() % 10 + 1;
	if (m_timerE == 200)
	{
		if (m_timerBird2 > 6)
		{
			m_bird2Number.push_back(new ObstaclesBird2({ 1024,(rand() % 384) }));
			m_bird2Number.shrink_to_fit();
			cout << " New bird2 vector capacity " << m_bird2Number.capacity() << endl;
		}
	}
	for (unsigned i = 0; i < m_bird2Number.size(); i++) // size() is actual filled numbers of elements
	{
		m_bird2Number[i]->UpdateBird2();
	}
	for (unsigned i = 0; i < m_bird2Number.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_bird2Number[i]->GetRekt()->x < -100)
		{
			//m_enemy.m_dst.x = 1024;
			delete m_bird2Number[i]; // flag for reallocation
			m_bird2Number[i] = nullptr; // get rid of the dangling pointer
			m_bird2Number.erase(m_bird2Number.begin() + i);
			m_bird2Number.shrink_to_fit();
			cout << " Bird2 Deleted \n";

		}
	}
	// Parse player movement.
	if (KeyDown(SDL_SCANCODE_W) && m_player.m_dst.y > 0)
	{
		m_player.m_dst.y -= m_speed;
		m_player.m_playercoDst.y -= m_speed;
	}
	else if (KeyDown(SDL_SCANCODE_S) && m_player.m_dst.y < HEIGHT - m_player.m_dst.h)
	{
		m_player.m_dst.y += m_speed;
		m_player.m_playercoDst.y += m_speed;
	}
	if (KeyDown(SDL_SCANCODE_A) && m_player.m_dst.x > 0)
	{
		m_player.m_dst.x -= m_speed;
		m_player.m_playercoDst.x -= m_speed;
	}
	else if (KeyDown(SDL_SCANCODE_D) && m_player.m_dst.x < WIDTH / 2 - m_player.m_dst.w)
	{
		m_player.m_dst.x += m_speed;
		m_player.m_playercoDst.x += m_speed;
	}

	// player Bullet
	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
	{
		//m_fireball.m_dst.x = m_player.m_dst.x + 40;

		m_bullet[i]->Update();
		//m_fireball.m_dst.x += 6;
	}//	 combines dereference and member accsess
	// check bullets going off screen


	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
	{
		
		if (m_bullet[i]->GetRekt()->x > WIDTH)
		{
			//m_fireball.m_dst.x = m_player.m_dst.x + 40;
			delete m_bullet[i]; // flag for reallocation
			m_bullet[i] = nullptr; // get rid of the dangling pointer
			m_bullet.erase(m_bullet.begin() + i);
			m_bullet.shrink_to_fit();
			cout << " Bullet Deleted \n";

			break;
		}
	}

	// enemy bullet

	for (unsigned i = 0; i < m_enemyBullet.size(); i++) // size() is actual filled numbers of elements
		m_enemyBullet[i]->UpdateEnemyB();
	for (unsigned i = 0; i < m_enemyBullet.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_enemyBullet[i]->GetRekt()->x < -10)
		{
			delete m_enemyBullet[i]; // flag for reallocation
			m_enemyBullet[i] = nullptr; // get rid of the dangling pointer
			m_enemyBullet.erase(m_enemyBullet.begin() + i);
			m_enemyBullet.shrink_to_fit();
			cout << " Enemy Bullet Deleted \n";
			break;
		}
	}

	// player bullet to enemy collision
	for (unsigned i = 0; i < m_bullet.size(); i++)
	{
		for (unsigned a = 0; a < m_enemyNumber.size(); a++)
		if (SDL_HasIntersection(m_bullet[i]->GetRekt(), m_enemyNumber[a]->GetEnemycoRekt()))
		{
			m_eExplode.m_dst.x = m_enemyNumber[a]->GetRekt()->x;
			m_eExplode.m_dst.y = m_enemyNumber[a]->GetRekt()->y;

			cout << "Player bullet Collision!" << endl;
			Mix_PlayChannel(-1, explosion, 0);
	
			delete m_bullet[i]; // flag for reallocation
			m_bullet[i] = nullptr; // get rid of the dangling pointer
			m_bullet.erase(m_bullet.begin() + i);
			m_bullet.shrink_to_fit();
			cout << " Bullet Deleted \n";
			
			delete m_enemyNumber[a]; // flag for reallocation
			m_enemyNumber[a] = nullptr; // get rid of the dangling pointer
			m_enemyNumber.erase(m_enemyNumber.begin() + a);
			m_enemyNumber.shrink_to_fit();
			cout << " Enemy Deleted \n";
			m_eExploOn = true;
		}
	}
	// enemy bullet to player collision
	for (unsigned i = 0; i < m_enemyBullet.size(); i++)
	{
		if (SDL_HasIntersection(m_enemyBullet[i]->GetRekt(), m_player.GetPlayercoDst()))
		{
		    m_pExplode.m_dst.x = m_player.m_dst.x; 
		    m_pExplode.m_dst.y = m_player.m_dst.y;

			cout << "Enemy bullet Collision!" << endl;
			Mix_PlayChannel(-1, explosion, 0);

			delete m_enemyBullet[i]; // flag for reallocation
			m_enemyBullet[i] = nullptr; // get rid of the dangling pointer
			m_enemyBullet.erase(m_enemyBullet.begin() + i);
			m_enemyBullet.shrink_to_fit();
			cout << " Enemy Bullet Deleted \n";

			cout << "Game Over" << endl;
			m_playerOn = false;
			m_pExploOn = true;
		}
	}

	// enemy and player collision
	for (unsigned i = 0; i < m_enemyNumber.size(); i++)
	{
		if (SDL_HasIntersection(m_enemyNumber[i]->GetEnemycoRekt(), m_player.GetPlayercoDst()))
		{
			m_pExplode.m_dst.x = m_player.m_dst.x;
			m_pExplode.m_dst.y = m_player.m_dst.y;
			Mix_PlayChannel(-1, explosion, 0);

			cout << "Enemy and Player Collision!" << endl;
			

			cout << "Game Over" << endl;
			m_playerOn = false;
			m_pExploOn = true;
		}
	}

	// tree collision
	for (unsigned i = 0; i < m_treeNumber.size(); i++)
	{
		if (SDL_HasIntersection(m_treeNumber[i]->GetTreecoRekt(), m_player.GetPlayercoDst()))
		{
			m_pExplode.m_dst.x = m_player.m_dst.x;
			m_pExplode.m_dst.y = m_player.m_dst.y;

			cout << "Tree Collision!" << endl;
			Mix_PlayChannel(-1, explosion, 0);

			cout << "Game Over" << endl;
			m_playerOn = false;
			m_pExploOn = true;
		}
	}

	// bird1 collision
	for (unsigned i = 0; i < m_bird1Number.size(); i++)
	{
		if (SDL_HasIntersection(m_bird1Number[i]->GetBird1coRekt(), m_player.GetPlayercoDst()))
		{
			m_pExplode.m_dst.x = m_player.m_dst.x;
			m_pExplode.m_dst.y = m_player.m_dst.y;

			cout << "Bird1 Collision!" << endl;
			Mix_PlayChannel(-1, explosion, 0);

			cout << "Game Over" << endl;
			m_playerOn = false;
			m_pExploOn = true;
		}
	}

	// bird2 collision
	for (unsigned i = 0; i < m_bird2Number.size(); i++)
	{
		if (SDL_HasIntersection(m_bird2Number[i]->GetBird2coRekt(), m_player.GetPlayercoDst()))
		{
			m_pExplode.m_dst.x = m_player.m_dst.x;
			m_pExplode.m_dst.y = m_player.m_dst.y;

			cout << "Bird2 Collision!" << endl;
			Mix_PlayChannel(-1, explosion, 0);

			cout << "Game Over" << endl;
			m_playerOn = false;
			m_pExploOn = true;
		}
	}
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 128, 255, 255);
	SDL_RenderClear(m_pRenderer);
	// Background
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg2.m_src, &m_bg2.m_dst);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg1.m_src, &m_bg1.m_dst);



	// Sprite
	if (m_playerOn == true)
	SDL_RenderCopy(m_pRenderer, m_pTexture, &m_player.m_src, &m_player.m_dst);




	// Tree Obstacles
	for (unsigned i = 0; i < m_treeNumber.size(); i++) // size() is actual filled numbers of elements
		m_treeNumber[i]->Render(m_pRenderer, m_pTreeTexture, &m_tree.m_src);
	// Enemy
	for (unsigned i = 0; i < m_enemyNumber.size(); i++) // size() is actual filled numbers of elements
		m_enemyNumber[i]->Render(m_pRenderer, m_pEnemyTexture, &m_enemy.m_src);

	// Bird1
	for (unsigned i = 0; i < m_bird1Number.size(); i++) // size() is actual filled numbers of elements
		m_bird1Number[i]->Render(m_pRenderer, m_bird1Texture, &m_bird1.m_src);
	// Bird2
	for (unsigned i = 0; i < m_bird2Number.size(); i++) // size() is actual filled numbers of elements
		m_bird2Number[i]->Render(m_pRenderer, m_bird2Texture, &m_bird2.m_src);

	// Bullet
	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
		m_bullet[i]->Render(m_pRenderer, m_pFireballTexture, &m_fireball.m_src);

	for (unsigned i = 0; i < m_enemyBullet.size(); i++) // size() is actual filled numbers of elements
		m_enemyBullet[i]->Render(m_pRenderer, m_pWaterballTexture, &m_waterball.m_src);

	// Explosion
	if (m_pExploOn == true)
		SDL_RenderCopy(m_pRenderer, m_explodeTexture, &m_pExplode.m_src, &m_pExplode.m_dst);
	if (m_eExploOn == true)
		SDL_RenderCopy(m_pRenderer, m_explodeTexture, &m_eExplode.m_src, &m_eExplode.m_dst);
	
	//SDL_RenderCopyEx(m_pRenderer, m_pTexture, &m_player.m_src, &m_player.m_dst, 90.0, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);

}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}


void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_bullet[i]; // flag for reallocation
		m_bullet[i] = nullptr; // get rid of the dangling pointer
	}
	for (unsigned i = 0; i < m_enemyBullet.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_enemyBullet[i]; // flag for reallocation
		m_enemyBullet[i] = nullptr; // get rid of the dangling pointer
	}
	for (unsigned i = 0; i < m_enemyNumber.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_enemyNumber[i]; // flag for reallocation
		m_enemyNumber[i] = nullptr; // get rid of the dangling pointer
	}
	for (unsigned i = 0; i < m_treeNumber.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_treeNumber[i]; // flag for reallocation
		m_treeNumber[i] = nullptr; // get rid of the dangling pointer
	}
	for (unsigned i = 0; i < m_bird1Number.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_bird1Number[i]; // flag for reallocation
		m_bird1Number[i] = nullptr; // get rid of the dangling pointer
	}
	for (unsigned i = 0; i < m_bird2Number.size(); i++) // size() is actual filled numbers of elements
	{
		delete m_bird2Number[i]; // flag for reallocation
		m_bird2Number[i] = nullptr; // get rid of the dangling pointer
	}

	m_bullet.clear();
	m_bullet.shrink_to_fit(); // reduces the capacity to size
	m_enemyBullet.clear();
	m_enemyBullet.shrink_to_fit(); // reduces the capacity to size
	m_enemyNumber.clear();
	m_enemyNumber.shrink_to_fit(); // reduces the capacity to size
	m_treeNumber.clear();
	m_treeNumber.shrink_to_fit(); // reduces the capacity to size
	m_bird1Number.clear();
	m_bird1Number.shrink_to_fit(); // reduces the capacity to size
	m_bird2Number.clear();
	m_bird2Number.shrink_to_fit(); // reduces the capacity to size


	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pFireballTexture);
	SDL_DestroyTexture(m_pWaterballTexture);
	SDL_DestroyTexture(m_explodeTexture);
	SDL_DestroyTexture(m_pTreeTexture);
	SDL_DestroyTexture(m_bird1Texture);
	SDL_DestroyTexture(m_bird2Texture);
	Mix_FreeChunk(waterball);
	Mix_FreeChunk(explosion);
	Mix_FreeChunk(shoot);
	Mix_FreeMusic(m_bgm);
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}


