#define keyDown(b) input->buttons[b].keyDown
#define pressed(b) (input->buttons[b].keyDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].keyDown && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;

static void SimulatePlayer(float *p, float *dp, float ddp, float dt)
{
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y)
	{
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y)
	{
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

static bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y)
{
	return p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y;
}

enum GameMode {
	GM_MENU,
	GM_GAMEPLAY
};

GameMode currentGameMode;
int hotButton;
bool enemy_is_ai;


static void SimulateGame(Input* input, float dt)
{
	DrawRect(0, 0, arena_half_size_x, arena_half_size_y, 0x0055D2);
	DrawArenaBorders(arena_half_size_x, arena_half_size_y, 0x00A0FD);

	if (currentGameMode == GM_GAMEPLAY)
	{

		float player_1_ddp = 0.f;

		if (!enemy_is_ai)
		{
			if (keyDown(BUTTON_UP)) player_1_ddp += 2000;
			if (keyDown(BUTTON_DOWN)) player_1_ddp -= 2000;
		}
		else
		{
			//if (ball_p_y > player_1_p+2.f) player_1_ddp += 1300;
			//if (ball_p_y < player_1_p+2.f) player_1_ddp -= 1300;
			player_1_ddp = (ball_p_y - player_1_p) * 100;
			if (player_1_ddp > 1300) player_1_ddp = 1300;
			if (player_1_ddp < -1300) player_1_ddp = -1300;
		}
		float player_2_ddp = 0.f;
		if (keyDown(BUTTON_W)) player_2_ddp += 2000;
		if (keyDown(BUTTON_S)) player_2_ddp -= 2000;

		SimulatePlayer(&player_1_p, &player_1_dp, player_1_ddp, dt);
		SimulatePlayer(&player_2_p, &player_2_dp, player_2_ddp, dt);

		// SIMULATE BALL
		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;

			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y))
			{
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
			}

			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y))
			{
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y)
			{
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y)
			{
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x)
			{
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x)
			{
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;
			}
		}

		DrawNumber(player_1_score, -10, 40, 1.f, 0xffffff);
		DrawNumber(player_2_score, 10, 40, 1.f, 0xffffff);

		// RENDERING
		DrawRect(ball_p_x, ball_p_y, ball_half_size, 1, 0xffffff);

		DrawRect(80, player_1_p, player_half_size_x, player_half_size_y, 0x00E8FF);
		DrawRect(-80, player_2_p, player_half_size_x, player_half_size_y, 0x00E8FF);
	} 
	else 
	{
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) 
		{
			hotButton = !hotButton;
		}


		if (pressed(BUTTON_ENTER))
		{
			currentGameMode = GM_GAMEPLAY;
			enemy_is_ai = hotButton ? 0 : 1;
		}

		if (hotButton == 0)
		{
			DrawText("SINGLE PLAYER", -80, -10, 1, 0x00E8FF);
			DrawText("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
		} 
		else 
		{
			DrawText("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			DrawText("MULTIPLAYER", 20, -10, 1, 0x00E8FF);
		}

		DrawText("PONG GAME", -50, 40, 2, 0xffffff);
	}
}