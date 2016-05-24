#pragma once

/*
Base.h - Contains main classes, and all the includes.

Notes - A struct is equivalent to a public class, you can even inherit from it.
Useful for all-public stuff.

Any class declared with an 'I' in its name is conceptually an interface, you need a class/struct to implement it. Just a convenient convention.
Eg. IEvent, IUnknown

Windowing
---------
Steps:
1. Make a Window object.
2. Use Window's createWindow() function.
3. Make a Renderer object.
4. Use Renderer's createRenderer(), pass a pointer to the Window object in the function
Voila. Windowing ready.

I know. All inline functions. Laziness.
*/

#include <SDL.h>
#include "Events.h"
#include <Windows.h>
#include <iostream>
#include <list>
#include <stack>
using namespace std;

enum class Colors
{
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK
};

struct Window
{
	SDL_Window *window;
	SDL_Surface *windowSurface;
	Window()
	{
		window = NULL;
		windowSurface = NULL;
	}

	bool createWindow(const char *title, int width, int height)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL)
			return false;
		else
			windowSurface = SDL_GetWindowSurface(window);
		return true;
	}

	~Window()
	{
		SDL_DestroyWindow(window);
	}
};

struct Box // represents a rectangle. Make a list if you want more than one. Basically a wrapper around SDL_Rect.
{
	SDL_Rect parameters;
	Colors defaultColor;

	void createBox(int x, int y, int width, int height, Colors color) //origin is top-left, not top-right. X is as usual, Y is reversed.
	{
		defaultColor = color;
		parameters.x = x;
		parameters.y = y;
		parameters.w = width;
		parameters.h = height;
	}

	void deltaPosition(int dx, int dy) //changes position of box by dx and dy.
	{
		parameters.x += dx;
		parameters.y += dy;
	}

	void deltaDimension(int dw, int dh) //similar to deltaPosition, caps values to 0. Position can be negative, dimensions can't.
	{
		parameters.w += dw;
		if (parameters.w < 0)
			parameters.w = 0;

		parameters.h += dh;
		if (parameters.h < 0)
			parameters.h = 0;
	}

	virtual ~Box() {}
};

struct Renderer
{
	SDL_Renderer *renderer;
	Renderer()
	{
		renderer = NULL;
	}

	bool createRenderer(Window *win)
	{
		renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
			return false;
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // sets color to Black
		return true;
	}

	void update() //use this after rendering to push results to the screen
	{
		SDL_RenderPresent(renderer);
	}
	
	void setColor(Colors color)
	{
		switch (color)
		{
		case Colors::RED:
			SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
			break;
		case Colors::BLUE:
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
			break;
		case Colors::GREEN:
			SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
			break;
		case Colors::WHITE:
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			break;
		case Colors::BLACK:
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
			break;
		default:
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
			break;
		}
	}

	void clear() //Use this before starting a new frame. Will reset the color to black.
	{
		setColor(Colors::BLACK);
		SDL_RenderClear(renderer);
	}

	void renderBox(Box *box) //Box rendering galore. Uses the box's default color.
	{
		setColor(box->defaultColor);
		SDL_RenderDrawRect(renderer, &(box->parameters));
	}

	void renderBoxList(list<Box*>& boxList) //All in one utility to render boxes. Clears screen automatically.
	{
		clear();
		for (auto temp = boxList.begin(); temp != boxList.end(); temp++)
		{
			renderBox(*temp);
		}
	}

	~Renderer()
	{
		SDL_DestroyRenderer(renderer);
	}
};

bool initialize()
{
	return (SDL_Init(SDL_INIT_VIDEO)) ? false : true; //laziness
}