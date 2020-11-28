#include "Engine.h"

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
	m_player = { {0,m_srcy+ m_srcHeight,m_srcWidth,m_srcHeight}, {0,384,m_dstWidth/5,m_dstHeight/5} }; // First {} is src rectangle, and second {} destination rect
	m_bg1 = { {0,0,1365,768},{0,0,1365,768} };
	m_bg2 = { {0,0,1365,768},{1365,0,1365,768} };
	// m_fireball = {0,0,  }

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

	m_time++;

	if (FPS / m_time == 4)
	{
		m_time = 0;
		m_player.m_src.x += m_srcWidth;
		if (m_player.m_src.x >= m_dstWidth)
			m_player.m_src.x = 0;
	}

	// Parse player movement.
	if (KeyDown(SDL_SCANCODE_W) && m_player.m_dst.y > 0)
		m_player.m_dst.y -= m_speed;
	else if (KeyDown(SDL_SCANCODE_S) && m_player.m_dst.y < HEIGHT - m_player.m_dst.h)
		m_player.m_dst.y += m_speed;
	if (KeyDown(SDL_SCANCODE_A) && m_player.m_dst.x > 0)
		m_player.m_dst.x -= m_speed;
	else if (KeyDown(SDL_SCANCODE_D) && m_player.m_dst.x < WIDTH/2 - m_player.m_dst.w)
		m_player.m_dst.x += m_speed;

	for(unsigned i = 0 ; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
		m_bullet[i]->Update();					//	 combines dereference and member accsess
	// check bullets going off screen

	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
	{
		if (m_bullet[i]->GetRekt()->x > WIDTH)
		{
			delete m_bullet[i]; // flag for reallocation
			m_bullet[i] = nullptr; // get rid of the dangling pointer
			m_bullet.erase(m_bullet.begin() + i);
			m_bullet.shrink_to_fit();
			cout << " Bullet Deleted \n";
			break;
		}
	}
	EnemyOne->Update();
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 128, 255, 255);
	SDL_RenderClear(m_pRenderer);
	// Background
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg2.m_src, &m_bg2.m_dst);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg1.m_src, &m_bg1.m_dst);
	// Bullet
	for (unsigned i = 0; i < m_bullet.size(); i++) // size() is actual filled numbers of elements
		m_bullet[i]->Render(m_pRenderer);

	//Enemy
	//EnemyOne.Render(m_pRenderer);
	// Sprite
	SDL_RenderCopy(m_pRenderer, m_pTexture, &m_player.m_src, &m_player.m_dst);
	
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
	m_bullet.clear();
	m_bullet.shrink_to_fit(); // reduces the capacity to size

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pFireballTexture);
	IMG_Quit();
	SDL_Quit();
}


