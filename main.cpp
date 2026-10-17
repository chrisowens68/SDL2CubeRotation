#include "Screen.h"

//this is a test comment

int main(int argc, char** argv) {
    Uint32 frameStart;
    float frameTime;

    Screen* screen = new Screen();
    if (screen->init() == false) return 1;

    while (screen->running()) {
        frameStart = SDL_GetTicks();

        
        screen->handleEvents();
        screen->update();
        screen->render();
        screen->clearPoints();

        

        //std::cout << "FPS " << 1/frameTime << std::endl;

        //if (FRAME_DELAY > frameTime) {
        //    SDL_Delay(FRAME_DELAY - frameTime);
        //}

        screen->frameCountInc();

        if (screen->getFrameCount() == 100)
        {

            frameTime = SDL_GetTicks() - frameStart;

            std::cout << "FPS " << frameTime/100*1000 << std::endl;
            //std::cout << "Framecount " << screen->getFrameCount() << std::endl;

            frameStart = SDL_GetTicks();
            screen->frameCountReset();
        }




    }

    screen->quit();

    return 0;
}