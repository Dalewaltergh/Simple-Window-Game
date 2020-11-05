struct Button_State
{
	bool keyDown;
	bool changed;
};

enum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,

	BUTTON_COUNT, // SHOULD BE THE LAST ITEM
};

struct Input 
{
	Button_State buttons[BUTTON_COUNT];
};