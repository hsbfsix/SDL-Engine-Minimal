#include "Base.h"
#include <Windows.h>
#include <iostream>
#include <list>
#include <stack>
using namespace std;

//example IListener type class
#define KEYBOARD_BOX_1_ID 1 //just for lols

struct KeyboardBox : public IListener, public Box 
{
	virtual void onTrigger(Event *event) override //This is how you override. Same function signature, add override keyword in front, voila.
	{
		deltaPosition(event->dx, event->dy);
		deltaDimension(event->dw, event->dh);
	}
};


int main(int argc, char **argv)
{
	if (!initialize())
	{
		cout << "Well. Lol. Init failed.";
		return 0;
	}

	Window *gWin;
	Renderer *gRend;

	KeyboardBox *keyBox1 = new KeyboardBox();
	keyBox1->createBox(100, 100, 30, 30); //must call createBox() or you'll get nothing
	keyBox1->setId(KEYBOARD_BOX_1_ID); //don't forget to set a unique Id, or events won't reach it

	list<IListener*> listeners;
	//add all your Listener objects (classes that implement IListener) to this vector
	listeners.push_back(keyBox1);

	gWin->createWindow("Something", 640, 480);
	gRend->createRenderer(gWin);

	//I'm not going to abstract the event loop too much - it's too basic. No multithreading either.
	
	SDL_Event e;
	stack<Event*> eventStack;

	bool quitFlag = false;

	while (!quitFlag)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quitFlag = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_w)
				{
					eventStack.push(new Event(0, 20, 0, 0, KEYBOARD_BOX_1_ID)); //specifiy destination of the event using the id
				}
				else if (e.key.keysym.sym == SDLK_d)
				{
					eventStack.push(new Event(0, -20, 0, 0, KEYBOARD_BOX_1_ID)); //specifiy destination of the event using the id
				}
			}
		}

		//add code to add events to eventStack from the Leap here

		//event pump
		

	}
	SDL_Delay(5000);

	return 0;
}