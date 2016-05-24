#include "Base.h"

//Read Base.h first. Then Events.h. Then this.

//example IListener type class
#define KEYBOARD_BOX_1_ID 1 //just for lols
#define KEYBOARD_BOX_2_ID 2 //just to show why I did all that IListener and list stuff. Adding another box is trivial.

struct KeyboardBox : public IListener, public Box 
{
	virtual void onTrigger(Event *event) override //This is how you override. Same function signature, add override keyword in front, voila.
	{
		deltaPosition(event->dx, event->dy);
		deltaDimension(event->dw, event->dh);
		delete event;
	}
};

int main(int argc, char **argv)
{
	if (!initialize())
	{
		cout << "Well. Lol. Init failed.";
		return 0;
	}

	Window *gWin = new Window();
	Renderer *gRend = new Renderer();

	gWin->createWindow("Something", 640, 480);
	gRend->createRenderer(gWin);

	KeyboardBox *keyBox1 = new KeyboardBox();
	keyBox1->createBox(100, 100, 30, 30, Colors::RED); //must call createBox() or you'll get nothing
	keyBox1->setId(KEYBOARD_BOX_1_ID); //don't forget to set a unique Id, or events won't reach it

	KeyboardBox *keyBox2 = new KeyboardBox();
	keyBox2->createBox(200, 100, 30, 30, Colors::BLUE); //must call createBox() or you'll get nothing
	keyBox2->setId(KEYBOARD_BOX_2_ID); //don't forget to set a unique Id, or events won't reach it

									   
	//add all your Listener objects (classes that implement IListener) to this list
	list<IListener*> listeners;
	listeners.push_back(keyBox1);
	listeners.push_back(keyBox2);

	//add all the Box objects here.
	list<Box*> boxList;
	boxList.push_back(keyBox1);
	boxList.push_back(keyBox2);
	
	
	//I'm not going to abstract the event loop too much - it's too basic. No multithreading either.
	//Here, W and S control the red box, keyBox1
	// T and G control the blue box, keyBox2
	// You'll notice that adding multiple boxes simply involves making a new ID, pushing the box onto the relevant lists
	// and then adding the relevant events. 
	// For the Leap just use another struct to implement IListener and Box, but generate the events from the Leap code
	// , tag the events with another unique ID, and then push them onto the eventStack in the indicated space, before the 
	// event pump.

	//Event Loop structure :
	// check QuitFlag(0) ---> pollEvent(1) ---> check if quit or keys, push events, jump back to (1) ---> push Leap events ---> event pump ---> render ---> jump back to (0)

	SDL_Event e;
	stack<Event*> eventStack;

	bool quitFlag = false;

	while (!quitFlag)
	{
		while (SDL_PollEvent(&e)) //standard SDL event handling loop
		{
			if (e.type == SDL_QUIT)
			{
				quitFlag = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//For box 1
				if (e.key.keysym.sym == SDLK_s)
				{
					eventStack.push(new Event(0, 20, 0, 0, KEYBOARD_BOX_1_ID)); //specifiy destination of the event using the id
				}
				else if (e.key.keysym.sym == SDLK_w)
				{
					eventStack.push(new Event(0, -20, 0, 0, KEYBOARD_BOX_1_ID)); //specifiy destination of the event using the id
				}

				//For box 2
				else if (e.key.keysym.sym == SDLK_t)
				{
					eventStack.push(new Event(0, -20, 0, 0, KEYBOARD_BOX_2_ID)); //Notice the different ID
				}
				else if (e.key.keysym.sym == SDLK_g)
				{
					eventStack.push(new Event(0, 20, 0, 0, KEYBOARD_BOX_2_ID)); //Notice the different ID
				}
			}
		}

		//add code to add events to eventStack from the Leap here
		//eventsFromLeap(); --some dummy function

		//event pump
		Event *temp;
		bool processed;

		while (!eventStack.empty())
		{
			processed = false;
			temp = eventStack.top();
			for (auto t = listeners.begin(); t != listeners.end(); t++)
			{
				if (temp->destId == (*t)->Id)
				{
					(*t)->onTrigger(temp);
					processed = true;
					break;
				}
			}
			if (processed == false)
				delete temp;
			eventStack.pop();
		}

		//The actual rendering. Shady. You probably don't need to change this.
		gRend->renderBoxList(boxList);
		gRend->update();
	}

	//SDL_Delay(5000);
	delete keyBox1;
	delete keyBox2;
	delete gRend;
	delete gWin;
	return 0;
}