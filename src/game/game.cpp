#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>
#include <cmath>
//#include <GameObjects/GameObject.h>
#include "game.h"

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
SpaceInvaders::SpaceInvaders()
{
  game_name = "Space Invaders";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
SpaceInvaders::~SpaceInvaders()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool SpaceInvaders::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  initDefender();
  initAliens();
  initLasers();
  initBarriers();
  initEarth();

  toggleFPS();

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &SpaceInvaders::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &SpaceInvaders::clickHandler, this);

  return true;
}

bool SpaceInvaders::initDefender()
{
  if (!defender.addSpriteComponent(renderer.get(), "/data/images/defender.png"))
  {
    return false;
  }

  defender.spriteComponent()->getSprite()->xPos(
    game_width / 2 - (defender.spriteComponent()->getSprite()->width() / 2));
  defender.spriteComponent()->getSprite()->yPos(game_height - 100);
}

bool SpaceInvaders::initAliens()
{
  for (int i = 0; i < alien_count; i++)
  {
    if (!aliens[i].addSpriteComponent(renderer.get(),
                                      "/data/images/green_alien.png"))
    {
      return false;
    }

    aliens[i].visibility = true;

    if (i < 10)
    {
      aliens[i].spriteComponent()->getSprite()->xPos((i * 40) + 100);
      aliens[i].spriteComponent()->getSprite()->yPos(alien_y_pos);
    }
    else if (i >= 10 && i < 20)
    {
      aliens[i].spriteComponent()->getSprite()->xPos(((i - 10) * 40) + 100);
      aliens[i].spriteComponent()->getSprite()->yPos(2 * alien_y_pos);
    }
    else if (i >= 20 && i < 30)
    {
      aliens[i].spriteComponent()->getSprite()->xPos(((i - 20) * 40) + 100);
      aliens[i].spriteComponent()->getSprite()->yPos(3 * alien_y_pos);
    }
    else if (i >= 30 && i < 40)
    {
      aliens[i].spriteComponent()->getSprite()->xPos(((i - 30) * 40) + 100);
      aliens[i].spriteComponent()->getSprite()->yPos(4 * alien_y_pos);
    }
    else
    {
      aliens[i].spriteComponent()->getSprite()->xPos(((i - 40) * 40) + 100);
      aliens[i].spriteComponent()->getSprite()->yPos(5 * alien_y_pos);
    }
  }
}

bool SpaceInvaders::initLasers()
{
  for (int i = 0; i < laser_count; i++)
  {
    if (!lasers[i].addSpriteComponent(renderer.get(),
                                      "/data/images/SpaceShooterRedux/PNG/"
                                      "Lasers/laserRed01.png"))
    {
      return false;
    }

    lasers[i].spriteComponent()->getSprite()->yPos(
      defender.spriteComponent()->getSprite()->yPos() -
      lasers[i].spriteComponent()->getSprite()->height());
  }
}

bool SpaceInvaders::initBarriers()
{
  for (int i = 0; i < barrier_count; i++)
  {
    if (!barriers[i].addSpriteComponent(renderer.get(),
                                        "/data/images/barrier.png"))
    {
      return false;
    }

    if (i < 13)
    {
      barriers[i].spriteComponent()->getSprite()->xPos(game_width * 0.25);
      barriers[i].spriteComponent()->getSprite()->yPos(game_height / 2.0);
    }
    else if (i >= 13 && i < 25)
    {
      barriers[i].spriteComponent()->getSprite()->xPos(game_width * 0.5);
      barriers[i].spriteComponent()->getSprite()->yPos(game_height / 2.0);
    }
    else
    {
      barriers[i].spriteComponent()->getSprite()->xPos(game_width * 0.75);
      barriers[i].spriteComponent()->getSprite()->yPos(game_height / 2.0);
    }
  }
}

bool SpaceInvaders::initEarth()
{
  if (!earth.addSpriteComponent(renderer.get(),
                                "/data/images/destroyed_earth.png"))
  {
    return false;
  }
  earth.spriteComponent()->getSprite()->xPos(
    game_width / 2.0 - earth.spriteComponent()->getSprite()->width() / 2);
  earth.spriteComponent()->getSprite()->yPos(
    game_height / 2.0 - earth.spriteComponent()->getSprite()->height() / 2);
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void SpaceInvaders::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/
  // Scaling_and_MultiResolution_in_2D_Games.php

  // 720p is a pretty modest starting point, consider 1080p
  game_width = 1280;
  game_height = 720;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void SpaceInvaders::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (key->key == ASGE::KEYS::KEY_P && key->action == ASGE::KEYS::KEY_PRESSED &&
      in_pause)
  {
    // ASGE::DebugPrinter{} << "P pressed" << std::endl;
    in_pause = false;
    in_game = true;
  }
  else if (key->key == ASGE::KEYS::KEY_P &&
           key->action == ASGE::KEYS::KEY_PRESSED && in_game)
  {
    // ASGE::DebugPrinter{} << "P pressed" << std::endl;
    in_pause = true;
    in_game = false;
  }

  if (in_menu)
  {
    if (key->key == ASGE::KEYS::KEY_UP &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      menu_option = menu_option - 1;
    }

    else if (key->key == ASGE::KEYS::KEY_DOWN &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
      menu_option = menu_option + 1;
    }

    if (menu_option == 4)
    {
      menu_option = 0;
    }
    else if (menu_option == -1)
    {
      menu_option = 3;
    }

    if (key->key == ASGE::KEYS::KEY_ENTER)
    {
      in_menu = false;
      in_game = true;
    }
  }

  if (in_game)
  {
    if (key->key == ASGE::KEYS::KEY_A && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      defender.setVelocity(Vector2{ -450, 0 });
    }

    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      defender.setVelocity(Vector2{ 0, 0 });
    }

    if (key->key == ASGE::KEYS::KEY_D && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      defender.setVelocity(Vector2{ 450, 0 });
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      defender.setVelocity(Vector2{ 0, 0 });
    }

    // DEFENDER LASER FIRING
    if (key->key == ASGE::KEYS::KEY_SPACE &&
        key->action == ASGE::KEYS::KEY_PRESSED && !lasers[0].visibility)
    {
      lasers[current_shot_index].spriteComponent()->getSprite()->xPos(
        defender.spriteComponent()->getSprite()->xPos() +
        defender.spriteComponent()->getSprite()->width() / 2 -
        lasers[current_shot_index].spriteComponent()->getSprite()->width() / 2);

      lasers[current_shot_index].spriteComponent()->getSprite()->yPos(
        defender.spriteComponent()->getSprite()->yPos() -
        lasers[current_shot_index].spriteComponent()->getSprite()->height());

      shoot = true;

      lasers[current_shot_index].visibility = true;
      lasers[current_shot_index].setVelocity(Vector2{ 0, -450 });

      current_shot_index++;
      if (current_shot_index == 5)
      {
        current_shot_index = 0;
      }
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void SpaceInvaders::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

void SpaceInvaders::linearAlienMovement(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;

  for (int i = 0; i < alien_count; i++)
  {
    aliens[i].setVelocity(Vector2{ alien_x_velocity, 0 });
    aliens[i].spriteComponent()->getSprite()->xPos(
      aliens[i].spriteComponent()->getSprite()->xPos() +
      aliens[i].getVelocity().x * dt_sec);

    if (i < 10)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(alien_y_pos);
    }
    else if (i >= 10 && i < 20)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(20 + alien_y_pos);
    }
    else if (i >= 20 && i < 30)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(40 + alien_y_pos);
    }
    else if (i >= 30 && i < 40)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(60 + alien_y_pos);
    }
    else
    {
      aliens[i].spriteComponent()->getSprite()->yPos(80 + alien_y_pos);
    }
  }
}
void SpaceInvaders::gravitationalAlienMovement(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;

  for (int i = 0; i < alien_count; i++)
  {
    aliens[i].setVelocity(Vector2{ alien_x_velocity, alien_y_velocity });

    aliens[i].spriteComponent()->getSprite()->xPos(
      aliens[i].spriteComponent()->getSprite()->xPos() +
      aliens[i].getVelocity().x * dt_sec);

    aliens[i].spriteComponent()->getSprite()->yPos(
      aliens[i].spriteComponent()->getSprite()->yPos() +
      aliens[i].getVelocity().y * dt_sec);

    alien_y_velocity = alien_y_velocity + 4.9 * dt_sec;
  }
}

void SpaceInvaders::quadraticAlienMovement(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;

  // y = 0.001 * pow(x - 640.0, 2.0)

  // SUPER HAPPY FUN TIME CODE
  /*
  for (int i = 0; i < alien_count; i++) {
      aliens[i].setVelocity(Vector2{alien_x_velocity, alien_x_velocity});

      aliens[i].spriteComponent()->getSprite()->xPos(
              aliens[i].spriteComponent()->getSprite()->xPos() +
  aliens[i].getVelocity().x * dt_sec);

      aliens[i].spriteComponent()->getSprite()->yPos(aliens[i].spriteComponent()->getSprite()->yPos()
  + (0.0001 * pow((aliens[i].spriteComponent()->getSprite()->xPos() -
  640.0), 2.0)) * dt_sec);
  }
   */

  for (int i = 0; i < alien_count; i++)
  {
    aliens[i].setVelocity(Vector2{ alien_x_velocity, 0 });

    aliens[i].spriteComponent()->getSprite()->xPos(
      aliens[i].spriteComponent()->getSprite()->xPos() +
      aliens[i].getVelocity().x * dt_sec);

    if (i < 10)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(
        (-0.0002 *
         pow(aliens[i].spriteComponent()->getSprite()->xPos() - 640.0, 2.0)) +
        200);
    }
    else if (i >= 10 && i < 20)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(
        (-0.0002 *
         pow(aliens[i].spriteComponent()->getSprite()->xPos() - 640.0, 2.0)) +
        220);
    }
    else if (i >= 20 && i < 30)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(
        (-0.0002 *
         pow(aliens[i].spriteComponent()->getSprite()->xPos() - 640.0, 2.0)) +
        240);
    }
    else if (i >= 30 && i < 40)
    {
      aliens[i].spriteComponent()->getSprite()->yPos(
        (-0.0002 *
         pow(aliens[i].spriteComponent()->getSprite()->xPos() - 640.0, 2.0)) +
        260);
    }
    else
    {
      aliens[i].spriteComponent()->getSprite()->yPos(
        (-0.0002 *
         pow(aliens[i].spriteComponent()->getSprite()->xPos() - 640.0, 2.0)) +
        280);
    }

    // ASGE::DebugPrinter{} << aliens[i].spriteComponent()->getSprite()->yPos()
    // << std::endl;

    /*if (aliens[4].spriteComponent()->getSprite()->xPos() <= 460)
    {
        aliens[i].spriteComponent()->getSprite()->yPos(aliens[i].spriteComponent()->getSprite()->yPos()
    + ( 0.001 * pow(aliens->spriteComponent()->getSprite()->xPos() -
    460.0, 2.0)) * dt_sec);
    }
    else
    {
        aliens[i].spriteComponent()->getSprite()->yPos(aliens[i].spriteComponent()->getSprite()->yPos()
    - ( 0.001 * pow(aliens->spriteComponent()->getSprite()->xPos() -
    460.0, 2.0)) * dt_sec);
    }*/
  }
}

void SpaceInvaders::sineAlienMovement(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;

  for (int i = 0; i < alien_count; i++)
  {
    aliens[i].setVelocity(Vector2{ alien_x_velocity, 0 });

    aliens[i].spriteComponent()->getSprite()->xPos(
      aliens[i].spriteComponent()->getSprite()->xPos() +
      aliens[i].getVelocity().x * dt_sec);

    aliens[i].spriteComponent()->getSprite()->yPos(
      aliens[i].spriteComponent()->getSprite()->yPos() +
      (50 * sin(0.01 * aliens->spriteComponent()->getSprite()->xPos())) *
        dt_sec);

    // SUPER HAPPY FUN TIME CODE v2
    /*
    aliens[i].spriteComponent()->getSprite()->yPos(
      aliens[i].spriteComponent()->getSprite()->yPos() +
      (50 * sin(0.01 * aliens[i].spriteComponent()->getSprite()->xPos())) *
        dt_sec);
        */
  }
}

void SpaceInvaders::alienMovement(const ASGE::GameTime& game_time)
{
  if (aliens[0].spriteComponent()->getSprite()->xPos() <= 0 ||
      aliens[9].spriteComponent()->getSprite()->xPos() +
          aliens[9].spriteComponent()->getSprite()->width() >=
        game_width)
  {
    alien_x_velocity *= -1;
    alien_y_pos += aliens->spriteComponent()->getSprite()->height();
  }

  if (menu_option == 0)
  {
    linearAlienMovement(game_time);
  }
  else if (menu_option == 1)
  {
    gravitationalAlienMovement(game_time);
  }
  else if (menu_option == 2)
  {
    quadraticAlienMovement(game_time);
  }
  else if (menu_option == 3)
  {
    sineAlienMovement(game_time);
  }
}

bool SpaceInvaders::isOverlapping(ASGE::Sprite* sprite1, ASGE::Sprite* sprite2)
{
  if ((sprite2->xPos() <= sprite1->xPos() + sprite1->width()) &&
      (sprite2->xPos() + sprite2->width() >= sprite1->xPos()) &&

      (sprite2->yPos() <= sprite1->yPos() + sprite1->height()) &&
      (sprite2->yPos() + sprite2->height() >= sprite1->yPos()))
  {
    return true;
  }
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvaders::update(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;
  // make sure you use delta time in any movement calculations!

  if (in_game)
  {
    defender.spriteComponent()->getSprite()->xPos(
      defender.spriteComponent()->getSprite()->xPos() +
      (defender.getVelocity().x * dt_sec));

    alienMovement(game_time);

    for (int i = 0; i < laser_count; i++)
    {
      if (lasers[i].visibility)
      {
        lasers[i].spriteComponent()->getSprite()->yPos(
          lasers[i].spriteComponent()->getSprite()->yPos() +
          lasers[i].getVelocity().y * dt_sec);
      }
      if (lasers[i].spriteComponent()->getSprite()->yPos() +
            lasers[i].spriteComponent()->getSprite()->height() <=
          0)
      {
        lasers[i].visibility = false;
      }
    }

    for (int i = 0; i < alien_count; i++)
    {
      for (int j = 0; j < laser_count; j++)
      {
        if (isOverlapping(lasers[j].spriteComponent()->getSprite(),
                          aliens[i].spriteComponent()->getSprite()) &&
            aliens[i].visibility && lasers[j].visibility)
        {
          // ASGE::DebugPrinter{} << "HIT" << std::endl;
          lasers[j].visibility = false;
          aliens[i].visibility = false;
          aliens_left--;
          // ASGE::DebugPrinter{} << "aliens left: " << aliens_left <<
          // std::endl;
          score += 10;
        }
      }

      for (int i = 0; i < alien_count; i++)
      {
        if (aliens[i].spriteComponent()->getSprite()->yPos() +
              aliens[i].spriteComponent()->getSprite()->height() >=
            defender.spriteComponent()->getSprite()->yPos())
        {
          in_game = false;
          lose = true;
        }
      }

      if (aliens_left == 0)
      {
        in_game = false;
        win = true;
      }
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */

/*void renderMenuScreen(const ASGE::GameTime&)
{

}
void renderGameScreen(const ASGE::GameTime&)
{

}
void renderPauseScreen(const ASGE::GameTime&)
{

}*/

void SpaceInvaders::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  /*renderMenuScreen();
  renderGameScreen();
  renderPauseScreen();*/

  if (in_menu)
  {
    renderer->renderText("MENU", game_width / 2, 40, 1.0, ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 0 ? ">STRAIGHT LINE" : "STRAIGHT LINE",
                         (game_width / 2.0f),
                         (game_height * 0.5f),
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 1 ? ">GRAVITY CURVE" : "GRAVITY CURVE",
                         (game_width / 2.0f),
                         (game_height * 0.6f),
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 2 ? ">QUADRATIC CURVE"
                                          : "QUADRATIC CURVE",
                         (game_width / 2.0f),
                         (game_height * 0.7f),
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 3 ? ">SINE CURVE" : "SINE CURVE",
                         (game_width / 2.0f),
                         (game_height * 0.8f),
                         1.0,
                         ASGE::COLOURS::WHITE);
  }

  if (in_game)
  {
    // renderer->renderText("IN GAME", game_width / 2, game_height / 2, 1.0,
    // ASGE::COLOURS::WHITE);

    renderer->renderSprite(*defender.spriteComponent()->getSprite());

    for (int i = 0; i < alien_count; i++)
    {
      if (aliens[i].visibility)
      {
        renderer->renderSprite(*aliens[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < laser_count; i++)
    {
      if (lasers[i].visibility)
      {
        renderer->renderSprite(*lasers[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < barrier_count; i++)
    {
      renderer->renderSprite(*barriers[i].spriteComponent()->getSprite());
    }

    renderer->renderText("SCORE: " + std::to_string(score),
                         game_width - 110,
                         game_height - 6,
                         1.0,
                         ASGE::COLOURS::WHITE);
  }

  if (in_pause)
  {
    renderer->renderText(
      "PAUSED", game_width / 2, game_height / 2, 1.0, ASGE::COLOURS::WHITE);
  }

  if (win)
  {
    renderer->renderText(
      "WIN", game_width / 2, game_height / 2, 1.0, ASGE::COLOURS::WHITE);
  }
  if (lose)
  {
    renderer->renderText("GAME OVER",
                         (game_width / 2.0) - 30.0,
                         earth.spriteComponent()->getSprite()->yPos() - 20.0,
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderSprite(*earth.spriteComponent()->getSprite());
  }
}
