#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TransformNode.h"


int main(int argc, char *argv)
{
  sf::RenderWindow window(sf::VideoMode(1200, 900), "Project 1 - Windmills", sf::Style::Titlebar | sf::Style::Close);

  sf::Event evt;
  sf::Clock appTimer;
  sf::Vector2<float>bladeDimesion = { 5.0f, 21.0f };

  bool millOneDefaultState = true; //only for first running the program, mill one will respondes to mouse movement by default, until number keys are pressed 

  //for key pressed decetion, press another number key (except 4 and self) should deactivate the current mill's mouse related movements
  bool NumberOneMillActivated = false; 
  bool NumberTwoMillActivated = false; 
  bool NumberThreeMillActivated = false;
  bool AllMillActivated = false; //for number 4 key

  int mouseX, mouseY; //for recording mouse movements
  int mouseXBeforeMove, mouseYBeforeMove, mouseXDifference, mouseYDifference;
  int millOneAnchorAngle, millOneBodyAngle, millTwoAnchorAngle, millTwoBodyAngle, millThreeAnchorAngle, millThreeBodyAngle;
  int soundTracklooper = 0;

  float cloudTimer = 0.0f; // elapsed time per second.
  float CloudmovementSpeed = 150.0f; // cloud scrolling movement speed.

  //define shapes for windMill one
  sf::CircleShape windMillOneUpperBody(20, 3);
  sf::CircleShape windMillOneAxel(3);
  sf::RectangleShape windMillOneBase(sf::Vector2<float>(windMillOneUpperBody.getLocalBounds().width, 8.0f));
  sf::RectangleShape windMillOneBladeBottom(bladeDimesion);
  sf::RectangleShape windMillOneBladeLeft(bladeDimesion);
  sf::RectangleShape windMillOneBladeRight(bladeDimesion);
  sf::RectangleShape windMillOneBladeTop(bladeDimesion);
  sf::CircleShape windMillOneRotationAnchor(1, 1); //for rotation of all windmills

  //define shapes for windMill two
  sf::CircleShape windMillTwoUpperBody(20, 3);
  sf::CircleShape windMillTwoAxel(3);
  sf::RectangleShape windMillTwoBase(sf::Vector2<float>(windMillOneUpperBody.getLocalBounds().width, 8.0f));
  sf::RectangleShape windMillTwoBladeBottom(bladeDimesion);
  sf::RectangleShape windMillTwoBladeLeft(bladeDimesion);
  sf::RectangleShape windMillTwoBladeRight(bladeDimesion);
  sf::RectangleShape windMillTwoBladeTop(bladeDimesion);
  sf::CircleShape windMillTwoRotationAnchor(1, 1); //for rotation of all windmills

  //define shapes for windMill three
  sf::CircleShape windMillThreeUpperBody(20, 3);
  sf::CircleShape windMillThreeAxel(3);
  sf::RectangleShape windMillThreeBase(sf::Vector2<float>(windMillOneUpperBody.getLocalBounds().width, 8.0f));
  sf::RectangleShape windMillThreeBladeBottom(bladeDimesion);
  sf::RectangleShape windMillThreeBladeLeft(bladeDimesion);
  sf::RectangleShape windMillThreeBladeRight(bladeDimesion);
  sf::RectangleShape windMillThreeBladeTop(bladeDimesion);
  sf::CircleShape windMillThreeRotationAnchor(1,1); //anchor for mill three's rotation around another point 

  //Loading Sound
  sf::SoundBuffer buffer;
  buffer.loadFromFile("res/wind.wav");
  sf::Sound sound(buffer);
							   
  //load textures
  sf::Texture windMillBodyTexture;
  windMillBodyTexture.loadFromFile("res/stone.jpg");

  sf::Texture windMillAxelTexture;
  windMillAxelTexture.loadFromFile("res/metalAxel.jpg");

  sf::Texture windMillBaseTexture;
  windMillBaseTexture.loadFromFile("res/stoneBase.jpg");

  sf::Texture windMillBladeTexture;
  windMillBladeTexture.loadFromFile("res/wood.jpg");

  sf::Texture cloudTexture;
  cloudTexture.loadFromFile("res/cloud.jpg");
  cloudTexture.setRepeated(true);

  //setup cloud
  sf::Sprite cloudSprite;
  cloudSprite.setTexture(cloudTexture);
  cloudSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
  sf::IntRect backgroundRect = cloudSprite.getTextureRect();
 
  //set textures for windmill one
  windMillOneUpperBody.setTexture(&windMillBodyTexture);
  windMillOneAxel.setTexture(&windMillAxelTexture);
  windMillOneBase.setTexture(&windMillBaseTexture);
  windMillOneBladeBottom.setTexture(&windMillBladeTexture);
  windMillOneBladeLeft.setTexture(&windMillBladeTexture);
  windMillOneBladeRight.setTexture(&windMillBladeTexture);
  windMillOneBladeTop.setTexture(&windMillBladeTexture);

  //set textures for windmill two
  windMillTwoUpperBody.setTexture(&windMillBodyTexture);
  windMillTwoAxel.setTexture(&windMillAxelTexture);
  windMillTwoBase.setTexture(&windMillBaseTexture);
  windMillTwoBladeBottom.setTexture(&windMillBladeTexture);
  windMillTwoBladeLeft.setTexture(&windMillBladeTexture);
  windMillTwoBladeRight.setTexture(&windMillBladeTexture);
  windMillTwoBladeTop.setTexture(&windMillBladeTexture);

  //set textures for windmill three
  windMillThreeUpperBody.setTexture(&windMillBodyTexture);
  windMillThreeAxel.setTexture(&windMillAxelTexture);
  windMillThreeBase.setTexture(&windMillBaseTexture);
  windMillThreeBladeBottom.setTexture(&windMillBladeTexture);
  windMillThreeBladeLeft.setTexture(&windMillBladeTexture);
  windMillThreeBladeRight.setTexture(&windMillBladeTexture);
  windMillThreeBladeTop.setTexture(&windMillBladeTexture);

  //set windmill one upperbody origin
  windMillOneUpperBody.setOrigin(20.0f, 20.0f);
  windMillOneUpperBody.setPosition(150, 100);
  windMillOneUpperBody.setScale(3, 3);
  windMillOneRotationAnchor.setOrigin(1.0f, 1.0f);
  windMillOneRotationAnchor.setPosition(200, 450); //a point to the left of mill one

  //set windmill two upperbody origin
  windMillTwoUpperBody.setOrigin(20.0f, 20.0f);
  windMillTwoUpperBody.setPosition(250, 100);
  windMillTwoUpperBody.setScale(3, 3);
  windMillTwoRotationAnchor.setOrigin(1.0f, 1.0f);
  windMillTwoRotationAnchor.setPosition(380, 450); //mill one's position

  //set windmill three upperbody origin
  windMillThreeUpperBody.setOrigin(20.0f, 20.0f);
  windMillThreeUpperBody.setPosition(250, 100);
  windMillThreeUpperBody.setScale(3, 3);
  windMillThreeRotationAnchor.setOrigin(1.0f, 1.0f);
  windMillThreeRotationAnchor.setPosition(630, 450); //mill two's location
 
  //define windmill axel position and origin
  sf::Vector2<float> axelPosition((windMillOneUpperBody.getLocalBounds().width * (-0.005f)), (windMillOneUpperBody.getLocalBounds().height * (-0.45f)));
  sf::Vector2<float> axelOrigin(3.0f, 3.0f);

  //define windmill base position and origin
  sf::Vector2<float> basePosition((windMillOneUpperBody.getLocalBounds().width *(-0.46f)),(windMillOneUpperBody.getLocalBounds().height /2.7f));
  sf::Vector2<float> baseOrigin(1.0f, 1.0f);

  //define windmill bottom blade position and origin
  sf::Vector2<float> bladeBottomPosition((windMillOneUpperBody.getLocalBounds().width * (-0.1f)), (windMillOneUpperBody.getLocalBounds().height * (-0.68f)));
  sf::Vector2<float> bladeBottomOrigin(0.0f, 3.0f);

  //define windmill left blade position and origin
  sf::Vector2<float> bladeLeftPosition((windMillOneUpperBody.getLocalBounds().width / 20.0f), (windMillOneUpperBody.getLocalBounds().height / 10.0f));
  sf::Vector2<float> bladeLeftOrigin(0.0f, 0.0f);

  //define windmill right blade position and origin
  sf::Vector2<float> bladeRightPosition((windMillOneUpperBody.getLocalBounds().width * (-0.06f)), (windMillOneUpperBody.getLocalBounds().height * (0.7f)));
  sf::Vector2<float> bladeRightOrigin(22.0f, 0.0f);

  //define windmill top blade position and origin
  sf::Vector2<float> bladeTopPosition((windMillOneUpperBody.getLocalBounds().width * (-0.01f)), (windMillOneUpperBody.getLocalBounds().height / 15.0f));
  sf::Vector2<float> bladeTopOrigin(3.0f,0.0f);

  //set windmill one axel origin and position
  windMillOneAxel.setOrigin(axelOrigin);
  windMillOneAxel.setPosition(axelPosition);

  //set windmill one base origin and position
  windMillOneBase.setOrigin(baseOrigin);
  windMillOneBase.setPosition(basePosition);

  //set windmill two axel origin and position
  windMillTwoAxel.setOrigin(axelOrigin);
  windMillTwoAxel.setPosition(axelPosition);
 
  //set windmill two base origin and position
  windMillTwoBase.setOrigin(baseOrigin);
  windMillTwoBase.setPosition(basePosition);

  //set windmill three axel origin and position
  windMillThreeAxel.setOrigin(axelOrigin);
  windMillThreeAxel.setPosition(axelPosition);

  //set windmill three base origin and position
  windMillThreeBase.setOrigin(baseOrigin);
  windMillThreeBase.setPosition(basePosition);

  //set windmill one's blades origin and position
  windMillOneBladeBottom.setOrigin(bladeBottomOrigin);
  windMillOneBladeBottom.setPosition(bladeBottomPosition);
  windMillOneBladeLeft.setOrigin(bladeLeftOrigin);
  windMillOneBladeLeft.setPosition(bladeLeftPosition);
  windMillOneBladeRight.setOrigin(bladeRightOrigin);
  windMillOneBladeRight.setPosition(bladeRightPosition);
  windMillOneBladeTop.setOrigin(bladeTopOrigin);
  windMillOneBladeTop.setPosition(bladeTopPosition);

  //set windmill two's blades origin and position
  windMillTwoBladeBottom.setOrigin(bladeBottomOrigin);
  windMillTwoBladeBottom.setPosition(bladeBottomPosition);
  windMillTwoBladeLeft.setOrigin(bladeLeftOrigin);
  windMillTwoBladeLeft.setPosition(bladeLeftPosition);
  windMillTwoBladeRight.setOrigin(bladeRightOrigin);
  windMillTwoBladeRight.setPosition(bladeRightPosition);
  windMillTwoBladeTop.setOrigin(bladeTopOrigin);
  windMillTwoBladeTop.setPosition(bladeTopPosition);

  //set windmill three's blades origin and position
  windMillThreeBladeBottom.setOrigin(bladeBottomOrigin);
  windMillThreeBladeBottom.setPosition(bladeBottomPosition);
  windMillThreeBladeLeft.setOrigin(bladeLeftOrigin);
  windMillThreeBladeLeft.setPosition(bladeLeftPosition);
  windMillThreeBladeRight.setOrigin(bladeRightOrigin);
  windMillThreeBladeRight.setPosition(bladeRightPosition);
  windMillThreeBladeTop.setOrigin(bladeTopOrigin);
  windMillThreeBladeTop.setPosition(bladeTopPosition);

  //add windmill one parts to transform node
  TransformNode millOneUpper(&windMillOneUpperBody);
  TransformNode millOneAxel(&windMillOneAxel);
  TransformNode millOneBase(&windMillOneBase);
  TransformNode millOneBladeBottom(&windMillOneBladeBottom);
  TransformNode millOneBladeLeft(&windMillOneBladeLeft);
  TransformNode millOneBladeRight(&windMillOneBladeRight);
  TransformNode millOneBladeTop(&windMillOneBladeTop);
  TransformNode millOneRotationAnchor(&windMillOneRotationAnchor);

  //add windmill two parts to transform node
  TransformNode millTwoUpper(&windMillTwoUpperBody);
  TransformNode millTwoAxel(&windMillTwoAxel);
  TransformNode millTwoBase(&windMillTwoBase);
  TransformNode millTwoBladeBottom(&windMillTwoBladeBottom);
  TransformNode millTwoBladeLeft(&windMillTwoBladeLeft);
  TransformNode millTwoBladeRight(&windMillTwoBladeRight);
  TransformNode millTwoBladeTop(&windMillTwoBladeTop);
  TransformNode millTwoRotationAnchor(&windMillTwoRotationAnchor);

  //add windmill three parts to transform node
  TransformNode millThreeUpper(&windMillThreeUpperBody);
  TransformNode millThreeAxel(&windMillThreeAxel);
  TransformNode millThreeBase(&windMillThreeBase);
  TransformNode millThreeBladeBottom(&windMillThreeBladeBottom);
  TransformNode millThreeBladeLeft(&windMillThreeBladeLeft);
  TransformNode millThreeBladeRight(&windMillThreeBladeRight);
  TransformNode millThreeBladeTop(&windMillThreeBladeTop);
  TransformNode millThreeRotationAnchor(&windMillThreeRotationAnchor);

  //Add child and parent releationship for windmill one
  millOneUpper.AddChild(&millOneAxel);
  millOneAxel.AddChild(&millOneBladeBottom);
  millOneAxel.AddChild(&millOneBladeTop);
  millOneAxel.AddChild(&millOneBladeLeft);
  millOneAxel.AddChild(&millOneBladeRight);
  millOneUpper.AddChild(&millOneBase);
  millOneRotationAnchor.AddChild(&millOneUpper); //only to be drawn if all mills are moving

  //Add child and parent releationship for windmill two
  millTwoUpper.AddChild(&millTwoAxel);
  millTwoAxel.AddChild(&millTwoBladeBottom);
  millTwoAxel.AddChild(&millTwoBladeTop);
  millTwoAxel.AddChild(&millTwoBladeLeft);
  millTwoAxel.AddChild(&millTwoBladeRight);
  millTwoUpper.AddChild(&millTwoBase);
  millTwoRotationAnchor.AddChild(&millTwoUpper); //only to be drawn if all mills are moving

  //Add child and parent releationship for windmill three
  millThreeUpper.AddChild(&millThreeAxel);
  millThreeAxel.AddChild(&millThreeBladeBottom);
  millThreeAxel.AddChild(&millThreeBladeTop);
  millThreeAxel.AddChild(&millThreeBladeLeft);
  millThreeAxel.AddChild(&millThreeBladeRight);
  millThreeUpper.AddChild(&millThreeBase);
  millThreeRotationAnchor.AddChild(&millThreeUpper);

  //set windmill one blades' default angle
  windMillOneBladeLeft.rotate(-90);
  windMillOneBladeRight.rotate(90);

  //set windmill two blades' default angle
  windMillTwoBladeLeft.rotate(-90);
  windMillTwoBladeRight.rotate(90);

  //set windmill three blades' default angle
  windMillThreeBladeLeft.rotate(-90);
  windMillThreeBladeRight.rotate(90);
 
  //set rotation for windmill one's anchor and body
  millOneAnchorAngle = 325;
  millOneBodyAngle = 35;

  //set rotation for windmill two's anchor and body
  millTwoAnchorAngle = 338;
  millTwoBodyAngle = 22;

  //set rotation for windmill three's anchor and body
  millThreeAnchorAngle = 338;
  millThreeBodyAngle = 22;

  //set default rotation for mill anchor and body
  windMillOneUpperBody.setRotation(millOneBodyAngle);
  windMillOneRotationAnchor.setRotation(millOneAnchorAngle);
  
  windMillTwoUpperBody.setRotation(millTwoBodyAngle);
  windMillTwoRotationAnchor.setRotation(millTwoAnchorAngle);

  windMillThreeUpperBody.setRotation(millThreeBodyAngle);
  windMillThreeRotationAnchor.setRotation(millThreeAnchorAngle);

  sound.play();

  while (window.isOpen()) {  
    float deltaTime = appTimer.restart().asSeconds();	
	float baseRotationAngle = 90.0f;
	float timeBasedRotation = baseRotationAngle * deltaTime;

	//loop the sound effect
	soundTracklooper == 7000 ? soundTracklooper = 0 : soundTracklooper++;

	if(soundTracklooper == 7000)
		sound.play();

	//setup cloud scrolling
	sf::IntRect deltaRect = backgroundRect;
	deltaRect.left = backgroundRect.left - (int)(CloudmovementSpeed * cloudTimer);
	cloudSprite.setTextureRect(deltaRect);

	cloudTimer += deltaTime;

	if (cloudTimer > 1.0f)
	{		
		cloudTimer -= 1.0f;
		backgroundRect.left -= (int)CloudmovementSpeed; 
												  
		if (backgroundRect.left >= (float)cloudTexture.getSize().x) {
			backgroundRect.left -= cloudTexture.getSize().x;
		}
	}

	mouseXBeforeMove = sf::Mouse::getPosition().x;
	mouseYBeforeMove = sf::Mouse::getPosition().y;
	mouseX = mouseXBeforeMove;
	mouseY = mouseYBeforeMove;	

	//Event handling for key presses 
    while (window.pollEvent(evt)) {
		switch (evt.type) 
		{
		 case sf::Event::KeyPressed:
			 if (evt.key.code == sf::Keyboard::Key::Num1) //number 1 key press
			 {				 
				 //if number one mill is not runing then make it the active one
				 //deactive the rest of the mills, as well as number 4 option
				 if (!NumberOneMillActivated) 
				 {
					 NumberOneMillActivated = true;					 
					 NumberTwoMillActivated = false;
					 NumberThreeMillActivated = false;
					 AllMillActivated = false; 
					 millOneDefaultState = false;
				 }					 
			 }
			 else if (evt.key.code == sf::Keyboard::Key::Num2) //number 2 key press
			 {				 
			    //if number two mill is not runing then make it the active one
			    //deactive the rest of the mills, as well as number 4 option
				 if (!NumberTwoMillActivated)
				 {
					 NumberTwoMillActivated = true;					 
					 NumberOneMillActivated = false;
					 NumberThreeMillActivated = false;
					 AllMillActivated = false;
					 millOneDefaultState = false;
				 }
			 }
			 else if (evt.key.code == sf::Keyboard::Key::Num3) //number 3 key press
			 {			 
				//if number three mill is not runing then make it the active one
				//deactive the rest of the mills, as well as number 4 option
				 if (!NumberThreeMillActivated)
				 {
					 NumberThreeMillActivated = true;					
					 NumberTwoMillActivated = false;
					 NumberOneMillActivated = false;
					 AllMillActivated = false;
					 millOneDefaultState = false;
				 }
			 }

			 else if (evt.key.code == sf::Keyboard::Key::Num4 || evt.key.code == sf::Keyboard::Key::A) //number 4 or 'a' key press
			 {				
				//All mill should be activated
				//deactivate individual options
				 if (!AllMillActivated)
				 {
					 AllMillActivated = true;				 
					 NumberTwoMillActivated = false;
					 NumberOneMillActivated = false;
					 NumberThreeMillActivated = false;
					 millOneDefaultState = false;
				 }
				 //reset all windmill's rotation
				 windMillOneUpperBody.setRotation(millOneBodyAngle);
				 windMillOneRotationAnchor.setRotation(millOneAnchorAngle);
				 windMillTwoUpperBody.setRotation(millTwoBodyAngle);
				 windMillTwoRotationAnchor.setRotation(millTwoAnchorAngle);
				 windMillThreeUpperBody.setRotation(millThreeBodyAngle);
				 windMillThreeRotationAnchor.setRotation(millThreeAnchorAngle);
			 }
		  break;
         
		  // Track mouse movements, use global mouse position for easier calculation
		 case sf::Event::MouseMoved:
			 mouseX = sf::Mouse::getPosition().x;
			 mouseY = sf::Mouse::getPosition().y;		 
			 break;

		 case sf::Event::Closed:
			 window.close(); //close window 
			 break;
		 default:
			 break;
		}
    }

	//get mouse movement difference 
	mouseXDifference = mouseXBeforeMove - mouseX;
	mouseYDifference = mouseYBeforeMove - mouseY;

	//blades always roates regardless of key selection			
	windMillOneAxel.rotate(timeBasedRotation);
	windMillTwoAxel.rotate(timeBasedRotation);
	windMillThreeAxel.rotate(timeBasedRotation);

	//rotate each or all windmills accroding to option selected
	if (NumberOneMillActivated || millOneDefaultState)
	{
			windMillOneUpperBody.rotate(mouseXDifference * 10 * -1);		
			windMillOneRotationAnchor.rotate(mouseYDifference * 9 * -1);
	}
	else if (NumberTwoMillActivated)
	{		
			windMillTwoUpperBody.rotate(mouseXDifference * 10 * -1);		
			windMillTwoRotationAnchor.rotate(mouseYDifference * 9 * -1);
	}
	else if (NumberThreeMillActivated)
	{		
			windMillThreeUpperBody.rotate(mouseXDifference * 10 * -1);		
			windMillThreeRotationAnchor.rotate(mouseYDifference * 9 * -1);
	}
	else if (AllMillActivated)
	{		
		windMillOneUpperBody.rotate(mouseXDifference * 10 * -1);
		windMillOneRotationAnchor.rotate(mouseYDifference * 9 * -1);
		windMillTwoUpperBody.rotate(mouseXDifference * 10 * -1);
		windMillTwoRotationAnchor.rotate(mouseYDifference * 9 * -1);
		windMillThreeUpperBody.rotate(mouseXDifference * 10 * -1);
		windMillThreeRotationAnchor.rotate(mouseYDifference * 9 * -1);		
	}
	 
    // do stuff.
    window.clear();
	window.draw(cloudSprite); //draw cloud before the windmills 
    //draw windmills using their rotation anchors as parent	
	millOneRotationAnchor.Draw(&window);
	millTwoRotationAnchor.Draw(&window);	
	millThreeRotationAnchor.Draw(&window); 	
    window.display();
  }
  return 0;
}