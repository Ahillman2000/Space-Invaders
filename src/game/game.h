#pragma once
#include "Utility/Vector2.h"
#include <Engine/OGLGame.h>
#include <string>

#include "GameObjects/GameObject.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class SpaceInvaders : public ASGE::OGLGame
{
 public:
  SpaceInvaders();
  ~SpaceInvaders() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void setupResolution();

  bool isOverlapping(ASGE::Sprite*, ASGE::Sprite*);

  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;

  /*void renderMenuScreen(const ASGE::GameTime&);
  void renderGameScreen(const ASGE::GameTime&);
  void renderPauseScreen(const ASGE::GameTime&);*/

  int alien_count = 50;
  int aliens_left = alien_count;
  int laser_count = 5;
  int barrier_count = 36;

  int current_shot_index = 0;

  bool initDefender();
  GameObject defender;
  bool initAliens();
  GameObject aliens[50];
  bool initLasers();
  GameObject lasers[5];
  bool initBarriers();
  GameObject barriers[36];
  bool initEarth();
  GameObject earth;

  void linearAlienMovement(const ASGE::GameTime& game_time);
  void gravitationalAlienMovement(const ASGE::GameTime& game_time);
  void quadraticAlienMovement(const ASGE::GameTime& game_time);
  void sineAlienMovement(const ASGE::GameTime& game_time);

  void alienMovement(const ASGE::GameTime& game_time);
  const float GRAVITY = 9.8;
  float alien_x_velocity = 200;
  float alien_y_velocity = 0;
  float alien_y_pos = 20;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  int menu_option = 0;

  bool in_menu = true;
  bool in_game = false;
  bool in_pause = false;

  bool shoot = false;
  int score = 0;

  bool win = false;
  bool lose = false;
};