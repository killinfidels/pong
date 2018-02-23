#include "header.h"

bool eventLoop()
{
	SDL_Event e;

	//event loop
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return true;
		}

		//menuitem eventhandlers check for if their object has been clicked
		mPlay.eventHandler(&e);
		mOptions.eventHandler(&e);
		mExit.eventHandler(&e);

		pUnpause.eventHandler(&e);
		pRestart.eventHandler(&e);
		pReturn.eventHandler(&e);

		oWeeb.eventHandler(&e);

		ball.eventHandler(&e);

		switch (e.type) //if a key has been pressed or let go the keyHandler stores the keys state
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyHandler.handleKeyboardEvent(&e);
			break;
		default:
			break;
		}
	}
	return false;
}

void stateChanger()
{
	switch (currentState)
	{
	case MAINMENU:
		if (mPlay.activated()) //if mPlay is clicked
		{
			currentState = RESTART; //switches state to PLAYING
		}

		if (mOptions.activated())
		{
			currentState = OPTIONS;
		}

		if (keyHandler.isPressed(SDLK_RETURN)) //if enter is pressed
		{
			currentState = PLAYING;
		}
		break;
	case PLAYING:
		if (keyHandler.isPressed(SDLK_ESCAPE))
		{
			currentState = PAUSED;
		}
		break;
	case PAUSED:
		if (pUnpause.activated())
		{
			currentState = PLAYING;
		}

		if (pRestart.activated())
		{
			currentState = RESTART;
		}

		if (pReturn.activated())
		{
			currentState = MAINMENU;
		}

		/*if (keyHandler.isPressed(SDLK_ESCAPE))
		{
			currentState = PLAYING;
		}*/
		break;
	case OPTIONS:
		if (keyHandler.isPressed(SDLK_ESCAPE))
		{
			currentState = MAINMENU;
		}
		break;
	case RESTART:
		currentState = PLAYING;
		break;
	default:
		break;
	}
}

//giving objects textures
void setTextures()
{
	winTextTexture.createTextureFromText(renderer, arial, "yeee haww, cowboyy, you did it!!!!!", Red); //creates win text from string

	for (int i = 0; i < 11; i++) //creates textures for numbers 0 to 10 and stores them in the countNumbers array
	{
		if (i < 10) //adds an extra zero infront of numbers less than 10
			countNumbers[i].createTextureFromText(renderer, arial, "0" + std::to_string(i), LightBlue);
		else
			countNumbers[i].createTextureFromText(renderer, arial, std::to_string(i), LightBlue);
	}

	//menu items
	mPlay.setTextures(tPlay.getTexture(), tPlay.getTexture());
	mOptions.setTextures(tOptions.getTexture(), tOptions.getTexture());
	mExit.setTextures(tExit.getTexture(), tExit.getTexture());

	pUnpause.setTextures(tUnpause.getTexture(), tUnpause.getTexture());
	pRestart.setTextures(tRestart.getTexture(), tRestart.getTexture());
	pReturn.setTextures(tReturn.getTexture(), tReturn.getTexture());

	//oWeeb.setTextures(tWeebOff.getTexture(), tWeebOn.getTexture());

	//players
	player1.setTexture(paddle.getTexture());
	player2.setTexture(paddle.getTexture());

	//balls
	ball.setTextures(circle.getTexture(), glarpball.getTexture()); //setting the balls default texture \circle texture

										  //text
	counter1.setTexture(countNumbers[0].getTexture()); //setting the counters default texture \number 0 texture
	counter2.setTexture(countNumbers[0].getTexture()); //setting the counters default texture \number 0 texture
	winText.setTexture(winTextTexture.getTexture());
}

int main(int argc, char* args[])
{
	if (!initialized)
	{
		SDL_Delay(5000); //waits five seconds so you can see why it failed to init
	}
	else
	{
		//quit flag
		bool quit = false;

		//object positions
		counter1.rect.x = 480;
		counter2.rect.x = 720;

		winText.rect.x = screen_width / 2 - winText.rect.w / 2;
		winText.rect.y = screen_height / 2;

		//ball settings
		ball.dontSlowDown = true;
		ball.setVelocityX(-5);
		ball.setVelocityY(-1);

		//sets all object textures
		setTextures();
		
		//counters for the score
		int p1 = 0, p2 = 0;

		//flags
		const int states = 4;
		bool doneBefore[states] = { false, false, false, false };
		bool won = false;
		bool weebMode = false;
		bool click = false, click2 = false;

		//game loop
		while (!quit)
		{
			stateChanger();

			quit = eventLoop();

			SDL_RenderClear(renderer); //clears the screen before starting to render the frame

			switch (currentState)
			{
			case MAINMENU: //runs when the gamestate is MAINMENU

				if (!doneBefore[MAINMENU])
				{
					for (int i = 0; i < states; i++)
					{
						if (i == MAINMENU)
						{
							screen.setTexture(mainmenu.getTexture()); //sets the screens texture to the mainmenu
							doneBefore[i] = true;
						}
						else
						{
							doneBefore[i] = false;
						}
					}
				}
				
				screen.draw(renderer); //draws the screens texture
				mPlay.draw(renderer);
				mOptions.draw(renderer);
				mExit.draw(renderer);

				mOptions.activated(); //if mOptions is clicked

				if (mExit.activated() && !click2) //if exit is clicked
					quit = true;
				else if (!mExit.activated())
					click2 = false;

				break;
				
			case PLAYING: //runs when the gamestate is PLAYING
				if (!doneBefore[PLAYING])
				{
					for (int i = 0; i < states; i++)
					{
						if (i == PLAYING)
						{
							if (weebMode)
								screen.setTexture(jojo.getTexture());
							else
								screen.setTexture(gamemap.getTexture()); //sets the screens texture to the game map
							
							doneBefore[i] = true;
						}
						else
						{
							doneBefore[i] = false;
						}
					}

				}

				screen.draw(renderer); //draws the screens texture

				SDL_RenderDrawLine(renderer, screen_width / 2, 0, screen_width / 2, screen_height); //draws going verticaly through the center

				//cheats
				{ 
					if (keyHandler.isPressed(SDLK_1) && p1 != 0) {
						p1--;
						counter1.setTexture(countNumbers[p1].getTexture());
					}

					if (keyHandler.isPressed(SDLK_2) && p1 != 10) {
						p1++;
						counter1.setTexture(countNumbers[p1].getTexture());
					}
				}

				if (!won)
				{
					//up, down movement for player1
					if (keyHandler.isPressed(SDLK_w))
						player1.move(player1.UP);
					if (keyHandler.isPressed(SDLK_s))
						player1.move(player1.DOWN);

					//up, down movement for player2
					if (keyHandler.isPressed(SDLK_UP))
						player2.move(player2.UP);
					if (keyHandler.isPressed(SDLK_DOWN))
						player2.move(player2.DOWN);

					//updates where players are gonna be rendered after movement
				player1.update();
				player2.update();

				switch (ball.ballCollision(screen.rect, true)) //checks if the ball collided with the screen
				{
				case 1: //if it collides with the right side

				//increments player ones score
				{	
					p1++;
					if (p1 > 10)
						p1 = 10;
				}

				//respawns the ball
				{ 
					ball.setVelocityX(-5);
					ball.setVelocityY(-1);
					ball.rect.x = screen_width / 2 - ball.rect.w / 2;
					ball.rect.y = screen_height / 2 - ball.rect.h / 2;
				}

					counter1.setTexture(countNumbers[p1].getTexture()); //switches counter1 texture to the next number

					break;
				case 2: //if it collides with the left side

				//increments player twos score
				{
					p2++;
					if (p2 > 10)
						p2 = 10;
				}
					
				//respawns the ball
				{ 
					ball.setVelocityX(5);
					ball.setVelocityY(-1);
					ball.rect.x = screen_width / 2 - ball.rect.w / 2;
					ball.rect.y = screen_height / 2 - ball.rect.h / 2;
				}

					counter2.setTexture(countNumbers[p2].getTexture()); //switches counter2 texture to the next number

					break;
				default:
					break;
				}

				ball.ballCollision(player1.rect, false);
				ball.ballCollision(player2.rect, false);

				ball.update();

				player1.collision(screen.rect, true);
				player2.collision(screen.rect, true);

				if (p1 == 10 || p2 == 10)
					won = true;

				}
				

				

				player1.draw(renderer);
				player2.draw(renderer);

				counter1.draw(renderer);
				counter2.draw(renderer);


				if (won)
					winText.draw(renderer);
				else
					ball.draw(renderer);


				break;
			case PAUSED:
				if (!doneBefore[PAUSED])
				{
					for (int i = 0; i < states; i++)
					{
						if (i == PAUSED)
						{
							screen.setTexture(pausemenu.getTexture()); //sets the screens texture to the game map
							doneBefore[i] = true;
						}
						else
						{
							doneBefore[i] = false;
						}
					}
				}
				screen.draw(renderer);

				pUnpause.draw(renderer);
				pRestart.draw(renderer);
				pReturn.draw(renderer);

				if (pReturn.activated()) //if return is clicked
					click2 = true;
				break;
			case OPTIONS:
				if (!doneBefore[OPTIONS])
				{
					for (int i = 0; i < states; i++)
					{
						if (i == OPTIONS)
						{
							screen.setTexture(optionsmenu.getTexture()); //sets the screens texture to the game map
							doneBefore[i] = true;
						}
						else
						{
							doneBefore[i] = false;
						}
					}
				}

				screen.draw(renderer);

				if (click)
					if (!oWeeb.activated())
						weebMode = !weebMode;

				if (oWeeb.activated())
					click = true;
				else
					click = false;

				if (weebMode)
					oWeeb.setTextures(tWeebOn.getTexture(), tWeebOff.getTexture());
				else if (!weebMode)
					oWeeb.setTextures(tWeebOff.getTexture(), tWeebOn.getTexture());
				
				oWeeb.draw(renderer);
				
				break;
			case RESTART:
				ball.setVelocityX(-5);
				ball.setVelocityY(-1);
				ball.rect.x = screen_width / 2 - ball.rect.w / 2;
				ball.rect.y = screen_height / 2 - ball.rect.h / 2;

				player1.rect.y = screen_height / 2 - player1.rect.h / 2;
				player2.rect.y = screen_height / 2 - player2.rect.h / 2;

				won = false;
				p1 = 0, p2 = 0;

				counter1.setTexture(countNumbers[p1].getTexture());
				counter2.setTexture(countNumbers[p1].getTexture());
				break;
			default:
				break;
			}

			SDL_RenderPresent(renderer);

			setFPS(60);
		}
	}

	close();

	return 0;
}