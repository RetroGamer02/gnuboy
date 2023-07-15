#include <3ds.h>
#include <SDL/SDL.h>
#include "input.h"
#include "rc.h"

//Matrix containing the name of each key. Useful for printing when a key is pressed
const char keysNames[32][32] = {
    "KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
    "KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
    "KEY_R", "KEY_L", "KEY_X", "KEY_Y",
    "", "", "KEY_ZL", "KEY_ZR",
    "", "", "", "",
    "KEY_TOUCH", "", "", "",
    "KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
    "KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
};

static int use_joy = 1;

rcvar_t joy_exports[] =
{
	RCV_BOOL("joy", &use_joy, "enable joystick"),
	RCV_END
};

void joy_init()
{
	/* Initilize the Joystick, and disable all later joystick code if an error occured */
	if (!use_joy) return;

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK))
		return;

	SDL_JoystickOpen(0);

	/* make sure that Joystick event polling is a go */
	SDL_JoystickEventState(SDL_ENABLE);
}

void joy_close() {}

void sdljoy_process_event(SDL_Event *event)
{
    event_t ev;

    u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame
    
    //Scan all the inputs. This should be done once for each frame
    //hidScanInput(); //Already done by SDL so only enable if all SDL code is replaced.

    //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
    u32 kDown = hidKeysDown();
    //hidKeysHeld returns information about which buttons have are held down in this frame
    //u32 kHeld = hidKeysHeld();
    //hidKeysUp returns information about which buttons have been just released
    u32 kUp = hidKeysUp();

    if (kDown & KEY_START)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY0;
        ev_postevent(&ev);
    }
    if (kUp & KEY_START)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY0;
        ev_postevent(&ev);
    }

    if (kDown & KEY_SELECT)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY7;
        ev_postevent(&ev);
    }
    if (kUp & KEY_SELECT)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY7;
        ev_postevent(&ev);
    }

    if (kDown & KEY_A)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY1;
        ev_postevent(&ev);
    }
    if (kUp & KEY_A)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY1;
        ev_postevent(&ev);
    }

    if (kDown & KEY_B)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY2;
        ev_postevent(&ev);
    }
    if (kUp & KEY_B)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY2;
        ev_postevent(&ev);
    }

    if (kDown & KEY_Y)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY4;
        ev_postevent(&ev);
    }
    if (kUp & KEY_Y)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY4;
        ev_postevent(&ev);
    }

    if (kDown & KEY_L)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY5;
        ev_postevent(&ev);
    }
    if (kUp & KEY_L)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY5;
        ev_postevent(&ev);
    }

    if (kDown & KEY_R)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOY6;
        ev_postevent(&ev);
    }
    if (kUp & KEY_R)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOY6;
        ev_postevent(&ev);
    }

    if (kDown & KEY_DUP)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYUP;
        ev_postevent(&ev);
    }
    if (kUp & KEY_DUP)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOYUP;
        ev_postevent(&ev);
    }

    if (kDown & KEY_DDOWN)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYDOWN;
        ev_postevent(&ev);
    }
    if (kUp & KEY_DDOWN)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOYDOWN;
        ev_postevent(&ev);
    }

    if (kDown & KEY_DLEFT)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYLEFT;
        ev_postevent(&ev);
    }
    if (kUp & KEY_DLEFT)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOYLEFT;
        ev_postevent(&ev);
    }

    if (kDown & KEY_DRIGHT)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYRIGHT;
        ev_postevent(&ev);
    }
    if (kUp & KEY_DRIGHT)
    {
        ev.type = EV_RELEASE;
        ev.code = K_JOYRIGHT;
        ev_postevent(&ev);
    }

    circlePosition pos;

    //Read the CirclePad position
    hidCircleRead(&pos);

    if (pos.dx < -15)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYLEFT;
        ev_postevent(&ev);
    } else if (pos.dx > 15)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYRIGHT;
        ev_postevent(&ev);
    } else {
        ev.type = EV_RELEASE;
        ev.code = K_JOYLEFT;
        ev_postevent(&ev);
        ev.type = EV_RELEASE;
        ev.code = K_JOYRIGHT;
        ev_postevent(&ev);
    }

    if (pos.dy > 15)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYUP;
        ev_postevent(&ev);
    } else if (pos.dy < -15)
    {
        ev.type = EV_PRESS;
        ev.code = K_JOYDOWN;
        ev_postevent(&ev);
    } else {
        ev.type = EV_RELEASE;
        ev.code = K_JOYUP;
        ev_postevent(&ev);
        ev.type = EV_RELEASE;
        ev.code = K_JOYDOWN;
        ev_postevent(&ev);
    }

    //Set keys old values for the next frame
    kDownOld = kDown;
    //kHeldOld = kHeld;
    kUpOld = kUp;
}