#include "Scene.h"
#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include "Components.h"
#include "Scene_Menu.h"
#include "Action.h"

#include <iostream>
#include <memory>
#include <cassert>
#include <fstream>
#include <format>
#include <SFML/Graphics.hpp>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");			// Toggle drawing (T)extures
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");		// Toggle drawing (C)ollision
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");		// Toggle grid (G)rid
	registerAction(sf::Keyboard::W, "UP");		// Jump (W)
	registerAction(sf::Keyboard::Space, "SHOOT");		// Jump (Spacebar)
	registerAction(sf::Keyboard::S, "DOWN");		// Crouch (S)
	registerAction(sf::Keyboard::A, "LEFT");		// Walk left (A)
	registerAction(sf::Keyboard::D, "RIGHT");		// Walk right (D)

	m_gridText.setCharacterSize(12);
 	m_gridText.setFont(m_game->assets().getFont("Tech"));

	m_nameText.setCharacterSize(16);
	m_nameText.setFont(m_game->assets().getFont("Pixeled"));
	m_nameText.setPosition(10, 10);
	m_nameText.setString("MARIO");

	m_scoreText.setCharacterSize(16);
	m_scoreText.setFont(m_game->assets().getFont("Pixeled"));
	m_scoreText.setPosition(10, 16+m_nameText.getCharacterSize());
	m_scoreText.setString("0");

	m_coinsText.setCharacterSize(16);
	m_coinsText.setFont(m_game->assets().getFont("Pixeled"));
	m_coinsText.setPosition(m_nameText.getPosition().x + m_nameText.getLocalBounds().width + 24, 10);
	m_coinsText.setString("x00");

	loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	assert(entity->hasComponent<CAnimation>());

	Vec2 entitySize{ entity->getComponent<CAnimation>().animation.getSize() };

	int x = gridX + (entitySize.x * entity->getComponent<CTransform>().scale.x) / 2;
	int y{ 0 };
	y = gridY + (entitySize.y * entity->getComponent<CTransform>().scale.y) / 2;

	return Vec2(x, y);
}

void Scene_Play::loadLevel(const std::string& fileName)
{
	// reset the entity manager every time we load a level
	m_entityManager = EntityManager();

	std::string buf;
	std::ifstream fin(fileName);

	sf::Vector2u windowSize = m_game->window().getSize();
	float maxGY = windowSize.y / m_gridSize.y - 1;

	while (fin >> buf)
	{
		if (buf == "Tile")
		{
			std::string animationName;
			float gx, gy;
			fin >> animationName >> gx >> gy;

			gy = maxGY - gy;

			Vec2 animationSize(m_game->assets().getAnimation(animationName).getSize());
			float scaleX = 64 / animationSize.x;
			float scaleY = 64 / animationSize.y;

			if (animationName == "PipeTall")
			{
				scaleX = 384 / animationSize.x;
				scaleY = 256 / animationSize.y;
				gy -= 3.7;
			}
			else if (animationName == "Pole")
			{
				scaleY = 448 / animationSize.y;
				gy -= 6;
			}

			auto tile = m_entityManager.addEntity(buf);
			tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
			tile->addComponent<CTransform>();
			tile->getComponent<CTransform>().scale.x = scaleX;
			tile->getComponent<CTransform>().scale.y = scaleY;
			tile->getComponent<CTransform>().pos = gridToMidPixel(gx * 64, gy * 64, tile);
			if (animationName == "PipeTall")
			{
				tile->addComponent<CBoundingBox>((tile->getComponent<CAnimation>().animation.getSize() * Vec2(.30, .90)) * tile->getComponent<CTransform>().scale);
				Vec2 calc{ (tile->getComponent<CAnimation>().animation.getSize() * tile->getComponent<CTransform>().scale) };
			}
			else
			{
				tile->addComponent<CBoundingBox>(tile->getComponent<CAnimation>().animation.getSize() * tile->getComponent<CTransform>().scale);
			}
		}
		else if (buf == "Dec")
		{
			std::string animationName;
			float gx, gy;
			fin >> animationName >> gx >> gy;

			gy = maxGY - gy;
			Vec2 animationSize(m_game->assets().getAnimation(animationName).getSize());
			float scaleX = 192 / animationSize.x;
			float scaleY = 128 / animationSize.y;

			gy -= 1;

			auto tile = m_entityManager.addEntity(buf);
			tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
			tile->addComponent<CTransform>();
			tile->getComponent<CTransform>().scale.x = scaleX;
			tile->getComponent<CTransform>().scale.y = scaleY;
			tile->getComponent<CTransform>().pos = gridToMidPixel(gx * 64, gy * 64, tile);
			tile->addComponent<CBoundingBox>(tile->getComponent<CAnimation>().animation.getSize() * tile->getComponent<CTransform>().scale);
		}
		else if (buf == "Player")
		{
				fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY >> m_playerConfig.SPEED >> m_playerConfig.JUMP
				>> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY >> m_playerConfig.weapon;
			spawnPlayer();
		}
		else
		{
			std::cout << "Found an unknown level entity.\n";
			exit(-1);
		}
	}
}

void Scene_Play::spawnPlayer()
{
	//Player GX / Y CW CH SX SY SM GY B
	//	GX, GY Grid Pos X, Y float, float(starting position of player)
	//	BoundingBox W / H CW, CH float, float
	//	Left / Right Speed SX float
	//	Jump Speed SY float
	//	Movement speed SX
	//	Max Speed SM float
	//	Gravity GY float
	//	Bullet Animation B std::string(Animation asset to use for bullets)
	// here is a sample player entity which you can use to construct other entities
	sf::Vector2u windowSize = m_game->window().getSize();
	float maxGY = windowSize.y / m_gridSize.y + 3;

	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);

	m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X * 64, (maxGY - m_playerConfig.Y) * 64, m_player));
	m_player->getComponent<CTransform>().scale.x = 64 / m_game->assets().getAnimation("Stand").getSize().x;
	m_player->getComponent<CTransform>().scale.y = 64 / m_game->assets().getAnimation("Stand").getSize().y;
	m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
	m_player->addComponent<CInput>();
	m_player->addComponent<CState>();
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);

}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	auto bullet = m_entityManager.addEntity("bullet");
	
	// bullet only exists for 300 frames
	const unsigned int bulletLifespan = 300;

	float bulletVelocityX = 2;
	if (m_player->getComponent<CTransform>().scale.x < 0)
		bulletVelocityX = -2;

	bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.weapon), true);
	bullet->addComponent<CTransform>(gridToMidPixel(m_player->getComponent<CTransform>().pos.x, m_player->getComponent<CTransform>().pos.y, bullet));
	bullet->getComponent<CTransform>().velocity = { bulletVelocityX, 0 };
	bullet->addComponent<CBoundingBox>(Vec2(22, 22));
	bullet->addComponent<CLifeSpan>(bulletLifespan, m_game->getCurrentFrames());
}

void Scene_Play::update()
{
	m_entityManager.update();

	if (!m_player->isActive())
		spawnPlayer();

	if (!m_paused)
	{
		sMovement();
		sLifespan();
		sCollision();
		sAnimation();
	}
	sRender();
}

void Scene_Play::sMovement()
{
	Vec2 playerVelocity{ 0, m_player->getComponent<CTransform>().velocity.y };

	if (m_player->getComponent<CInput>().left)
	{
		if (m_player->getComponent<CTransform>().scale.x > 0)
			m_player->getComponent<CTransform>().scale.x = -m_player->getComponent<CTransform>().scale.x;
		playerVelocity.x = -m_playerConfig.SPEED;
		if (m_player->getComponent<CState>().state != "jumping")
			m_player->getComponent<CState>().state = "run";
	}

	if (m_player->getComponent<CInput>().right)
	{
		if (m_player->getComponent<CTransform>().scale.x < 0)
			m_player->getComponent<CTransform>().scale.x = -m_player->getComponent<CTransform>().scale.x;
		playerVelocity.x = m_playerConfig.SPEED;
		if (m_player->getComponent<CState>().state != "jumping")
			m_player->getComponent<CState>().state = "run";
	}

	if (m_player->getComponent<CInput>().up && m_grounded)
	{
		playerVelocity.y = m_playerConfig.JUMP;
	}

	m_player->getComponent<CTransform>().velocity = playerVelocity;

	for (auto& e : m_entityManager.getEntities())
	{
		if (e->getComponent<CTransform>().velocity.x > m_playerConfig.MAXSPEED)
			e->getComponent<CTransform>().velocity.x = m_playerConfig.MAXSPEED;
		if (e->getComponent<CTransform>().velocity.y > m_playerConfig.MAXSPEED)
			e->getComponent<CTransform>().velocity.y = m_playerConfig.MAXSPEED;
		if (e->hasComponent<CGravity>())
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
	}
}

void Scene_Play::sLifespan()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (!e->hasComponent<CLifeSpan>())
			continue;

		auto& eCLifespan = e->getComponent<CLifeSpan>();
		if ((m_game->getCurrentFrames() - (eCLifespan.frameCreated + eCLifespan.lifespan)) >= 0)
			e->destroy();
	}
}

void Scene_Play::sCollision()
{
	auto& tileVec = m_entityManager.getEntities("Tile");
	auto& decVec = m_entityManager.getEntities("Dec");

	m_grounded = false;

	for (auto& tile : tileVec)
	{
		if (m_entityManager.entityExists("bullet"))
		{
			auto& bulletVec = m_entityManager.getEntities("bullet");
			for (auto& bulletEnt : bulletVec)
			{
				for (auto& tile : tileVec)
				{
					Vec2 ov = Physics::GetOverlap(tile, bulletEnt);
					if (ov.x <= 0 || ov.y <= 0)
						continue;
					bulletEnt->destroy();
					if (tile->getComponent<CAnimation>().animation.getName() != "Brick")
						continue;
					tile->addComponent<CAnimation>(m_game->assets().getAnimation("Explosion"), false);
					tile->getComponent<CTransform>().scale.x = 1;
					tile->getComponent<CTransform>().scale.y = 1;
					// bounding box is not needed for the explosion
					tile->getComponent<CBoundingBox>().has = false;

				}
			}
		}

		Vec2 ovPlayer = Physics::GetOverlap(m_player, tile);
		// there are no overlaps (player is in the air)
		if (ovPlayer.x <= 0 || ovPlayer.y <= 0)
		{
			continue;
		}

		if (tile->getComponent<CAnimation>().animation.getName() == "Pole")
		{
			// we have hit the ending pole
			m_player->getComponent<CTransform>().pos.x += 3;
			onEnd();
			continue;
		}

		// if hit the brick from below set player y velocity to 0 and explode brick
		Vec2 prevOvPlayer = Physics::GetPreviousOverlap(m_player, tile);

		// there is a horizontal overlap before we came from somewhere up top
		if (prevOvPlayer.x > 0)
		{
			// we hit the tile from the bottom
			if (m_player->getComponent<CTransform>().pos.y < m_player->getComponent<CTransform>().prevPos.y)
			{
				if (tile->getComponent<CAnimation>().animation.getName() == "Brick")
					tile->destroy();
				// we did not hit a block so push the player back down
				else if (tile->getComponent<CAnimation>().animation.getName() == "Question")
				{
					tile->addComponent<CAnimation>(m_game->assets().getAnimation("Question3"), true);
					tile->getComponent<CTransform>().scale.x = 64 / m_game->assets().getAnimation("Question3").getSize().x;
					tile->getComponent<CTransform>().scale.y = 64 / m_game->assets().getAnimation("Question3").getSize().y;
					m_player->getComponent<CTransform>().pos.y += ovPlayer.y;

					auto coinEnt = m_entityManager.addEntity("coin");
					coinEnt->addComponent<CAnimation>(m_game->assets().getAnimation("CoinAnim"), true);
					coinEnt->addComponent<CTransform>();
					coinEnt->getComponent<CTransform>().scale.x = 64 / m_game->assets().getAnimation("CoinAnim").getSize().x;
					coinEnt->getComponent<CTransform>().scale.y = 128 / m_game->assets().getAnimation("CoinAnim").getSize().y;
					// we do -32 on the x position because the original position of the question block is in the middle of the grid
					// we do -96 on the y position because itthe original position of the question block is in the middle of the grid and one block under where it is 
					// we do -128 on the y position because itthe original position of the question block is in the middle of the grid and one block under where it is 
					//		supposed to be. it is 128 instead of -96 because the y size of the coin is bigger from the scale.y
					coinEnt->getComponent<CTransform>().pos = gridToMidPixel(tile->getComponent<CTransform>().pos.x-32, tile->getComponent<CTransform>().pos.y - 128, coinEnt);
					coinEnt->addComponent<CLifeSpan>(150, m_game->getCurrentFrames());
					m_score += 300;
					m_coins += 1;

				}
				else
					m_player->getComponent<CTransform>().pos.y += ovPlayer.y;

				m_player->getComponent<CTransform>().velocity.y = 0;
				m_player->getComponent<CInput>().up = false;
			}

			// player hit the block from up top
			else
			{
				// reset the velocity we are on top of a block
				m_player->getComponent<CTransform>().velocity.y = 0;
				m_player->getComponent<CInput>().canJump = true;
				m_player->getComponent<CTransform>().pos.y -= ovPlayer.y;
				// We are standing on a block so change the state to standing
				m_player->getComponent<CState>().state = "standing";
				m_grounded = true;
			}
		}
		else
		{
			// we came from the left side
			if (m_player->getComponent<CTransform>().pos.x > m_player->getComponent<CTransform>().prevPos.x)
			{
				m_player->getComponent<CTransform>().pos.x -= ovPlayer.x;
			}
			// we came from the right side
			else
			{
				m_player->getComponent<CTransform>().pos.x += ovPlayer.x;
			}
		}
	}

	// make sure the player does not fall of the map on the left
	if (m_player->getComponent<CTransform>().pos.x - m_player->getComponent<CBoundingBox>().halfSize.x < 0)
	{
		m_player->getComponent<CTransform>().pos.x = 0 + m_player->getComponent<CBoundingBox>().halfSize.x;
	}
	
	// if the player falls of the map kill the player
	if (m_player->getComponent<CTransform>().pos.y > m_game->window().getSize().y)
	{
		m_player->destroy();
	}
}

void Scene_Play::doAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; } // T
		else if (action.getName() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; } // C
		else if (action.getName() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; } // G
		else if (action.getName() == "PAUSE") { setPaused(!m_paused); } // P
		else if (action.getName() == "QUIT") { onEnd(); } // ESC

		else if (action.getName() == "UP") // W
		{ 
			if (m_player->getComponent<CInput>().canJump)
			{
				m_player->getComponent<CInput>().up = true;
				m_player->getComponent<CInput>().canJump = false;
				m_player->getComponent<CState>().state = "jumping";
			}
		}
		else if (action.getName() == "SHOOT") // SPACEBAR
		{
			if (m_player->getComponent<CInput>().canShoot != false)
				spawnBullet(m_player);
			m_player->getComponent<CInput>().canShoot = false;
			if (m_player->getComponent<CState>().state != "jumping")
				m_player->getComponent<CState>().state = "shoot";
		}
		else if (action.getName() == "LEFT") // A
		{
			m_player->getComponent<CInput>().left = true;
		}
		else if (action.getName() == "RIGHT") // D
		{
			m_player->getComponent<CInput>().right = true;
		}
#ifdef _DEBUG
		// This can be used for debugging when needed
		//else if (action.getName() == "DOWN") // S
		//{
		//	m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;
		//	m_player->getComponent<CTransform>().pos.y -= m_playerConfig.JUMP;
		//	//m_player->getComponent<CState>().state = "down";
		//}
#endif
	}

	else if (action.getType() == "END")
	{
		if (action.getName() == "LEFT") // A
		{
			m_player->getComponent<CInput>().left = false;
		}
		else if (action.getName() == "SHOOT") // SPACEBAR
		{
			m_player->getComponent<CInput>().canShoot = true;
		}
		else if (action.getName() == "RIGHT") // D
		{
			m_player->getComponent<CInput>().right = false;
		}
		else if (action.getName() == "UP") // W
		{
			if (m_player->getComponent<CInput>().up)
			{
				m_player->getComponent<CInput>().up = false;
				m_player->getComponent<CTransform>().velocity.y = 0;
				m_player->getComponent<CInput>().canJump = true;
			}
		}
	}
}

void Scene_Play::sAnimation()
{
	if (m_player->getComponent<CState>().state == "jumping")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Jump"), true);
	}
	else if (m_player->getComponent<CState>().state == "standing")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	}
	else if (m_player->getComponent<CState>().state == "shoot")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Shoot"), true);
	}
	else if (m_player->getComponent<CState>().state == "run" && m_player->getComponent<CAnimation>().animation.getName() != "Run")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
		m_player->getComponent<CState>().state = "run2";
	}

	for (auto& e : m_entityManager.getEntities())
	{
		if (!e->hasComponent<CAnimation>())
			continue;
		e->getComponent<CAnimation>().animation.update();
		if (e->getComponent<CAnimation>().animation.hasEnded() && !e->getComponent<CAnimation>().repeat)
			e->destroy();
	}

}

void Scene_Play::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::sRender()
{
	// color the background darker so you know that the game is paused
	if (!m_paused)
	{
		m_game->window().clear(sf::Color(100, 100, 255));
	}
	else
	{
		m_game->window().clear(sf::Color(50, 50, 150));
	}

	// set the viewport of the window to be centered on the player if it's far enough right
	auto& pPos = m_player->getComponent<CTransform>().pos;
	float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
	sf::View view = m_game->window().getView();
	view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
	m_game->window().setView(view);

	// draw all Entity textures / animations
	if (m_drawTextures)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();

			if (e->hasComponent<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimation>().animation;
				if (animation.getName() == "Stand")
					continue;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	// draw all Entity collision bounding boxes with a rectangle shape
	if (m_drawCollision)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0,0,0,0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

	// draw the grid so that you can easily debug
	if (m_drawGrid)
	{
		float leftX = m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2(x, 0), Vec2(x, height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x +3, height() - y - m_gridSize.y + 2);
				m_game->window().draw(m_gridText);
			}
		}
	}


	// TODO: fix this and make it so that we don't have to call it like this but a loop
	// render the player last so that he is on top of the decoration
	auto& transform = m_player->getComponent<CTransform>();
	auto& animation = m_player->getComponent<CAnimation>().animation;
	animation.getSprite().setRotation(transform.angle);
	animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
	animation.getSprite().setScale(transform.scale.x, transform.scale.y);
	m_game->window().draw(animation.getSprite());
	m_game->window().setView(m_game->window().getDefaultView());

	m_scoreText.setString(std::format("{}", m_score));
	m_coinsText.setString(std::format("x{}", m_coins));
	m_game->window().draw(m_nameText);
	m_game->window().draw(m_scoreText);
	m_game->window().draw(m_coinsText);

	if (m_paused)
	{
		sf::RectangleShape overlay(sf::Vector2f(m_game->window().getSize().x, m_game->window().getSize().y));
		overlay.setFillColor(sf::Color(191, 191, 191, 1));
		m_game->window().draw(overlay);
		//m_game->window().clear(sf::Color(50, 50, 150));
	}

	m_game->window().display();
}

void Scene_Play::registerAction(sf::Keyboard::Key key, const std::string& state)
{
	m_actionMap[key] = state;
}
