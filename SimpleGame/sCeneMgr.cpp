#include "stdafx.h"
#include "sCeneMgr.h"



sCeneMgr::sCeneMgr(int windowSizeX,int windowSizeY)
{
	m_bulletTime= (float)timeGetTime()*0.001f;
	g_Renderer = new Renderer(windowSizeX, windowSizeY);
	m_sound = new Sound();
	soundBG = m_sound->CreateSound("./Dependencies/SoundSamples/getout.ogg");
	m_sound->PlaySound(soundBG, true, 0.2f);
	
	WindowWidth = windowSizeX;
	WindowHeight = windowSizeY;
	m_redCharTime = (float)timeGetTime()*0.001f;
	m_blueCharTime = (float)timeGetTime()*0.001f;
	
	m_texCharacter = g_Renderer->CreatePngTexture("./Textures/PNGs/Knight.png");
	m_texCharacter2 = g_Renderer->CreatePngTexture("./Textures/PNGs/Ninza.png");
	m_background= g_Renderer->CreatePngTexture("./Textures/PNGs/background.png");
	m_texBullet= g_Renderer->CreatePngTexture("./Textures/PNGs/particle.png");
	m_texParticle= g_Renderer->CreatePngTexture("./Textures/PNGs/snow.png");
	for (int i = 0; i < ObjCount; i++)
	{
		obj[i] = NULL;
	}
	AddObject(0, 350, OBJECT_BUILDING,0);
	AddObject(-150, 350, OBJECT_BUILDING, 0);
	AddObject(150, 350, OBJECT_BUILDING, 0);

	AddObject(0, -350, OBJECT_BUILDING, 1);
	AddObject(-150, -350, OBJECT_BUILDING, 1);
	AddObject(150, -350, OBJECT_BUILDING, 1);
	
}




bool sCeneMgr::BoxBoxCollisionTest(float minX, float minY, float maxX, float maxY, float minX1, float minY1, float maxX1, float maxY1)
{
	if (minX > maxX1)
		return false;
	if (maxX < minX1)
		return false;

	if (minY > maxY1)
	    return false;
	if (maxY < minY1)
		return false;

	return true;
}
void sCeneMgr::Animate()
{
	float NowTime = (float)timeGetTime()*0.001f;
	if (NowTime - m_startTime >= 0.05f)
	{
		AniX++;
		if (AniX >= 10)
		{
			AniX = 0;
			AniY++;
		}
		if (AniY >= 2)
			AniY = 0;
		m_startTime = NowTime;
	}

	

}
void sCeneMgr::UpdateAllObjects(float elapsedTime)
{
	
	for (int i = 0; i < ObjCount; i++)
	{
		if (obj[i] != NULL)
		{
			
			if (obj[i]->GetLife() < 0.0001f || obj[i]->GetLifeTime() < 0.0001f || obj[i]->GetDelete()==true)
			{
				DeleteObject(i);
				//delete obj[i];
				//obj[i] = NUll;
			}			
			else
			{
				obj[i]->Update(elapsedTime);
				

			}
		}
	}
	snowTime += elapsedTime/1000.f;
	CollisionTest();
	RedCharacterAdd();
	RedBulletShot();
	//BulletShot();
	CharacterShot();
	Animate();
}
void sCeneMgr::DrawObjects()
{
	float NowTime = (float)timeGetTime()*0.001f;
	g_Renderer->DrawTexturedRect(0,0,0,800,1.0,1.0,1.0,1.0,m_background,0.99);
	DrawSnow();
	
	for (int i = 0; i < ObjCount; i++)
	{
		if (obj[i] != NULL)
		{			
			

			Drawtext(i);
			if (obj[i]->GetType() == OBJECT_BUILDING || obj[i]->GetType() == OBJECT_CHARACTER)
			{
				
				if (obj[i]->GetTeamNum() == TEAM_RED)
				{
					g_Renderer->DrawSolidRectGauge(
						obj[i]->GetpositionX(),
						obj[i]->GetpositionY() + obj[i]->Getsize()/2.f+5,
						0,
						obj[i]->Getsize(),
						3,
						1,
						0,
						0,
						1,
						obj[i]->GetGuage(),
						0.05
					);

				}
				else
				{
					g_Renderer->DrawSolidRectGauge(
						obj[i]->GetpositionX(),
						obj[i]->GetpositionY() + obj[i]->Getsize() / 2.f+5,
						0,
						obj[i]->Getsize(),
						3,
						0,
						0,
						1,
						1,
						obj[i]->GetGuage(),
						0.05
					);
				}
			}
			
			if (obj[i]->GetType() == OBJECT_BUILDING)
			{
				if (obj[i]->GetTeamNum() == TEAM_RED)
				{
					g_Renderer->DrawTexturedRectSeq(
						obj[i]->GetpositionX(),
						obj[i]->GetpositionY(),
						0, 
						obj[i]->Getsize(),
						1,
						1,
						1,
						1,
						m_texCharacter,
						AniX,
						AniY,
						10,
						3,
						LEVEL_BUILDING);
					//g_Renderer->DrawTexturedRect(
					//	obj[i]->GetpositionX(),
					//	obj[i]->GetpositionY(),
					//	0,
					//	obj[i]->Getsize(),
					//	1,
					//	1,
					//	1,
					//	1,
					//	m_texCharacter,
					//	LEVEL_BUILDING
					//);
				}
				else
				{
				
					g_Renderer->DrawTexturedRectSeq(
						obj[i]->GetpositionX(),
						obj[i]->GetpositionY(),
						0,
						obj[i]->Getsize(),
						1,
						1,
						1,
						1,
						m_texCharacter2,
						AniX,
						AniY,
						10,
						3,
						LEVEL_BUILDING);
					//g_Renderer->DrawTexturedRect(
					//	obj[i]->GetpositionX(),
					//	obj[i]->GetpositionY(),
					//	0,
					//	obj[i]->Getsize(),
					//	1,
					//	1,
					//	1,
					//	1,
					//	m_texCharacter2,
					//	LEVEL_BUILDING
					//);
				}
			}
			
			else
			{
				g_Renderer->DrawSolidRect(
					obj[i]->GetpositionX(),
					obj[i]->GetpositionY(),
					0,
					obj[i]->Getsize(),
					obj[i]->m_color.getX(),
					obj[i]->m_color.getY(),
					obj[i]->m_color.getZ(),
					obj[i]->m_color.getW(),
					LEVEL_BULLET
				);
				if(obj[i]->GetType()==OBJECT_BULLET)
				{
					g_Renderer->DrawParticle(
						obj[i]->GetpositionX(),
						obj[i]->GetpositionY(),
						0,
						3,
						1,
						1,
						1,
						1,
						-obj[i]->GetdirX()*0.01f,
						-obj[i]->GetdirY()*0.01f ,
						m_texBullet,
						//m_bulletTime - NowTime / 2.0f,
						obj[i]->m_particleTime,
						0.5
					);
				}
				
			}
		}
	}

}
int sCeneMgr::AddObject(float x,float y,int type,int num)
{
	for (int i = 0; i < ObjCount; i++)
	{
		if (obj[i] == NULL)
		{
		
				obj[i] = new Object(x, y, type, num);
			
		
			//obj[i]->SetArrowOwn(num);
		
			return i;
		}
	}
	return -1;
}

void sCeneMgr::DeleteObject(int index)
{
	if (obj[index] != NULL)
	{
		delete obj[index];
		obj[index] = NULL;
	}
}
void sCeneMgr::CollisionTest()
{
	for (int i = 0; i < ObjCount; i++)
	{
		
		if (obj[i] != NULL)
		{
			for (int j = 0; j < ObjCount; j++)
			{
				if (i == j)
					continue;
				if (obj[j] != NULL)
				{
					float minX, minY;
					float maxX, maxY;

					float minX1, minY1;
					float maxX1, maxY1;

					minX = obj[i]->GetpositionX() - obj[i]->Getsize()/ 2.f;
					minY = obj[i]->GetpositionY() - obj[i]->Getsize()/ 2.f;
					maxX = obj[i]->GetpositionX() + obj[i]->Getsize()/ 2.f;
					maxY = obj[i]->GetpositionY() + obj[i]->Getsize()/ 2.f;
					
					minX1= obj[j]->GetpositionX ()- obj[j]->Getsize() / 2.f;
					minY1 = obj[j]->GetpositionY() - obj[j]->Getsize() / 2.f;
					maxX1= obj[j]->GetpositionX() + obj[j]->Getsize() / 2.f;
					maxY1 = obj[j]->GetpositionY() + obj[j]->Getsize() / 2.f;
					if (BoxBoxCollisionTest(minX, minY, maxX, maxY, minX1, minY1, maxX1, maxY1))
					{
						CollisionAfer(i, j);
						
						//DeleteObject(i);
					}
				}
			}
			
		}
	}
}

void sCeneMgr::RedCharacterAdd()
{
	
	//x - 250, -y + 400
	float NowTime = (float)timeGetTime()*0.001f;
	if (NowTime -m_redCharTime >= 5.0f)
	{
		AddObject((int)std::rand()%500-250, (int)std::rand() % 400, 1, TEAM_RED);
		m_redCharTime = NowTime;
	}
}
void sCeneMgr::BlueCharacterAdd(float x, float y, int type, int num)
{
	float NowTime = (float)timeGetTime()*0.001f;
	if (NowTime - m_blueCharTime >= 7.0f)
	{
		AddObject(x, y, 1, TEAM_BLUE);
		m_blueCharTime = NowTime;
	}
	
	
}
void sCeneMgr::RedBulletShot()
{
	for (int i = 0; i < ObjCount; i++)
	{
		if (obj[i] != NULL && obj[i]->GetType() == OBJECT_BUILDING)
		{
			obj[i]->m_nowTime = (float)timeGetTime()*0.001f;
			if (obj[i]->m_nowTime - obj[i]->GetstartTime() >= 1.0f)
			{
				if (obj[i]->GetTeamNum() == TEAM_RED)
					AddObject(obj[i]->GetpositionX(), obj[i]->GetpositionY(), 2, TEAM_RED);
				else if (obj[i]->GetTeamNum() == TEAM_BLUE)
					AddObject(obj[i]->GetpositionX(), obj[i]->GetpositionY(), 2, TEAM_BLUE);

				obj[i]->SetstartTime(obj[i]->m_nowTime);
				
			}
		}
	}
	
}
void sCeneMgr::CharacterShot()
{
	
	for (int i = 0; i < ObjCount; i++)
	{
		if (obj[i] != NULL && obj[i]->GetType()==OBJECT_CHARACTER)
		{
			obj[i]->m_nowTime = (float)timeGetTime()*0.001f;
			if (obj[i]->m_nowTime - obj[i]->GetstartTime() >= 3.0f)
			{
				if(obj[i]->GetTeamNum()==TEAM_RED)
					AddObject(obj[i]->GetpositionX(), obj[i]->GetpositionY(), 3, TEAM_RED);
				else if (obj[i]->GetTeamNum() == TEAM_BLUE)
					AddObject(obj[i]->GetpositionX(), obj[i]->GetpositionY(), 3, TEAM_BLUE);


				//if (num >= ObjCount-1)
				//	DeleteObject(num);
				//else
				//{
				
					obj[i]->SetstartTime(obj[i]->m_nowTime);
					//obj[num]->SetArrowOwn(i);
				//}
							
			}
		}
	}
	
}

void sCeneMgr::Drawtext(int index)
{
	int HP = obj[index]->GetLife();
	string str = to_string(HP);
	char* c_HP = (char*)str.c_str();
	if (obj[index]->GetType() == OBJECT_BUILDING)
	{
		g_Renderer->DrawText(
			obj[index]->GetpositionX() - 15,
			obj[index]->GetpositionY() - 40,
			GLUT_BITMAP_9_BY_15,
			1.0, 0, 0,
			c_HP);
	}
	else if (obj[index]->GetType() == OBJECT_CHARACTER)
	{
		g_Renderer->DrawText(
			obj[index]->GetpositionX() - 15,
			obj[index]->GetpositionY() - 20,
			GLUT_BITMAP_9_BY_15,
			1.0, 0, 0,
			c_HP);
	}
	
}
void sCeneMgr::SoundExplosion()
{
	float NowTime = (float)timeGetTime()*0.001f;
	soundEX = m_sound->CreateSound("./Dependencies/SoundSamples/expolsion.wav");
	m_sound->PlaySound(soundEX, true, 0.4f);
	if (NowTime >= 1.0f)
	{
		m_sound->DeleteSound(soundEX);
		soundTime = NowTime;
	}
}
void sCeneMgr::DrawSnow()
{
	g_Renderer->DrawParticleClimate(0,0,0,1,1,1,1,1,-0.1,-0.1,m_texParticle,snowTime,0.01);
}
void sCeneMgr::CollisionAfer(int i, int j)
{

	if (obj[i]->GetTeamNum() != obj[j]->GetTeamNum())
	{
		SoundExplosion();
		if (obj[i]->GetType() == OBJECT_BUILDING && obj[j]->GetType() == OBJECT_CHARACTER)
		{
			obj[i]->SetLife(obj[i]->GetLife() - obj[j]->GetLife());
			DeleteObject(j);
		}
		else if (obj[i]->GetType() == OBJECT_CHARACTER && obj[j]->GetType() == OBJECT_BULLET)
		{

			obj[i]->SetLife(obj[i]->GetLife() - obj[j]->GetLife());
			DeleteObject(j);

		}
		else if (obj[i]->GetType() == OBJECT_BUILDING  && obj[j]->GetType() == OBJECT_ARROW)
		{

			obj[i]->SetLife(obj[i]->GetLife() - obj[j]->GetLife());
			DeleteObject(j);
		}
		else if (obj[i]->GetType() == OBJECT_CHARACTER && obj[j]->GetType() == OBJECT_ARROW)
		{
			if (i != obj[j]->GetArrowOwn())
			{
				obj[i]->SetLife(obj[i]->GetLife() - obj[j]->GetLife());
				DeleteObject(j);
			}
		}
		else if (obj[i]->GetType() == OBJECT_BUILDING && obj[j]->GetType() == OBJECT_BULLET)
		{
			obj[i]->SetLife(obj[i]->GetLife() - obj[j]->GetLife());
			DeleteObject(j);
		}
	}	
}
Object* sCeneMgr::Getobject(int index)
{
	return obj[index];
}
sCeneMgr::~sCeneMgr()
{
}

void Timer::addTime(float elapsedTime)
{
	Time += elapsedTime/1000.f;
}
float Timer::getTime()
{
	return Time;
}
