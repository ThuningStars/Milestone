//#include "SoundManager.h"
//#include<ostream>
//using namespace std;
////initializer
//SoundMng* SoundMng::s_instance = nullptr;
//
////Constructor
//SoundMng::SoundMng()
//{
//	if (Mix_OpenAudio(FREQ, MIX_DEFAULT_FORMAT, 2, CHUNK_SIZE) < 0)
//		cerr << Mix_GetError() << endl;
//}
////Music
//void SoundMng::PlayMusic(string id)
//{
//	if (Mix_PlayMusic(m_musicmap[id], -1) == -1)
//		cerr << Mix_GetError() << " : " << id << endl;
//}
//void SoundMng::LoadMusic(string id, string source)
//{
//	Mix_Music* music = Mix_LoadMUS(source.c_str());
//	if (music != nullptr)
//		m_musicmap[id] = music;
//	else
//		cerr << Mix_GetError() << " : " << source << endl;
//}
//
////Chunk SFX
//void SoundMng::PlayEffect(string id)
//{
//	if (Mix_PlayChannel(-1, m_effectmap[id], 0) == -1)
//		cerr << Mix_GetError() << " : " << id << endl;
//}
//void SoundMng::LoadEffect(string id, string source)
//{
//	Mix_Chunk* effect = Mix_LoadWAV(source.c_str());
//	if (effect != nullptr)
//		m_effectmap[id] = effect;
//	else
//		cerr << Mix_GetError() << " : " << source << endl;
//}
//void SoundMng::Clean() 
//{
//	for (MusicMap::iterator it = m_musicmap.begin(); it != m_musicmap.end(); it++)
//		Mix_FreeMusic(it->second);
//	for (EffectMap::iterator it = m_effectmap.begin(); it != m_effectmap.end(); it++)
//		Mix_FreeChunk(it->second);
//
//	m_musicmap.clear();
//	m_effectmap.clear();
//	cout << "Sound System cleared \n";
//}