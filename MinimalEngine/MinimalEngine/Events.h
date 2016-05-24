#pragma once

/*
Events.h - self explanatory

Since all an event can do is change the position or dimension of the box, I'm not going to make sep. events, just one basic events class.
*/

struct Event //Represents a move or resize event
{
	int dx;
	int dy;
	int dw;
	int dh;
	int destId;
	Event()
	{
		dx = dy = dw = dh = 0;
	}

	Event(int dfx, int dfy, int dfw, int dfh, int id) : dx(dfx), dy(dfy), dw(dfw), dh(dfh), destId(id) {} //initializer list, perfectly legal
};

struct IListener //Interface for all listeners. Just implement this. Just a way to organize things so that one can make a nice list of 'em.
{
	int Id; //IMPORTANT - You MUST fill in the Id with a unique number. Either make a generator class or just hardcode it.
	
	void setId(int id) //clean way of doing it.
	{
		Id = id;
	}

	virtual void onTrigger(Event *event) = 0; //pure virtual function, have to override. Example in Source.cpp. Basically actionPerformed.

	virtual ~IListener() {} //virtual destructor. Important if you call the destructor on a derived object through a base class pointer.
};
