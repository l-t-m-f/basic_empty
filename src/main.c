#include "main.h"

#define TERMINATE(code)                                                       \
  do                                                                          \
    {                                                                         \
      end_code = code;                                                        \
      goto terminate;                                                         \
    }                                                                         \
  while (0)

int
main (__attribute__ ((unused)) int argc, __attribute__ ((unused)) char *argv[])
{

  SDL_Window *window;
  SDL_Renderer *renderer;
  uint8_t end_code;
  bool should_quit;
  FC_Font *font;
  uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_RENDERER_ACCELERATED
                   | SDL_RENDERER_PRESENTVSYNC;

  SDL_Init (SDL_INIT_VIDEO);
  Mix_Init (MIX_INIT_OGG | MIX_INIT_MP3);
  TTF_Init ();

  SDL_version compiled;
  SDL_version linked;
  SDL_GetVersion (&linked);
  SDL_VERSION (&compiled)

  SDL_LogInfo (
      SDL_LOG_CATEGORY_APPLICATION,
      "SDL2 version...\n      Linked:    %hu_%hu\n      Compiled:  %hu_%hu\n",
      linked.major, linked.minor, compiled.major, compiled.minor);

  if (SDL_CreateWindowAndRenderer (WIDTH, HEIGHT, flags, &window, &renderer)
      < 0)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ASSERT,
                    "Failed to create window and/or renderer:\n %s\n",
                    SDL_GetError ());
      TERMINATE (1);
    }
  SDL_SetWindowTitle (window, "pathfinding");

  font = FC_CreateFont ();
  FC_LoadFont (font, renderer, "data/wadim_giant.ttf", 20,
               FC_MakeColor (255, 255, 255, 255), TTF_STYLE_NORMAL);

  SDL_Surface *render_target = SDL_CreateRGBSurfaceWithFormat (
      0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
  SDL_Rect test_rect = { 0, 0, WIDTH / 4, HEIGHT / 4 };

  uint64_t frame_count = 0, unlimited_frame_count = 0, frame_count_total = 0,
           unlimited_frame_count_total = 0, start, end;
  float accumulator = 0, fps_timer = 0, dt;

  // test values
  uint8_t red_amount = 0;

  SDL_Point window_size;
  SDL_GetWindowSizeInPixels (window, &window_size.x, &window_size.y);

  SDL_Texture *final_draw_texture;
  SDL_Rect final_draw_rect = { 0, 0, window_size.x, window_size.y };

  int32_t random_n;

  SDL_Log ("Aspect ratio defined to be ... %.4f", ASPECT_RATIO);

  char *title = malloc (50);

  while (!should_quit)
    {

      start = SDL_GetPerformanceCounter ();

      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          if (event.type == SDL_QUIT)
            {
              should_quit = true;
            }

          if (event.type == SDL_RENDER_TARGETS_RESET)
            {
              FC_ResetFontFromRendererReset (font, renderer,
                                             SDL_RENDER_TARGETS_RESET);
            }

          if (event.type == SDL_WINDOWEVENT)
            {
              if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                  SDL_GetWindowSizeInPixels (window, &window_size.x,
                                             &window_size.y);
                  float actual_ratio
                      = (float)window_size.x / (float)window_size.y;
                  float ratio_diff = actual_ratio - ASPECT_RATIO;
                  if (ratio_diff > 0)
                    {
                      final_draw_rect.w
                          = (int)((float)window_size.y * ASPECT_RATIO);
                      final_draw_rect.h = window_size.y;
                      final_draw_rect.x
                          = (window_size.x - final_draw_rect.w) / 2;
                      final_draw_rect.y = 0;
                    }
                  else if (ratio_diff < 0)
                    {
                      final_draw_rect.w = window_size.x;
                      final_draw_rect.h
                          = (int)((float)window_size.x / ASPECT_RATIO);
                      final_draw_rect.x = 0;
                      final_draw_rect.y
                          = (window_size.y - final_draw_rect.h) / 2;
                    }
                  else
                    {
                      final_draw_rect.w = window_size.x;
                      final_draw_rect.h = window_size.y;
                      final_draw_rect.x = 0;
                      final_draw_rect.y = 0;
                    }
                }
            }
        }

      // Ensures that the game logic is updated in fixed time steps
      while (accumulator >= TARGET_RATE)
        {
          // Update game logic here
          accumulator -= TARGET_RATE;
          red_amount = red_amount == UINT8_MAX ? 0 : red_amount + 1;
          frame_count++;
          frame_count_total++;
        }

      // Update based on any frame timing with this method
      if (frame_count % 60 == 0)
        {
          generate_random_number (&random_n);
        }

      // Non-graphic code outside of the fixed time step loop will be updated
      // very fast
      unlimited_frame_count_total++;
      unlimited_frame_count++;

      SDL_FillRect (render_target, NULL,
                    SDL_MapRGBA (render_target->format, red_amount, 0, 0,
                                 SDL_ALPHA_OPAQUE));

      SDL_FillRect (
          render_target, &test_rect,
          SDL_MapRGBA (render_target->format, 0, 255, 0, SDL_ALPHA_OPAQUE));

      final_draw_texture
          = SDL_CreateTextureFromSurface (renderer, render_target);

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
      SDL_RenderClear (renderer);

      SDL_RenderCopy (renderer, final_draw_texture, NULL, &final_draw_rect);

      FC_Draw (font, renderer, (float)(final_draw_rect.x + 10),
               (float)(final_draw_rect.y + 10), "%d\n\n%d\n\n%d, %d\n\n\n%d",
               frame_count_total, unlimited_frame_count_total, window_size.x,
               window_size.y, random_n);

      SDL_RenderPresent (renderer);

      SDL_DestroyTexture (final_draw_texture);

      end = SDL_GetPerformanceCounter ();
      dt = (float)(end - start) / (float)SDL_GetPerformanceFrequency ();
      accumulator += dt;

      // FPS calculation
      fps_timer += dt;
      if (fps_timer >= 1.0)
        { // Update FPS every second
          float fps = (float)frame_count / fps_timer;
          float fps_unlimited = (float)unlimited_frame_count / fps_timer;
          sprintf (title, "pathfinding - FPS: %.2f ~ %.2f", fps,
                   fps_unlimited);
          SDL_SetWindowTitle (window, title);
          frame_count = 0;
          unlimited_frame_count = 0;
          fps_timer = 0; // Reset fps_timer after updating FPS
        }
    }

  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  SDL_Quit ();

  return 0;

terminate:

  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  SDL_Quit ();

  return end_code;
}
