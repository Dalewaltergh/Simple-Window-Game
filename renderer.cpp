static void RenderBackground()
{
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int y = 0; y < renderState.height; y++)
	{
		for (int x = 0; x < renderState.width; x++)
		{
			*pixel++ = x * y;
		}
	}
}

static void ClearScreen(unsigned int color)
{
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int y = 0; y < renderState.height; y++)
	{
		for (int x = 0; x < renderState.width; x++)
		{
			*pixel++ = color;
		}
	}
}

static void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color)
{
	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);

	for (int y = y0; y < y1; y++)
	{
		unsigned int* pixel = (unsigned int*)renderState.memory + x0 + y* renderState.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

static float renderScale = 0.01f;

static void DrawArenaBorders(float arenaX, float arenaY, unsigned color) 
{
	arenaX *= renderState.height * renderScale;
	arenaY *= renderState.height * renderScale;

	int x0 = (int)((float)renderState.width * .5f - arenaX);
	int x1 = (int)((float)renderState.width * .5f + arenaX);
	int y0 = (int)((float)renderState.height * .5f - arenaY);
	int y1 = (int)((float)renderState.height * .5f + arenaY);

	DrawRectInPixels(0, 0, renderState.width, y0, color);
	DrawRectInPixels(0, y1, x1, renderState.height, color);
	DrawRectInPixels(0, y0, x0, y1, color);
	DrawRectInPixels(x1, y0, renderState.width, renderState.height, color);
}

static void DrawRect(float x, float y, float halfSizeX, float halfSizeY, unsigned int color)
{
	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfSizeX *= renderState.height * renderScale;
	halfSizeY *= renderState.height * renderScale;

	x += renderState.width / 2.f;
	y += renderState.height / 2.f;

	// CHANGE TO PIXELS
	int x0 = x - halfSizeX;
	int x1 = x + halfSizeX;
	int y0 = y - halfSizeY;
	int y1 = y + halfSizeY;

	DrawRectInPixels(x0, y0, x1, y1, color);
}

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

static void DrawText(const char* text, float x, float y, float size, unsigned int color)
{
	float halfSize = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32)
		{
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26]; 
			else letter = letters[*text - 'A'] ;
			float original_x = x;

			for (int i = 0; i < 7; i++)
			{
				const char* row = letter[i];
				while (*row)
				{
					if (*row == '0')
					{
						DrawRect(x, y, halfSize, halfSize, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

static void DrawNumber(int number, float x, float y, float size, unsigned int color)
{
	float halfSize = size * .5f;

	bool drawnNumber = false;

	while (number || !drawnNumber) {
		drawnNumber = true;
		int digit = number % 10;
		number = number / 10;
		switch (digit)
		{
			case 0:
			{
				DrawRect(x - size, y, halfSize, 2.5f * size, color);
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				DrawRect(x, y + size * 2.f, halfSize, halfSize, color);
				DrawRect(x, y - size * 2.f, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 1:
			{
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				x -= size * 2.f;
			} break;
			case 2:
			{
				DrawRect(x, y + size * 2.f, 1.5f * size, halfSize, color);
				DrawRect(x, y, 1.5f * size, halfSize, color);
				DrawRect(x, y - size * 2.f, 1.5f * size, halfSize, color);
				DrawRect(x + size, y + size, halfSize, halfSize, color);
				DrawRect(x - size, y - size, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 3:
			{
				DrawRect(x - halfSize, y + size * 2.f, size, halfSize, color);
				DrawRect(x - halfSize, y, size, halfSize, color);
				DrawRect(x - halfSize, y - size * 2.f, size, halfSize, color);
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				x -= size * 4.f;
			} break;
			case 4:
			{
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				DrawRect(x - size, y + size, halfSize, 1.5f * size, color);
				DrawRect(x, y, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 5:
			{
				DrawRect(x, y + size * 2.f, 1.5f * size, halfSize, color);
				DrawRect(x, y, 1.5 * size, halfSize, color);
				DrawRect(x, y - size * 2.f, 1.5f * size, halfSize, color);
				DrawRect(x - size, y + size, halfSize, halfSize, color);
				DrawRect(x + size, y - size, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 6:
			{
				DrawRect(x + halfSize, y + size * 2.f, size, halfSize, color);
				DrawRect(x + halfSize, y, size, halfSize, color);
				DrawRect(x + halfSize, y - size * 2.f, size, halfSize, color);
				DrawRect(x - size, y, halfSize, 2.5f * size, color);
				DrawRect(x + size, y - size, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 7:
			{
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				DrawRect(x - halfSize, y + size * 2.f, size, halfSize, color);
				x -= size * 4.f;
			} break;
			case 8:
			{
				DrawRect(x - size, y, halfSize, 2.5f * size, color);
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				DrawRect(x, y + size * 2.f, halfSize, halfSize, color);
				DrawRect(x, y - size * 2.f, halfSize, halfSize, color);
				DrawRect(x, y, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
			case 9:
			{
				DrawRect(x - halfSize, y + size * 2.f, size, halfSize, color);
				DrawRect(x - halfSize, y, size, halfSize, color);
				DrawRect(x - halfSize, y - size * 2.f, size, halfSize, color);
				DrawRect(x + size, y, halfSize, 2.5f * size, color);
				DrawRect(x - size, y + size, halfSize, halfSize, color);
				x -= size * 4.f;
			} break;
		}
	}
}